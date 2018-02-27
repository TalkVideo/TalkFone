#ifndef KIAX2JSONUSERSONLINE_H
#define KIAX2JSONUSERSONLINE_H

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


class Kiax2JSONUsersOnline : public QWidget
{

  Q_OBJECT

public:
  
	Kiax2JSONUsersOnline(QWidget* parent);
	~Kiax2JSONUsersOnline();
	void getJSONData(QString requestUrl);
	QString getUsersOnline(QString jsondata);
	bool isAborted();
	
private slots:
    void cancelRequest();
    void httpRequestFinished(int requestId, bool error);
    void readResponseHeader(const QHttpResponseHeader &responseHeader);
	void sslHandle( const QList<QSslError> & errors);
signals:
	void requestInitiated();
	void requestComplete(bool cancelled, QString jsondata);
	void requestAborted();
	void requestError();
	
private:
    QHttp *http;
    int httpGetId;
    bool httpRequestAborted;

};
#endif
