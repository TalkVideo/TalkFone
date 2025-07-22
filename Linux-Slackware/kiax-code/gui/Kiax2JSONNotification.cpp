#include "Kiax2JSONNotification.h"

typedef int boolean;

#include "json.h"


Kiax2JSONNotification::Kiax2JSONNotification(QWidget* parent) : QWidget(NULL)
{
     http = new QHttp(this);
     connect(http, SIGNAL(requestFinished(int, bool)),
             this, SLOT(httpRequestFinished(int, bool)));
     connect(http, SIGNAL(responseHeaderReceived(const QHttpResponseHeader &)),
             this, SLOT(readResponseHeader(const QHttpResponseHeader &)));
	 connect(http, SIGNAL(sslErrors ( const QList<QSslError> & )),
             this, SLOT(sslHandle(const QList<QSslError> &)));
 }

Kiax2JSONNotification::~Kiax2JSONNotification()
{
	delete http;
}

void Kiax2JSONNotification::getJSONData(QString requestUrl)
{
     QUrl url(requestUrl);
     // add useranme and password to the query only if notification is not anonymous (for branders only)
     // kiax does not need user information, except version
     if ((!JSON_NOTIFICATION_ANONYMOUS)&&(username!="")&&(password!=""))
     {     	url.addQueryItem(JSON_USERNAME_PARAMETER, username);
     	url.addQueryItem(JSON_PASSWORD_PARAMETER, password);
     }
	if (version!="")
     	url.addQueryItem(JSON_SOFTPHONEVERSION_PARAMETER, version);
     Logger::log(Logger::DEBUG, "json notification request url is %s\n", url.toString().toStdString().data());
     QHttp::ConnectionMode mode = url.scheme().toLower() == "https" ? QHttp::ConnectionModeHttps : QHttp::ConnectionModeHttp;
     http->setHost(url.host(), mode, url.port() == -1 ? 0 : url.port());

     httpRequestAborted = false;
     httpGetId = http->get(url.toString());

	 emit requestInitiated();
	 
}

void Kiax2JSONNotification::sslHandle( const QList<QSslError> & errors)
{	
	/// enable for openssl
	http->ignoreSslErrors();
}

void Kiax2JSONNotification::cancelRequest()
{	
    httpRequestAborted = true;
    http->abort();
	emit requestAborted();
}

void Kiax2JSONNotification::httpRequestFinished(int requestId, bool error)
{
     if (requestId != httpGetId) {
         return;
     }

	 QString jsondata(http->readAll().constData());
	 
	if (!httpRequestAborted)
	{
		//emit requestComplete(httpRequestAborted, data);
		struct json_object *resultArray;
		int codeValue = 0;
		QString messageValue;
		QString notificationMessageValue;
		QString notificationUrlValue;
		
		resultArray = json_tokener_parse((char*)jsondata.toStdString().data());
		
		if (resultArray!=NULL)
		if (!is_error(resultArray))
		{
			struct json_object* returnObject = NULL;
			struct json_object* dataObject = NULL;
			struct json_object* codeObject = NULL;
			struct json_object* messageObject = NULL;
			struct json_object* notificationMessageObject = NULL;		
			struct json_object* notificationUrlObject = NULL;

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
				notificationMessageObject = json_object_object_get(dataObject, "message");
				notificationUrlObject = json_object_object_get(dataObject, "messageurl");
	
			}
			if (notificationMessageObject!=NULL)
			{
				notificationMessageValue = json_object_get_string(notificationMessageObject);
			}
			if (notificationUrlObject!=NULL)
			{
				notificationUrlValue = json_object_get_string(notificationUrlObject);
			}
		}

		Logger::log(Logger::DEBUG, "codeValue = %d, messageValue = %s, notificationMessageValue = %s, notificationUrlValue\n", codeValue, messageValue.toStdString().data(), notificationMessageValue.toStdString().data(), notificationUrlValue.toStdString().data());

		if ((messageValue.compare("Success")==0)&&(codeValue==200))
		{
			if (notificationMessageValue!="")
				emit notificationAvailable(notificationMessageValue, notificationUrlValue);
		}
	}	 

     	if (error) {
		emit requestError();
	 }
	 
}

void Kiax2JSONNotification::readResponseHeader(const QHttpResponseHeader &responseHeader)
{
    if (responseHeader.statusCode() != 200) {
         httpRequestAborted = true;
         http->abort();
		 emit requestError();
         return;
     }
}


bool Kiax2JSONNotification::isAborted()
{
	return httpRequestAborted;
}
void Kiax2JSONNotification::setUsername(QString usernameStr)
{
	username = usernameStr;
}

void Kiax2JSONNotification::setPassword(QString passwordStr)
{
	password = passwordStr;
}
void Kiax2JSONNotification::setVersion(QString versionStr)
{
	version = versionStr;
}
