#ifndef KIAX2_DISPATCHER_SERVER_H
#define KIAX2_DISPATCHER_SERVER_H

#include <QTcpServer>
#include "Kiax2DispatcherThread.h"
#include "Account.h"

class Kiax2DispatcherThread;

class Kiax2DispatcherServer : public QObject
{
    Q_OBJECT

public:
    Kiax2DispatcherServer(QObject *parent = 0);
	virtual ~Kiax2DispatcherServer();
	void stopServer();
	
public slots:	
	void removeThread(Kiax2DispatcherThread* thread);
	void emitConfigureAccount(QString username, QString password, QString host1, QString host2);
	void emitCall(QString number);
	void emitCallWithAccount(QString number, Account* account);
	void emitHangupAndCall(QString number);
	void emitHangupCalls(int* callList, int size);
	void emitHangup();
	
private slots:
    void acceptConnection();

signals:
    void invalidSession(QString session);
	void changePassword(QString username, QString password);
	void configureAccount(QString username, QString password, QString host1, QString host2);
	void call(QString number);
	void callWithAccount(QString number, Account* account);
	void hangup();
	void hangupAndCall(QString number);
	void hangupCalls(int* callsList, int size);

private:
    QTcpServer *tcpServer;
	QList<Kiax2DispatcherThread*> threads;
};
#endif
