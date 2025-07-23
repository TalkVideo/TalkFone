#ifndef KIAX2JSONBALANCE_H
#define KIAX2JSONBALANCE_H

#include <QtWidgets/QWidget>
#include <QtNetwork>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#ifdef WIN32DEP
#include <cstdlib.>
#endif
#include <string>
#include "Logger.h"
#include <unistd.h>
#include "PConfiguration.h"
#include "tinyxml/tinyxml.h"

class Kiax2JSONBalance : public QWidget
{

  Q_OBJECT

public:
  
	Kiax2JSONBalance(QWidget* parent);
	~Kiax2JSONBalance();
	void getJSONData(QString requestUrl);
	QString getAccountBalance(QString jsondata);
	QString getAccountBalanceXML(QString xmldata);
	bool getUserValidity(QString username, QString jsondata);
	bool isAborted();
	void setUsername(QString usernameStr);
	void setPassword(QString passwordStr);
	
private slots:
    void cancelRequest();
    void httpRequestFinished(int requestId, bool error);
    void readResponseHeader(const QNetworkReply &responseHeader);
	void sslHandle( const QList<QSslError> & errors);
signals:
	void requestInitiated();
	void requestComplete(bool cancelled, QString jsondata);
	void requestAborted();
	void requestError();
	
private:
    QNetworkAccessManager *qnam;
    QNetworkRequest *qrequest;
    QNetworkReply *qreply;
    int httpGetId;
    bool httpRequestAborted;
	QString username;
	QString password;
	
};
#endif
