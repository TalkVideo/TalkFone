#ifndef KIAX2JSONSERVERS_H
#define KIAX2JSONSERVERS_H

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
#include "Kiax2Servers.h"

class Kiax2JSONServers : public QWidget
{

  Q_OBJECT

public:
  
	Kiax2JSONServers(QWidget* parent);
	~Kiax2JSONServers();
	void getJSONData(QString requestUrl);
	Kiax2Servers* getServers(QString jsondata);
	bool isAborted();
	
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

};
#endif
