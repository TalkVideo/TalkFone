#include "Kiax2JSONUsersOnline.h"

typedef int boolean;

#include "json.h"


Kiax2JSONUsersOnline::Kiax2JSONUsersOnline(QWidget* parent) : QWidget(NULL)
{
	 qnam = new QNetworkAccessManager(this);

     connect(qnam, SIGNAL(requestFinished(int, bool)),
             this, SLOT(httpRequestFinished(int, bool)));
     connect(qnam, SIGNAL(responseHeaderReceived(const QNetworkReply &)),
             this, SLOT(readResponseHeader(const QNetworkReply &)));
	 connect(qnam, SIGNAL(sslErrors ( const QList<QSslError> & )),
             this, SLOT(sslHandle(const QList<QSslError> &)));
 }

Kiax2JSONUsersOnline::~Kiax2JSONUsersOnline()
{
	delete qrequest;
}

void Kiax2JSONUsersOnline::getJSONData(QString requestUrl)
{
     QUrl url(requestUrl);

     // QHttp::ConnectionMode mode = url.scheme().toLower() == "https" ? QHttp::ConnectionModeHttps : QHttp::ConnectionModeHttp;
     // http->setHost(url.host(), mode, url.port() == -1 ? 0 : url.port());

	 QNetworkRequest qrequest(url);

     httpRequestAborted = false;
     // TODO Fix This
     // httpGetId = qnam->get(url.toString());
     httpGetId = 1;

	 emit requestInitiated();
	 
}

void Kiax2JSONUsersOnline::sslHandle( const QList<QSslError> & errors)
{	
	/// enable for openssl
	qreply->ignoreSslErrors();
}

void Kiax2JSONUsersOnline::cancelRequest()
{	
    httpRequestAborted = true;
    qreply->abort();
	emit requestAborted();
}

void Kiax2JSONUsersOnline::httpRequestFinished(int requestId, bool error)
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

void Kiax2JSONUsersOnline::readResponseHeader(const QNetworkReply &responseHeader)
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

QString Kiax2JSONUsersOnline::getUsersOnline(QString jsondata)
{
	struct json_object *resultArray;
	int codeValue = 0;
	QString messageValue;
	QString onlineValue;
	
	resultArray = json_tokener_parse((char*)jsondata.toStdString().data());
	
	if (resultArray!=NULL)
	if (!is_error(resultArray))
	{
		struct json_object* returnObject = NULL;
		struct json_object* dataObject = NULL;
		struct json_object* codeObject = NULL;
		struct json_object* messageObject = NULL;
		struct json_object* onlineObject = NULL;
		
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
			onlineObject = json_object_object_get(dataObject, "online");
		}
		if (onlineObject!=NULL)
		{
			onlineValue = json_object_get_string(onlineObject);
		}
	}

	Logger::log(Logger::DEBUG, "codeValue = %d, messageValue = %s, onlineValue = %s\n", codeValue, messageValue.toStdString().data(),
												onlineValue.toStdString().data());

	if ((messageValue.compare("Success")==0)&&(codeValue==200))
		return onlineValue;
	else 
		return "#";
}


bool Kiax2JSONUsersOnline::isAborted()
{
	return httpRequestAborted;
}
