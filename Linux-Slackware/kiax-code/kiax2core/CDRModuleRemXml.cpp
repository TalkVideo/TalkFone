#include "CDRModuleRemXml.h"
#include "HttpXmlFetcher.h"
#include "Core.h"

CDRModule* CDRModule::singleton;

AbstractModule* CDRModuleRemXml::instance()
{
	if (singleton==NULL)
	{
		singleton = new CDRModuleRemXml();
	}
	return singleton;
}

CDRModuleRemXml::CDRModuleRemXml()
{
	moduleName = "CDRModuleRemXml";
	moduleType = "CDRModule";
}

void CDRModuleRemXml::init()
{
	Core* core = Core::instance();
	storageService = core->storageService;
	Logger::log(Logger::INFO, "Module %s initialized.\n", moduleName.data());
}

void CDRModuleRemXml::dispose()
{
    Logger::log(Logger::INFO, "Module %s disposed.\n", moduleName.data());
}

void CDRModuleRemXml::logCDR(CDR* cdr)
{
}

std::vector<CDR*> CDRModuleRemXml::getCDRs(std::string filter)
{
	Logger::log(Logger::DEBUG, "getCDRs(%s) IN\n", filter.data());
	HttpXmlFetcher fetcher;
	std::vector<CDR*> cdrs_;
	string fetchurl (HTTP_CDR_FETCH_URL);
	string username = Core::configuration["username"];
	fetchurl = fetchurl + "?username=" + username;
	Logger::log(Logger::DEBUG, "Fetch url is:%s\n", fetchurl.data());
	int ferror = fetcher.fetchData(fetchurl);
	if (ferror!= FETCH_CONFIGURATION_ERROR)
	{
		string data = fetcher.buffer;
		fetcher.buffer = "";
		Logger::log(Logger::DEBUG, "got XML data:\n%s\n", data.data());
		TiXmlDocument doc;
		doc.Parse(data.c_str());
 
		// parse XMl and instantiate CDRs
        	TiXmlNode* node = NULL;
       		TiXmlElement* mainElement = NULL;
        	node = doc.FirstChild("cdr");
		if (node!=NULL) 
		{
		mainElement = node->ToElement();
        	for (TiXmlNode* _node = mainElement->FirstChild(
                                "entry"); 
                        _node;
                        _node = _node->NextSibling("entry"))
        	{
				CDR* newcdr = new CDR();
        		/* Entry Iteration */
                	if (_node!=NULL) {
                        	TiXmlElement* entryElement = _node->ToElement();
				// date //
				TiXmlNode* dateNode = entryElement->FirstChild("date");
				TiXmlText* dateTextElement  = NULL;
				char* dateValue = NULL;
				if (dateNode!=NULL) 
				{
					dateTextElement = dateNode->FirstChild()->ToText();
					dateValue = (char*)dateTextElement->Value();
					newcdr->cdrTime = string(dateValue);
					Logger::log(Logger::DEBUG, "cdr date: %s\n", dateValue);
				}
				// src  //
				TiXmlNode* srcNode = entryElement->FirstChild("src");
				TiXmlText* srcTextElement  = NULL;
				char* srcValue = NULL;
				if (srcNode!=NULL) 
				{
					srcTextElement = srcNode->FirstChild()->ToText();
					srcValue = (char*)srcTextElement->Value();
					newcdr->srcExt = string(srcValue);
					Logger::log(Logger::DEBUG, "cdr src: %s\n", srcValue);
				}
				// dst  //
				TiXmlNode* dstNode = entryElement->FirstChild("dst");
				TiXmlText* dstTextElement  = NULL;
				char* dstValue = NULL;
				if (dstNode!=NULL) 
				{
					dstTextElement = dstNode->FirstChild()->ToText();
					dstValue = (char*)dstTextElement->Value();
					newcdr->dstExt = string(dstValue);
					Logger::log(Logger::DEBUG, "cdr dst: %s\n", dstValue);
				}
				// duration  //
				TiXmlNode* durationNode = entryElement->FirstChild("duration");
				TiXmlText* durationTextElement  = NULL;
				char* durationValue = NULL;
				if (durationNode!=NULL) 
				{
					durationTextElement = durationNode->FirstChild()->ToText();
					durationValue = (char*)durationTextElement->Value();
					newcdr->duration = durationValue;
					Logger::log(Logger::DEBUG, "cdr duration: %s\n", durationValue);
				}
				// dstdesc  //
				TiXmlNode* dstdescNode = entryElement->FirstChild("dstdesc");
				TiXmlText* dstdescTextElement  = NULL;
				char* dstdescValue = NULL;
				if (dstdescNode!=NULL) 
				{
					if (dstdescNode->FirstChild()!=NULL)
						dstdescTextElement = dstdescNode->FirstChild()->ToText();
					else
					{
						newcdr->dstName = dstValue;
						Logger::log(Logger::DEBUG, "cdr dstdesc: %s\n", dstdescValue);
					}
					if (dstdescTextElement!=NULL)
					{
						dstdescValue = (char*)dstdescTextElement->Value();
						newcdr->dstName = dstValue;
						newcdr->dstName.append(", ");						
						newcdr->dstName.append(dstdescValue);
						Logger::log(Logger::DEBUG, "cdr dstdesc: %s\n", dstdescValue);
					}
				}
				// cost  //
				TiXmlNode* costNode = entryElement->FirstChild("cost");
				TiXmlText* costTextElement  = NULL;
				char* costValue = NULL;
				if (costNode!=NULL) 
				{
					costTextElement = costNode->FirstChild()->ToText();
					costValue = (char*)costTextElement->Value();
					newcdr->cdrInfo = costValue;
					Logger::log(Logger::DEBUG, "cdr cost: %s\n", costValue);
				}
				if (newcdr->srcExt!=username)
					newcdr->direction = CALL_DIRECTION_INCOMING;
				else
					newcdr->direction = CALL_DIRECTION_OUTGOING;
				newcdr->callState = CALL_STATE_ANSWERED	; // current XML schema does not define call state. use lame one - answered
 				cdrs_.push_back(newcdr);
			}
		}
 
		
		}
		else {
			Logger::log(Logger::SEVERE, "Error fetching data.\n");
		}
	}
	
	Logger::log(Logger::DEBUG, "getCDRs(%s) OUT\n", filter.data());
	return cdrs_;
}

std::vector<CDR*> CDRModuleRemXml::getCDRs()
{
	Logger::log(Logger::DEBUG, "getCDRs() IN\n");
	std::vector<CDR*> cdrs_ = getCDRs("");
	Logger::log(Logger::DEBUG, "getCDRs() OUT\n");
	return cdrs_;
}

