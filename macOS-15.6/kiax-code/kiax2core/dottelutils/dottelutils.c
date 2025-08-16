/*
	(c) Forschung-Direkt EOOD 2009
	http://www.forschung-direkt.eu
	This software is licensed under Lesser General Public License (LGPL)  v3 License. 
*/

#include "dottelutils.h"

#ifdef WIN32DEP
#include <windows.h>
#include <winsock2.h>
#include <iphlpapi.h>
#endif

/* Initialize resolver, push DNS servers */
extern bool initDotTel()
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
	printf("Winsock not initialized. Attempt to re-initialize it.\n");
	initWinsock();
	if (winsockError!=0)
	{
		printf("Attempt to re-initialize winsock failed.");
		return false;
	}
	
   /* get assigned DNS servers */
   
	FIXED_INFO * FixedInfo;
	ULONG    ulOutBufLen;
	DWORD    dwRetVal;
	IP_ADDR_STRING * pIPAddr;
	
	FixedInfo = (FIXED_INFO *) GlobalAlloc( GPTR, sizeof( FIXED_INFO ) );
	ulOutBufLen = sizeof( FIXED_INFO );

	/* get networking parameters. we get the DNS servers too. */
	/* this could be done with DnsQueryConfig but then we are dependent on dnsapi.dll */
	
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

/* Destroy resolver, stop winsock if windows */
extern void destroyDotTel()
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


#ifdef WIN32DEP

/* pushed DNS servers are randomly selected by ldns_resolver upon query send */
bool pushDnsServer(char* ipAddress)
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

void initWinsock()
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

/* Parse NAPTR record data (represented as string) 
* to TelRecord form. Called by processNextRdfsNAPTR */
void parseTel(char* naptrType, char* naptrData, struct TelRecord* number)
{
	/* get number */
	if (strstr(naptrType, TAG_VOICE_PHONE)!=NULL)
	{
		char* numberPosition = strchr(naptrData, ':');
		if (numberPosition!=NULL) 
		{		
			numberPosition++;
			int endOfStr = strlen(naptrData);
			int numberPositionIndex = numberPosition - naptrData;
			if (endOfStr>3) endOfStr -= 2;
			int numberLength = endOfStr-numberPositionIndex + 1; /* keep 0 at the end in mind */
			number->content = (char*) malloc(numberLength * sizeof(char*));
			strncpy(number->content, numberPosition, numberLength - 1);
			number->content[numberLength-1] = 0;
		}
	}
	
	/* check labels */
	if (strstr(naptrType, TAG_LABEL)!=NULL)
	{
		char* labelPosition = strstr(naptrType, TAG_LABEL);
		
		/* jump to label value*/
		labelPosition+=strlen(TAG_LABEL);
		char* endOfLabelValue = NULL; 
		if ((strchr(labelPosition, '+') !=NULL)  /* next descriptor reached */
			 || (strchr(labelPosition, '\"') !=NULL)  /* end of RR reached */
			)
			{
				endOfLabelValue = strchr(labelPosition, '+');
				if (endOfLabelValue==NULL)
					endOfLabelValue = strchr(labelPosition, '\"');
				/*We got it*/
				int labelLength = endOfLabelValue - labelPosition + 1;
				number->descriptor = (char*) malloc(labelLength * sizeof(char*));
				strncpy(number->descriptor, labelPosition, labelLength-1);
				number->descriptor[labelLength-1] = 0;
			} else
			{
				/* no label found */
				fprintf(stderr, "no label found for %s\n", naptrData);
				fflush(stderr);
				number->descriptor = NULL;
			}
			
	}
	
	/* check phone type/location */
	if (strstr(naptrType, PHONE_MAIN)!=NULL)
			number->ph_type = ph_main;
	else if (strstr(naptrType, PHONE_MOBILE)!=NULL)
			number->ph_type = ph_mobile;
	else if (strstr(naptrType, PHONE_WORK)!=NULL)
			number->ph_type = ph_work;
	else if (strstr(naptrType, PHONE_HOME)!=NULL)
			number->ph_type = ph_home;
	else if (strstr(naptrType, PHONE_TRANSIT)!=NULL)
			number->ph_type = ph_transit;
	else if (strstr(naptrType, PHONE_PRS)!=NULL)
			number->ph_type = ph_prs;
	else 	number->ph_type = ph_none;

}

/* Delete arrays returned by this API */
extern void deleteTelArray(struct TelRecord* recarray, int reccount)
{
	int i =0;
 	for(i = 0; i < reccount; i++)
	{
		if (recarray[i].content!=NULL)
			free(recarray[i].content);
		if (recarray[i].descriptor!=NULL)
			free(recarray[i].descriptor);
	}
	free(recarray);
}

