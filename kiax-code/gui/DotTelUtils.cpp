/*
	(c) Forschung-Direkt EOOD 2009
	http://www.forschung-direkt.eu
	This software is licensed under General Public License (GPL)  v3 License. 
*/

#include "DotTelUtils.h"

#ifdef WIN32DEP
#include <windows.h>
#include <winsock2.h>
#include <iphlpapi.h>
#endif

DotTelUtils::DotTelUtils()
{
#ifdef WIN32DEP
	initWinsock();
#endif
}

DotTelUtils::~DotTelUtils()
{
	if (resolver!=NULL)	
	{
		ldns_resolver_deep_free(resolver);
		resolver = NULL;
	}
#ifdef WIN32DEP
	
	// cleanup winsock
	
	WSACleanup();
	
#endif
}

bool DotTelUtils::initDotTel()
{
	resolver = NULL;
	
#if !defined(WIN32DEP)

	ldns_status status;
	
	/* initialize ldns resolver from /etc/resolv.conf */
	status = ldns_resolver_new_frm_file(&resolver, NULL);

	if (status != LDNS_STATUS_OK) {
		return false;
	}		
	
#else

	/* check winsock status */
	if (winsockError!=0)
	{
		printf("Winsock not initialized. Attempt to re-initialize it.\n");
		initWinsock();
		if (winsockError!=0)
		{
			printf("Attempt to re-initialize winsock failed.");
			return false;
		}
	}
	
   /* get assigned DNS servers */
   
	FIXED_INFO * FixedInfo;
	ULONG    ulOutBufLen;
	DWORD    dwRetVal;
	IP_ADDR_STRING * pIPAddr;
	
	FixedInfo = (FIXED_INFO *) GlobalAlloc( GPTR, sizeof( FIXED_INFO ) );
	ulOutBufLen = sizeof( FIXED_INFO );

	/* get networking parameters. we get the DNS servers too. */
	/* this could ne done with DnsQueryConfig but then we are dependent on dnsapi.dll */
	
	/* first proble to get the size of the buffer */
	if( ERROR_BUFFER_OVERFLOW == GetNetworkParams( FixedInfo, &ulOutBufLen ) ) {
		GlobalFree( FixedInfo );
		FixedInfo = (FIXED_INFO *) GlobalAlloc( GPTR, ulOutBufLen );
	}
	
	/* then call to get data */
	if ( dwRetVal = GetNetworkParams( FixedInfo, &ulOutBufLen ) ) {
		printf( "Call to GetNetworkParams failed.");
		return false;
	}
	else {      

		/* initialize ldns resolver */
		resolver = ldns_resolver_new();
		
		/* get first DNS server IP */		
		char* ipAddress = FixedInfo -> DnsServerList.IpAddress.String;
		bool result = pushDnsServer(ipAddress);
		if (!result)
			return false;
			
		/* iterate on IP list */
		pIPAddr = FixedInfo -> DnsServerList.Next;
		
		while ( pIPAddr ) {
			result = pushDnsServer(pIPAddr ->IpAddress.String);
			if (!result)
				return false;
			pIPAddr = pIPAddr -> Next;
		}
		
   }    
#endif

	/* 512 bytes are not enough sometimes, set to max 4096*/
	ldns_resolver_set_edns_udp_size(resolver, MAX_EDNS_UDP_SIZE);
	
	/* set resolver timeout to 5 seconds */
	struct timeval rtimeout;
	rtimeout.tv_sec = DEFAULT_TIMEOUT_SEC;
    rtimeout.tv_usec = DEFAULT_TIMEOUT_USEC;
	ldns_resolver_set_timeout(resolver, rtimeout);
	
	return true;
}

#ifdef WIN32DEP

