#include "Kiax2JSONSupernode.h"

typedef int boolean;

#include "json/json.h"


Kiax2JSONSupernode::Kiax2JSONSupernode(QWidget* parent) : QWidget(NULL)
{
	 qnam = new QNetworkAccessManager(this);

     connect(qnam, SIGNAL(requestFinished(int, bool)),
             this, SLOT(httpRequestFinished(int, bool)));
     connect(qnam, SIGNAL(responseHeaderReceived(const QNetworkReply &)),
             this, SLOT(readResponseHeader(const QNetworkReply &)));
	 connect(qnam, SIGNAL(sslErrors ( const QList<QSslError> & )),
             this, SLOT(sslHandle(const QList<QSslError> &)));
 }

Kiax2JSONSupernode::~Kiax2JSONSupernode()
{
	delete qnam;
}

void Kiax2JSONSupernode::makeApiCall(QString requestUrl)
{
     QUrl url(requestUrl);

     // QHttp::ConnectionMode mode = url.scheme().toLower() == "https" ? QHttp::ConnectionModeHttps : QHttp::ConnectionModeHttp;
     // http->setHost(url.host(), mode, url.port() == -1 ? 0 : url.port());
     
	 QNetworkRequest qrequest(url);

     httpRequestAborted = false;
	 //TODO Fix This
     // httpGetId = http->get(url.toString());
     httpGetId = 1;

	 emit requestInitiated();
	 
}

void Kiax2JSONSupernode::sslHandle( const QList<QSslError> & errors)
{	
	/// enable for openssl
	qreply->ignoreSslErrors();
}

void Kiax2JSONSupernode::cancelRequest()
{	
    httpRequestAborted = true;
    qreply->abort();
	emit requestAborted();
}

void Kiax2JSONSupernode::httpRequestFinished(int requestId, bool error)
{
     if (requestId != httpGetId) {
         return;
     }

	 QString data(qreply->readAll().constData());
	 
	 emit requestComplete(httpRequestAborted, data);

     if (error) {
		emit requestError();
	}
	 
}

void Kiax2JSONSupernode::readResponseHeader(const QNetworkReply &responseHeader)
{
	QNetworkReply::NetworkError error = qreply->error();

	if (error != QNetworkReply::NoError) {
    // if (responseHeader.statusCode() != 200) {
         httpRequestAborted = true;
         qreply->abort();
		 emit requestError();
         return;
     }
}

QString Kiax2JSONSupernode::getCallResult(QString jsondata)
{
	QString result;
	struct json_object *resultArray;
	int codeValue = 0;
	QString messageValue;
	QString nameValue;
	
	resultArray = json_tokener_parse((char*)jsondata.toStdString().data());
	
	if (resultArray!=NULL)
	if (!is_error(resultArray))
	{
		struct json_object* returnObject = NULL;
		struct json_object* dataObject = NULL;
		struct json_object* codeObject = NULL;
		struct json_object* messageObject = NULL;
		struct json_object* nameObject = NULL;
		
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
		}
		if (nameObject!=NULL)
		{
			nameValue = json_object_get_string(nameObject);
		}
	}

	if ((messageValue.compare("Success")==0)&&(codeValue==200))
	{
		return "success";
	}
	else
	{ 
		return "messageValue";
	}

}


bool Kiax2JSONSupernode::isAborted()
{
	return httpRequestAborted;
}

