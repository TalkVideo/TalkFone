#ifndef HTTPXMLFETCHER
#define HTTPXMLFETCHER

#include "PConfiguration.h"
#include "tinyxml/tinyxml.h"
#include "Logger.h"

#include <string>
#include <map>
#include <curl/curl.h>

#define FETCH_CONFIGURATION_SUCCESS	0
#define FETCH_CONFIGURATION_ERROR	-1

using namespace std;

class HttpXmlFetcher 
{

public:

        /* constructs the Http Configuration Resolver */
        HttpXmlFetcher();

        /* disposes the resolver */
        ~HttpXmlFetcher();

        virtual int fetchData(string fUrl);

        static int writer(char *data, size_t size, size_t nmemb, std::string *buffer);

        static char errorBuffer[CURL_ERROR_SIZE];

        static string buffer;

}; // end HttpXmlFetcher class

#endif

