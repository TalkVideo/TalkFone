/*
	(c) Forschung-Direkt EOOD 2009
	http://www.forschung-direkt.eu
	This software is licensed under Lesser General Public License (LGPL)  v3 License. 
*/

#ifndef DOTTELUTILS_H
#define DOTTELUTILS_H

#include "ldns/ldns.h"

#define TXT_FREEFORM 0 
#define NAPTR_TYPE  3
#define NAPTR_DATA  4

#define TAG_VOICE_PHONE "E2U+voice:tel"
#define TAG_LABEL "x-lbl:"

#define TEL_DATA_TEL	1
#define TEL_DATA_TXT	2

#define MAX_EDNS_UDP_SIZE		4096 /* 4096 is the max expected UDP packet size for some modem routers */
#define DEFAULT_TIMEOUT_SEC		5	 /* wait 5 timeout for sockets */
#define DEFAULT_TIMEOUT_USEC	0

#define PHONE_MOBILE	"x-mobile"
#define PHONE_WORK		"x-work"
#define PHONE_MAIN		"x-main"
#define PHONE_HOME		"x-home"
#define PHONE_TRANSIT	"x-transit"
#define PHONE_PRS		"x-prs"

#define PH_TYPE_NONE		"Unknown"
#define PH_TYPE_MOBILE		"Mobile"
#define PH_TYPE_WORK		"Work"
#define PH_TYPE_MAIN		"Main"
#define PH_TYPE_HOME		"Home"
#define PH_TYPE_TRANSIT		"Transit"
#define PH_TYPE_PRS			"PRS"

#ifdef __cplusplus

extern  "C" {
#endif

/* Types of phones/locations */
enum phone_type {
	ph_none,
	ph_mobile,
	ph_work,
	ph_main,
	ph_home,
	ph_transit,
	ph_prs
};

/* Stores .tel query results */
struct TelRecord {
	enum phone_type		ph_type;
	char*				content;		/* number or TXT value*/
	char*				descriptor;		/* phone label or TXT descriptor */
};

/* Initialize resolver, push DNS servers */
extern bool initDotTel();

/* Destroy resolver, stop winsock if windows */
extern void destroyDotTel();

/* Delete arrays returned by this API */
extern void deleteTelArray(struct TelRecord* recarray, int reccount);

/* Get .tel data, given a domain name and type of query */
extern struct TelRecord* getTelData(int dataType, char* domainName, int* reccount);	

/* Get .tel numbers, given a domain name. Result 
*  returned in TelRecord array with length reccount*/
extern struct TelRecord* getTelNumbers(char* domainName, int* reccount);

/* Get .tel TXT records, given a domain name. Result returned 
*  in TelRecord array with length reccount*/
extern struct TelRecord* getTelDescription(char* domainName, int* reccount);

/* Convert phone type to string values, defined in by PH_TYPE_* macros in this header*/
extern void phoneTypeToStr(enum phone_type p_type, char** targetString);	

/* Windows routines*/
#ifdef WIN32DEP

/* Store winsock result */
#ifdef __cplusplus
extern
#endif
int winsockError;

/* Initialize winsock, result set to winsockError */
void initWinsock();	

/* Inserts ip addresses as DNS servers into ldns_resolver */
bool pushDnsServer(char* ipAddress);
#endif

#ifdef __cplusplus
extern
#endif
/* main ldns_resolver */
ldns_resolver *resolver;

/* Parse NAPTR record data (represented as string) 
* to TelRecord form. Called by processNextRdfsNAPTR */
void parseTel(char* naptrType, char* naptrData, struct TelRecord* telrecord);

/* Iteration step for inserting values in reccarray. */
struct TelRecord* processNextRdfsNAPTR(ldns_rdf** rdfs, ldns_rr * rr, struct TelRecord* recarray, int * reccount );

/* Iteration step for inserting values in reccarray. */
struct TelRecord* processNextRdfsTXT(ldns_rdf** rdfs, ldns_rr * rr, struct TelRecord* recarray, int * reccount );		

#ifdef __cplusplus
} // close extern
#endif

#endif


