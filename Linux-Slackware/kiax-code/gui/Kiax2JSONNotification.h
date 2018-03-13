#ifndef KIAX2JSONNOTIFICATION_H
#define KIAX2JSONNOTIFICATION_H

#include <QWidget>
#include <QtNetwork>
#include <QHttp>
#include <QHttpResponseHeader>
#ifdef WIN32DEP
#include <cstdlib.>
#endif
#include <string>
#include "Logger.h"
#include <unistd.h>
#include "PConfiguration.h"


class Kiax2JSONNotification : public QWidget
{

  Q_OBJECT

public:
  
	Kiax2JSONNotification(QWidget* parent);
	~Kiax2JSONNotification();
	void getJSONData(QString requestUrl);
	bool isAborted();
	void setUsername(QString username);
	void setPassword(QString password);
	void setVersion(QString version);
	
private slots:
    void cancelRequest();
    void httpRequestFinished(int requestId, bool error);
    void readResponseHeader(const QHttpResponseHeader &responseHeader);
	void sslHandle( const QList<QSslError> & errors);
signals:
	void requestInitiated();
	void requestComplete(bool cancelled, QString jsondata);
	void notificationAvailable(QString message, QString messageUrl);
	void requestAborted();
	void requestError();
	
private:
    QHttp *http;
    int httpGetId;
    bool httpRequestAborted;
    QString username;
    QString password;
    QString version;

};
#endif
