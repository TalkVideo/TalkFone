/*
	(c) Forschung-Direkt EOOD 2009
	http://www.forschung-direkt.eu
	This software is licensed under Lesser General Public License (LGPL)  v3 License. 
*/
#ifndef CPPDOTTELUTILS_H
#define CPPDOTTELUTILS_H

#include "dottelutils.h"
#include <string>
#include <vector>

using namespace std;

/* Stores .tel query results */
struct CppTelRecord {
	enum phone_type		ph_type;
	string				content;		/* number or TXT value*/
	string				descriptor;		/* phone label or TXT descriptor */
};

class CppDotTelUtils {

public:
	
	CppDotTelUtils();
	~CppDotTelUtils();
	
	/* Initialize resolver, push DNS servers */
	bool init();

	/* Destroy resolver, stop winsock if windows */
	void destroy();

	/* Get .tel data, given a domain name and type of query */
	vector<CppTelRecord> getData(int dataType, string domainName);	

	/* Get .tel numbers, given a domain name. Result 
	*returned in TelRecord array with length reccount*/
	vector<CppTelRecord> getNumbers(string domainName);

	/* Get .tel TXT records, given a domain name. Result returned 
	*  in TelRecord array with length reccount*/
	vector<CppTelRecord> getDescriptions(string domainName);

	/* Convert phone type to string values, defined in by PH_TYPE_* macros in this header*/
	string phTypeToStr(enum phone_type p_type);	
};

#endif