/* Iteration step for inserting values in reccarray. */
struct TelRecord* processNextRdfsNAPTR(ldns_rdf** rdfs, ldns_rr * rr, struct TelRecord* recarray, int * reccount )
{
	/* locate the field containing ENUM data */
	if (rr->_rd_count > NAPTR_DATA)
	{
		ldns_rdf* typeRdf = rdfs[NAPTR_TYPE];
		char* naptrType = ldns_rdf2str(typeRdf);
		ldns_rdf* dataRdf = rdfs[NAPTR_DATA];
		char* naptrData = ldns_rdf2str(dataRdf);
		struct TelRecord tel;
		tel.descriptor=NULL;
		tel.content = NULL;
		parseTel(naptrType, naptrData, &tel);
		/* check if there are numbers found (tel!=NULL) */
		if (tel.content!=NULL)
		{
			recarray = (struct TelRecord*)realloc(recarray, ((*reccount) + 1) * sizeof(struct TelRecord));
			recarray[(*reccount)].content = strdup(tel.content);
			recarray[(*reccount)].ph_type = tel.ph_type;
			if (tel.descriptor!=NULL)
				recarray[(*reccount)++].descriptor = strdup(tel.descriptor);
			else
				recarray[(*reccount)++].descriptor = NULL;
		}
		LDNS_FREE(tel.content);
	} 

	return recarray;
}

/* Iteration step for inserting values in reccarray. */
struct TelRecord* processNextRdfsTXT(ldns_rdf** rdfs, ldns_rr * rr, struct TelRecord* recarray, int * reccount )
{
	/* locate field containing freeform TXT data */
	ldns_rdf* txtRdf = rdfs[TXT_FREEFORM];
	char* freeForm = ldns_rdf2str(txtRdf);
	if ((strstr(freeForm, "\".")!=freeForm)&&(strlen(freeForm)>4)) /* fields with . are descriptors for typed data, we don't support them yet */
	{
		recarray = (struct TelRecord*)realloc(recarray, ((*reccount) + 1) * sizeof(struct TelRecord));
		recarray[(*reccount)].content = strdup(freeForm);
		recarray[(*reccount)++].descriptor = NULL;
	}
	LDNS_FREE(freeForm);
	
	return recarray;
}

/* Get .tel numbers, given a domain name. Result 
*  returned in TelRecord array with length reccount*/
extern struct TelRecord* getTelNumbers(char* domainName, int* reccount)
{
	return getTelData(TEL_DATA_TEL, domainName, reccount);
}

/* Get .tel data, given a domain name and type of query */
extern struct TelRecord* getTelData(int dataType, char* domainName, int* reccount)
{
	ldns_rdf *domain;
	ldns_pkt *p;
	ldns_rr_list *records;
	
	p = NULL;
	records = NULL;
	domain = NULL;
	(*reccount) = 0;
	
	ldns_rr_type queryType;
	
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
				struct TelRecord* results = NULL;
				
				/* iterate on RRs and check type*/
				for (i = 0; i < rrn; i++) {
					ldns_rr * rr = ldns_rr_list_rr(records, i);
					ldns_rdf** rdfs = rr->_rdata_fields;
					switch (dataType)
					{
						case TEL_DATA_TEL:
							results = processNextRdfsNAPTR(rdfs, rr, results, reccount);
							break;
						case TEL_DATA_TXT:
							results = processNextRdfsTXT(rdfs, rr, results, reccount);
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

/* Get .tel TXT records, given a domain name. Result returned 
*  in TelRecord array with length reccount*/
extern struct TelRecord* getTelDescription(char* domainName, int* reccount)
{
	return getTelData(TEL_DATA_TXT, domainName, reccount);
}

/* Convert phone type to string values, defined in by PH_TYPE_* macros in this header*/
extern void phoneTypeToStr(enum phone_type ph_type, char** targetString)
{
		switch (ph_type) {
			case ph_mobile:
					*targetString = strdup(PH_TYPE_MOBILE);
				break;
			case ph_work:
					*targetString = strdup(PH_TYPE_WORK);
				break;
			case ph_main:
					*targetString = strdup(PH_TYPE_MAIN);
				break;
			case ph_home:
					*targetString = strdup(PH_TYPE_HOME);
				break;
			case ph_transit:
					*targetString = strdup(PH_TYPE_TRANSIT);
				break;
			case ph_prs:
					*targetString = strdup(PH_TYPE_PRS);
				break;
			default:
					*targetString = strdup(PH_TYPE_NONE);
				break;
		}
}