/* pushed DNS servers are randomly selected by ldns_resolver upon query send */
bool DotTelUtils::pushDnsServer(char* ipAddress)
{
		ldns_rdf *rdf;
		
		/* create rdf from the DNS Server IP */
		
		/*try ipv6 first */
		rdf = ldns_rdf_new_frm_str(LDNS_RDF_TYPE_AAAA, ipAddress);

		/*try ipv4 */
		if (!rdf) { 
			rdf = ldns_rdf_new_frm_str(LDNS_RDF_TYPE_A, ipAddress);
		}
		
		/* if no success give up */
		if (!rdf) {
			ldns_resolver_deep_free(resolver);
			resolver = NULL;
			printf("Could not create rdf from ip address for nameserver.\n");
			return false;
		}	

		/* set ldns name servers */
		(void)ldns_resolver_push_nameserver(resolver, rdf);
		ldns_rdf_deep_free(rdf);
		
		return true;
}

void DotTelUtils::initWinsock()
{
	/* initialize winsock */
	winsockError = -1;
	WSADATA wsa_data;
	winsockError = WSAStartup(MAKEWORD(2,2), &wsa_data);
	if(winsockError != 0) {
		printf("Failed WSAStartup: %d\n", winsockError);
	}	
}
#endif

char* DotTelUtils::parseTel(char* naptrType, char* naptrData)
{
	if (strstr(naptrType, TAG_VOICE_PHONE)!=NULL)
	{
		char* numberPosition = strchr(naptrData, ':');
		if (numberPosition!=NULL) 
		{		
			numberPosition++;
			int endOfStr = strlen(naptrData);
			int numberPositionIndex = numberPosition - naptrData;
			if (endOfStr>3) endOfStr -= 2;
			int numberLength = endOfStr-numberPositionIndex;
			char* number = (char*) malloc(numberLength * sizeof(char*));
			strncpy(number, numberPosition, numberLength);
			number[numberLength] = 0;
			//printf("number=%s", number);
			return number;
		}
	}
	return NULL;
}
void DotTelUtils::deleteTelArray(char** strarray, int strcount)
{
	int i =0;
 	for(i = 0; i < strcount; i++)
  		free(strarray[i]);
}

char** DotTelUtils::processNextRdfsNAPTR(ldns_rdf** rdfs, ldns_rr * rr, char** strarray, int * strcount )
{
	/* locate the field containing ENUM data */
	if (rr->_rd_count > NAPTR_DATA)
	{
		ldns_rdf* typeRdf = rdfs[NAPTR_TYPE];
		char* naptrType = ldns_rdf2str(typeRdf);
		ldns_rdf* dataRdf = rdfs[NAPTR_DATA];
		char* naptrData = ldns_rdf2str(dataRdf);
		char* tel = parseTel(naptrType, naptrData);
		/* check if there are numbers found (tel!=NULL) */
		if (tel!=NULL)
		{
			//printf("detected telephone number:%s\n", tel);
			strarray = (char **)realloc(strarray, ((*strcount) + 1) * sizeof(char *));
			strarray[(*strcount)++] = strdup(tel);
		}
		LDNS_FREE(tel);
	} 

	return strarray;
}

char** DotTelUtils::processNextRdfsTXT(ldns_rdf** rdfs, ldns_rr * rr, char** strarray, int * strcount )
{
	/* locate field containing freeform TXT data */
	ldns_rdf* txtRdf = rdfs[TXT_FREEFORM];
	char* freeForm = ldns_rdf2str(txtRdf);
	if ((strstr(freeForm, "\".")!=freeForm)&&(strlen(freeForm)>4)) /* fields with . are descriptors for typed data, we don't support them yet */
	{
		//printf("freeform:%s\n", freeForm);
		strarray = (char **)realloc(strarray, ((*strcount) + 1) * sizeof(char *));
		strarray[(*strcount)++] = strdup(freeForm);
	}
	LDNS_FREE(freeForm);
	
	return strarray;
}

char** DotTelUtils::getTelNumbers(char* domainName, int* strcount)
{
	return getTelData(TEL_DATA_TEL, domainName, strcount);
}

