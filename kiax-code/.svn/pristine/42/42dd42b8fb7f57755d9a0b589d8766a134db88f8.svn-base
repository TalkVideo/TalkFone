
#ifndef KIAX2_DISPATCHER_THREAD_H
#define KIAX2_DISPATCHER_THREAD_H

#include <QThread>
#include <QTcpSocket>
#include "Kiax2DispatcherServer.h"
#include "Kiax2JSONSupernode.h"
#include <QList>

class Kiax2DispatcherServer;
class Kiax2DispatcherHandler;

class Kiax2DispatcherThread : public QThread
{
    Q_OBJECT

public:
    Kiax2DispatcherThread(Kiax2DispatcherServer* dispatcherServer, QTcpSocket* clientConnection, QObject *parent);
    ~Kiax2DispatcherThread();
    void run();
	int* getNumbersList(int* numbersLength);
public slots:
	virtual void readMessage();
	virtual void socketDisconnected();
	virtual void requestCompleteSlot();
	virtual void connectionRefusedSlot(QUrl requestUrl);
	void requestCompleteSupernode(bool errorFlag, QString data);
	void requestCompleteLocal(bool errorFlag, QString data);
		
signals:
    void error(QTcpSocket::SocketError socketError);
	void runOut(Kiax2DispatcherThread* thread);
	
private:
	bool retried;
    QTcpSocket* connection;
	Kiax2DispatcherServer* server;
	Kiax2JSONSupernode* jsonSupernode;
	Kiax2JSONSupernode* jsonLocal;

};
#endif
