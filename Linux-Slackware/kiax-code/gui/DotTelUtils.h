/*
	(c) Forschung-Direkt EOOD 2009
	http://www.forschung-direkt.eu
	This software is licensed under General Public License (GPL)  v3 License. 
*/

#ifndef DOTTELUTILS_H
#define DOTTELUTILS_H


extern "C" {
#include "ldns/ldns.h"
}

#define TXT_FREEFORM 0 
#define NAPTR_TYPE  3
#define NAPTR_DATA  4

#define TAG_VOICE_PHONE "E2U+voice:tel"
#define REGEXP_TEL  "!^.*$!+tel:"

#define TEL_DATA_TEL	1
#define TEL_DATA_TXT	2

#define MAX_EDNS_UDP_SIZE		4096 /* 4096 is the max expected UDP packet size for some modem routers */
#define DEFAULT_TIMEOUT_SEC		5	 /* wait 5 timeout for sockets */
#define DEFAULT_TIMEOUT_USEC	0

class DotTelUtils {

 public:
	
	DotTelUtils();
	~DotTelUtils();
	bool initDotTel();
	void deleteTelArray(char** strarray, int strcount);
	char** getTelData(int dataType, char* domainName, int* strcount);	
	char** getTelNumbers(char* domainName, int* strcount);
	char** getTelDescription(char* domainName, int* strcount);
	
private:

#ifdef WIN32DEP
	int winsockError;
	void initWinsock();	
	bool pushDnsServer(char* ipAddress);
#endif

	ldns_resolver *resolver;
	
	char* parseTel(char* naptrType, char* naptrData);
	char** processNextRdfsNAPTR(ldns_rdf** rdfs, ldns_rr * rr, char** strarray, int * strcount );
	char** processNextRdfsTXT(ldns_rdf** rdfs, ldns_rr * rr, char** strarray, int * strcount );	
	
};
	

#endif