char** DotTelUtils::getTelData(int dataType, char* domainName, int* strcount)
{
	ldns_rdf *domain;
	ldns_pkt *p;
	ldns_rr_list *records;
	
	p = NULL;
	records = NULL;
	domain = NULL;
	
	ldns_rr_type queryType;
	
	fprintf(stderr, "number of dns servers in the resolver:%d\n", ldns_resolver_nameserver_count(resolver));
	fflush(stderr);
	/* attempt to reinitialize in case there are no set nameservers */
	if (ldns_resolver_nameserver_count(resolver)==0)
	{
		printf("No nameservers set. Retrying to initialize..");
		if (resolver!=NULL)
		{
			printf("Freeing resolver..");
			ldns_resolver_deep_free(resolver);
			resolver = NULL;
		}
		bool result = initDotTel();
		if (!result)
		{
			printf("Failed attempt to re-initialize resolver.\n");
			return NULL;
		}
	}
	
	domain = ldns_dname_new_frm_str(domainName);

	if (!domain) {
		printf("invalid domain %s\n", domainName);	
		return NULL;
	}

	/* set ldns RR type */
	switch (dataType)
	{
		case TEL_DATA_TEL:
			queryType = LDNS_RR_TYPE_NAPTR;
			break;
		
		case TEL_DATA_TXT:
			queryType = LDNS_RR_TYPE_TXT;
			break;

		default:
			queryType = LDNS_RR_TYPE_NAPTR;
			break;
	}

	/* make a DNS query */
	p = ldns_resolver_query(resolver,
	                        domain,
	                        queryType,
	                        LDNS_RR_CLASS_IN,
	                        LDNS_RD);

	/* free domain, we don't need it anymore */
	ldns_rdf_deep_free(domain);
	
	if (!p)  
	{
			/* bad packet, give up */
			return NULL;
    } 
	else 
	{
		records = ldns_pkt_rr_list_by_type(p,
								  queryType,
								  LDNS_SECTION_ANSWER);
		if (!records) {
				printf( " *** invalid answer name after query for %s\n",
					domainName);
				ldns_pkt_free(p);
				return NULL;
		} else {
				//ldns_rr_list_sort(mx); 
				int rrn = ldns_rr_list_rr_count(records);
				int i =0;
				char** results = NULL;
				
				/* iterate on RRs and check type*/
				for (i = 0; i < rrn; i++) {
					ldns_rr * rr = ldns_rr_list_rr(records, i);
					ldns_rdf** rdfs = rr->_rdata_fields;
					switch (dataType)
					{
						case TEL_DATA_TEL:
							results = processNextRdfsNAPTR(rdfs, rr, results, strcount);
							break;
						case TEL_DATA_TXT:
							results = processNextRdfsTXT(rdfs, rr, results, strcount);
					}
				}
				
				ldns_rr_list_deep_free(records);
				return results;

		}
	}
	ldns_pkt_free(p);
	ldns_resolver_deep_free(resolver);
	return NULL;
}

char** DotTelUtils::getTelDescription(char* domainName, int* strcount)
{
	return getTelData(TEL_DATA_TXT, domainName, strcount);
}

/* uncomment main if you want to use it as a test utility from command prompt */
/*

int main(int argc, char *argv[])
{
	if (argc==2)
	{
		char** numbers = NULL;
		char** descriptions = NULL;
		int length = 0;
	
		DotTelUtils utils;
		bool status = utils.initDotTel();
		if (!status)
		{
			printf("Resolver could not be initialized. Exiting.\n");
			return 0;
		}
		numbers = utils.getTelNumbers((char*)argv[1], &length);
		if (numbers!=NULL)
		{
			printf("Total number of discovered tel numbers is %d.\n", length);
			for (int i=0; i<length; i++)			
				printf("   Number (%d): %s\n", i+1, numbers[i]);
		}
		utils.deleteTelArray(numbers, length);
		length = 0;
		descriptions = utils.getTelDescription((char*)argv[1], &length);
		if (descriptions!=NULL)
		{
			printf("Total number of discovered tel descriptions is %d.\n", length);
			for (int i=0; i<length; i++)			
				printf("   Description (%d): %s\n", i+1, descriptions[i]);
		}
		utils.deleteTelArray(descriptions, length);
	}
	return 0;
}

*/
