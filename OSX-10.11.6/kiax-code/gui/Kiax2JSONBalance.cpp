#include "Kiax2JSONBalance.h"


typedef int boolean;

#include "json.h"


Kiax2JSONBalance::Kiax2JSONBalance(QWidget* parent) : QWidget(NULL)
{
     http = new QHttp(this);
     connect(http, SIGNAL(requestFinished(int, bool)),
             this, SLOT(httpRequestFinished(int, bool)));
     connect(http, SIGNAL(responseHeaderReceived(const QHttpResponseHeader &)),
             this, SLOT(readResponseHeader(const QHttpResponseHeader &)));
	 connect(http, SIGNAL(sslErrors ( const QList<QSslError> & )),
             this, SLOT(sslHandle(const QList<QSslError> &)));
 }

Kiax2JSONBalance::~Kiax2JSONBalance()
{
	delete http;
}

void Kiax2JSONBalance::getJSONData(QString requestUrl)
{
     QUrl url(requestUrl);
	 url.addQueryItem("username", username);
	 url.addQueryItem("password", password);

     QHttp::ConnectionMode mode = url.scheme().toLower() == "https" ? QHttp::ConnectionModeHttps : QHttp::ConnectionModeHttp;
     http->setHost(url.host(), mode, url.port() == -1 ? 0 : url.port());

     if (!url.userName().isEmpty())
         http->setUser(url.userName(), url.password());

     httpRequestAborted = false;
     httpGetId = http->get(url.toString());

	 emit requestInitiated();
	 
}

void Kiax2JSONBalance::sslHandle( const QList<QSslError> & errors)
{	
	/// enable for openssl
	http->ignoreSslErrors();
}

void Kiax2JSONBalance::cancelRequest()
{	
    httpRequestAborted = true;
    http->abort();
	emit requestAborted();
}

void Kiax2JSONBalance::httpRequestFinished(int requestId, bool error)
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

void Kiax2JSONBalance::readResponseHeader(const QHttpResponseHeader &responseHeader)
{
    if (responseHeader.statusCode() != 200) {
         httpRequestAborted = true;
         http->abort();
		 emit requestError();
         return;
     }
}

/*
QString Kiax2JSONBalance::getAccountBalanceXML(QString xmldata)
{
	TiXmlDocument doc;
	doc.Parse(xmldata.toStdString().c_str());
	if (doc.Error())
	{
		return "";
	}
        TiXmlNode* node = 0;
        TiXmlElement* balanceElement = 0;
        node = doc.FirstChild("balance");
        balanceElement = node->ToElement();
	TiXmlText* balanceValueElement  = node->FirstChild()->ToText();	
	const char* valueText = balanceValueElement->Value();
	if (valueText)
		return QString(valueText);
	return "";
}
*/

QString Kiax2JSONBalance::getAccountBalance(QString jsondata)
{
	struct json_object *resultArray;
	int codeValue = 0;
	QString messageValue;
	QString idValue;
	QString creditValue;
	QString currencySignValue;
	QString currencyValue;
	QString invoiceValue;
	
	resultArray = json_tokener_parse((char*)jsondata.toStdString().data());
	
	if (resultArray!=NULL)
	if (!is_error(resultArray))
	{
		struct json_object* returnObject = NULL;
		struct json_object* dataObject = NULL;
		struct json_object* codeObject = NULL;
		struct json_object* messageObject = NULL;
		struct json_object* idObject = NULL;
		struct json_object* creditObject = NULL;
		struct json_object* invoiceObject = NULL;
		struct json_object* currencySignObject = NULL;
		struct json_object* currencyObject = NULL;
		
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
			idObject = json_object_object_get(dataObject, "id");
			creditObject = json_object_object_get(dataObject, "credit");
			currencySignObject = json_object_object_get(dataObject, "currencysign"); // UTF Symbol
			currencyObject = json_object_object_get(dataObject, "currency"); // UTF Symbol
			invoiceObject = json_object_object_get(dataObject, "invoice");
		}
		if (idObject!=NULL)
		{
			idValue = json_object_get_string(idObject);
		}
		if (creditObject!=NULL)
		{
			creditValue = json_object_get_string(creditObject);
		}
		if (currencySignObject!=NULL)
		{
			currencySignValue = QString::fromUtf8(json_object_get_string(currencySignObject));
		}
		if (currencyObject!=NULL)
		{
			currencyValue = json_object_get_string(currencyObject);
		}		
		if (invoiceObject!=NULL)
		{
			invoiceValue = json_object_get_string(invoiceObject);
		}
	}

	/*Logger::log(Logger::DEBUG, "codeValue = %d, messageValue = %s, idValue = %s, creditValue = %s\n", codeValue, messageValue.toStdString().data(),
												idValue.toStdString().data(), creditValue.toStdString().data());
*/

	QString eurPart;
	QString centPart;

	if (creditValue.indexOf('.')>0)
	{
		creditValue = creditValue.remove(creditValue.indexOf('.'), creditValue.length()-1);
	}
	QString balance =  creditValue;
	
	
	
	if (creditValue.length()>2)
	{
		eurPart = balance.left(balance.length()-2);
		centPart = balance.right(2);
	}
	else
	{
		eurPart = "0";
		centPart = QString("0") + balance;
	}
	QString balanceEur = eurPart + "," + centPart + " " + currencySignValue;

	if ((messageValue.compare("Success")==0)&&(codeValue==200))
	{
		if (invoiceValue=="1")
			return "";
		else
			return balanceEur;
	}
	else 
		return "";
}

bool Kiax2JSONBalance::getUserValidity(QString user, QString jsondata)
{
	struct json_object *resultArray;
	int codeValue;
	QString messageValue;
	QString idValue;
	
	resultArray = json_tokener_parse((char*)jsondata.toStdString().data());

	if (resultArray!=NULL)
	{
		struct json_object* returnObject = NULL;
		struct json_object* dataObject = NULL;
		struct json_object* codeObject = NULL;
		struct json_object* messageObject = NULL;
		struct json_object* idObject = NULL;
		//struct json_object* creditObject = NULL;
		//struct json_object* invoiceObject = NULL;
		
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
			idObject = json_object_object_get(dataObject, "id");
		}
		if (idObject!=NULL)
		{
			idValue = json_object_get_string(idObject);
		}
	}

	Logger::log(Logger::DEBUG, "codeValue = %d, messageValue = %s, idValue = %s\n", codeValue, messageValue.toStdString().data(),
												idValue.toStdString().data());

	if ((messageValue.compare("Success")==0)&&(codeValue==200))
		return true;
	else 
		return false;
 
}

bool Kiax2JSONBalance::isAborted()
{
	return httpRequestAborted;
}

void Kiax2JSONBalance::setUsername(QString usernameStr)
{
	username = usernameStr;
}

void Kiax2JSONBalance::setPassword(QString passwordStr)
{
	password = passwordStr;
}
