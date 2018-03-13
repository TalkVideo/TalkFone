#ifndef KIAX2JSONSUPERNODE_H
#define KIAX2JSONSUPERNODE_H

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

class Kiax2JSONSupernode : public QWidget
{

  Q_OBJECT

public:
  
	Kiax2JSONSupernode(QWidget* parent);
	~Kiax2JSONSupernode();
	void makeApiCall(QString requestUrl);
	QString getCallResult(QString jsondata);
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
