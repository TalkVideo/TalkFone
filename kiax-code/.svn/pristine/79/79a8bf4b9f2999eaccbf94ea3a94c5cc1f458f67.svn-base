#include "HttpXmlFetcher.h"


char HttpXmlFetcher::errorBuffer[CURL_ERROR_SIZE];
string HttpXmlFetcher::buffer;

HttpXmlFetcher::HttpXmlFetcher()
{
}

HttpXmlFetcher::~HttpXmlFetcher()
{
}

int HttpXmlFetcher::fetchData(string fUrl)
{
    CURL* curl;

    const char* url = fUrl.c_str();

    CURLcode result;

    int funcResult = FETCH_CONFIGURATION_ERROR; // pessimism :)

    // Create curl handle
    curl = curl_easy_init();

    if (curl)
    {
      // Curl options
      curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errorBuffer);
      curl_easy_setopt(curl, CURLOPT_URL, url);
      curl_easy_setopt(curl, CURLOPT_HEADER, 0);
      curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
      curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);
      curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
      // Retrieve the remote page
      result = curl_easy_perform(curl);

      // Cleanup
      curl_easy_cleanup(curl);

      if (result == CURLE_OK)
      {
        funcResult = FETCH_CONFIGURATION_SUCCESS;
        TiXmlDocument doc;
        doc.Parse(buffer.c_str());
        if ( doc.Error() )
        {
                Logger::log(Logger::SEVERE, "Error in %s: %s\n", doc.Value(), doc.ErrorDesc() );
                funcResult = FETCH_CONFIGURATION_ERROR;
		buffer = "";
        } 
	else 
	{
                        //doc.SaveFile();
        }

      }
      else
      {
      		Logger::log(Logger::SEVERE, "Error: [%d] %s\n", result, errorBuffer);
     		funcResult = FETCH_CONFIGURATION_ERROR;
      }
    }
    return funcResult;

}

int HttpXmlFetcher::writer(char *data, size_t size, size_t nmemb,
                  std::string *buffer)
{
  // return code
  int result = 0;

  // empty buffer?
  if (buffer != NULL)
  {
    // Append the data to the buffer
    buffer->append(data, size * nmemb);

    // How much did we write?
    result = size * nmemb;
  }

  return result;
}


