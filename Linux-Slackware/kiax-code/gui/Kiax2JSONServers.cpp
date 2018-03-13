#include "Kiax2JSONServers.h"

typedef int boolean;

#include "json/json.h"


Kiax2JSONServers::Kiax2JSONServers(QWidget* parent) : QWidget(NULL)
{
     http = new QHttp(this);
     connect(http, SIGNAL(requestFinished(int, bool)),
             this, SLOT(httpRequestFinished(int, bool)));
     connect(http, SIGNAL(responseHeaderReceived(const QHttpResponseHeader &)),
             this, SLOT(readResponseHeader(const QHttpResponseHeader &)));
	 connect(http, SIGNAL(sslErrors ( const QList<QSslError> & )),
             this, SLOT(sslHandle(const QList<QSslError> &)));
 }

Kiax2JSONServers::~Kiax2JSONServers()
{
	delete http;
}

void Kiax2JSONServers::getJSONData(QString requestUrl)
{
     QUrl url(requestUrl);

     QHttp::ConnectionMode mode = url.scheme().toLower() == "https" ? QHttp::ConnectionModeHttps : QHttp::ConnectionModeHttp;
     http->setHost(url.host(), mode, url.port() == -1 ? 0 : url.port());

     httpRequestAborted = false;
     httpGetId = http->get(url.toString());

	 emit requestInitiated();
	 
}

void Kiax2JSONServers::sslHandle( const QList<QSslError> & errors)
{	
	/// enable for openssl
	http->ignoreSslErrors();
}

void Kiax2JSONServers::cancelRequest()
{	
    httpRequestAborted = true;
    http->abort();
	emit requestAborted();
}

void Kiax2JSONServers::httpRequestFinished(int requestId, bool error)
{
     if (requestId != httpGetId) {
         return;
     }

	 QString data(http->readAll().constData());
	 
	 emit requestComplete(httpRequestAborted, data);

     if (error) {
		emit requestError();
	}
	 
}

void Kiax2JSONServers::readResponseHeader(const QHttpResponseHeader &responseHeader)
{
    if (responseHeader.statusCode() != 200) {
         httpRequestAborted = true;
         http->abort();
		 emit requestError();
         return;
     }
}

Kiax2Servers* Kiax2JSONServers::getServers(QString jsondata)
{
	Kiax2Servers* servers = new Kiax2Servers();
	struct json_object *resultArray;
	int codeValue = 0;
	QString messageValue;
	QString nameValue;
	QString mixpbxValue;
	QString usernameValue;
	QString server1Value;
	QString server2Value;
	QString server3Value;
	
	resultArray = json_tokener_parse((char*)jsondata.toStdString().data());
	
	if (resultArray!=NULL)
	if (!is_error(resultArray))
	{
		struct json_object* returnObject = NULL;
		struct json_object* dataObject = NULL;
		struct json_object* codeObject = NULL;
		struct json_object* messageObject = NULL;
		struct json_object* nameObject = NULL;
		struct json_object* usernameObject = NULL;
		struct json_object* mixpbxObject = NULL;
		struct json_object* server1Object = NULL;
		struct json_object* server2Object = NULL;
		struct json_object* server3Object = NULL;
		
		returnObject = json_object_object_get(resultArray, "return");
		dataObject = json_object_object_get(resultArray, "data");
		if (returnObject!=NULL)
		{
			codeObject = json_object_object_get(returnObject, "code");
			messageObject = json_object_object_get(returnObject, "message");
		}
		if (codeObject!=NULL)
			codeValue = json_object_get_int(codeObject);
		if (messageObject!=NULL)
			messageValue = json_object_get_string(messageObject);
		if (dataObject!=NULL)
		{
			nameObject = json_object_object_get(dataObject, "name");
			mixpbxObject = json_object_object_get(dataObject, "mixpbx");
			usernameObject = json_object_object_get(dataObject, "username");
			server1Object= json_object_object_get(dataObject, "server1");
			server2Object= json_object_object_get(dataObject, "server2");
			server3Object= json_object_object_get(dataObject, "server3");
		}
		if (nameObject!=NULL)
		{
			nameValue = json_object_get_string(nameObject);
			servers->setRealName(nameValue.toStdString());
		}
		if (usernameObject!=NULL)
		{
			usernameValue = json_object_get_string(usernameObject);
			servers->setUsername(usernameValue.toStdString());
		}
		if (mixpbxObject!=NULL)
		{
			mixpbxValue = json_object_get_string(mixpbxObject);
			if (mixpbxValue.compare("true")==0)
				servers->setSupernodeUser(false);
			else
				servers->setSupernodeUser(true);
		}
		if (server1Object!=NULL)
		{		
			server1Value = json_object_get_string(server1Object);
		}
		servers->addServer(server1Value.toStdString());
		
		if (server2Object!=NULL)
		{
			server2Value = json_object_get_string(server2Object);
		}
		servers->addServer(server2Value.toStdString());
		
		if (server3Object!=NULL)
		{
			server3Value = json_object_get_string(server3Object);
		}
		servers->addServer(server3Value.toStdString());		

	}

	Logger::log(Logger::DEBUG, "codeValue = %d, messageValue = %s, nameValue = %s, usernameValue=%s, mixpbxValue=%s, server1Value=%s, server2Value=%s, server3Value=%s\n", codeValue, messageValue.toStdString().data(),
												nameValue.toStdString().data(), usernameValue.toStdString().data(), mixpbxValue.toStdString().data(), server1Value.toStdString().data(), server2Value.toStdString().data(), server3Value.toStdString().data());

	if ((messageValue.compare("Success")==0)&&(codeValue==200))
	{
		return servers;
	}
	else
	{ 
		delete servers;
		servers = NULL;
	}

	return servers;
}


bool Kiax2JSONServers::isAborted()
{
	return httpRequestAborted;
}

