#include <QtNetwork>

#include <stdlib.h>
#include "Logger.h"
#include "Kiax2DispatcherServer.h"
#include "Kiax2DispatcherThread.h"
#include "PConfiguration.h"

Kiax2DispatcherServer::Kiax2DispatcherServer(QObject* parent)
    : QObject(parent)
{
	Logger::log(Logger::INFO, "Initializing API Server..\n");
    tcpServer = new QTcpServer(this);
	Logger::log(Logger::INFO, "tcpServer initialized.\n");
    if (!tcpServer->listen(QHostAddress::Any, HTTP_API_PORT)) {
        Logger::log(Logger::SEVERE, "Unable to start the API server.");
        return;
    }

	Logger::log(Logger::INFO, "API Server running on port %d\n", tcpServer->serverPort());

    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(acceptConnection()));

}
Kiax2DispatcherServer::~Kiax2DispatcherServer()
{
	Logger::log(Logger::INFO, "~Kiax2DispatcherServer() IN\n");
	if (tcpServer->isListening())
		tcpServer->close();
	delete tcpServer;
	Logger::log(Logger::INFO, "~Kiax2DispatcherServer() OUT\n");
}

void Kiax2DispatcherServer::acceptConnection()
{
	Logger::log(Logger::INFO, "Kiax2DispatcherServer::acceptConnection() IN\n");
    QTcpSocket *clientConnection = tcpServer->nextPendingConnection();
	Logger::log(Logger::INFO, "Initializing new thread..\n");
    Kiax2DispatcherThread *thread = new Kiax2DispatcherThread(this, clientConnection, this);
	threads.push_back(thread);
	Logger::log(Logger::INFO, "New thread initialized.\n");
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
	connect(thread, SIGNAL(runOut(Kiax2DispatcherThread*)), this, SLOT(removeThread(Kiax2DispatcherThread*)));
	Logger::log(Logger::INFO, "thread->start()\n");
    thread->start();	
	Logger::log(Logger::INFO, "Kiax2DispatcherServer::acceptConnection() OUT\n");
}

void Kiax2DispatcherServer::stopServer()
{
	Logger::log(Logger::DEBUG, "stopServer() IN\n");
	int listSize = threads.size();
	Logger::log(Logger::DEBUG, "Threads in the list: %d\n", listSize);
	for (int i=0; i<listSize; i++)
	{
		Kiax2DispatcherThread* thread = threads[i];
		Logger::log(Logger::DEBUG, "Exitting thread %d\n", i);
		thread->exit(0);
	}
	Logger::log(Logger::DEBUG, "Clearing threads list..\n");

	threads.clear();
	Logger::log(Logger::DEBUG, "stopServer() OUT\n");
}

void Kiax2DispatcherServer::removeThread(Kiax2DispatcherThread* thread)
{
	Logger::log(Logger::DEBUG, "removing thread.\n");
	threads.removeOne(thread);
}

void Kiax2DispatcherServer::emitCall(QString number)
{
	Logger::log(Logger::DEBUG, "emitting call signal with number %s.\n", number.toStdString().data());
	emit call(number);
}

void Kiax2DispatcherServer::emitHangupAndCall(QString number)
{
	Logger::log(Logger::DEBUG, "emitting hangupAndCall signal with number %s.\n", number.toStdString().data());
	emit hangupAndCall(number);
}

void Kiax2DispatcherServer::emitHangupCalls(int* callsList, int size)
{
	emit hangupCalls(callsList, size);
}

void Kiax2DispatcherServer::emitCallWithAccount(QString number, Account* account)
{
	Logger::log(Logger::DEBUG, "emitting call signal with number %s.\n", number.toStdString().data());
	emit callWithAccount(number, account);
}

void Kiax2DispatcherServer::emitHangup()
{
	Logger::log(Logger::DEBUG, "emitting hangup signal\n");
	emit hangup();
}

void Kiax2DispatcherServer::emitConfigureAccount(QString username, QString password, QString host1, QString host2)
{
	Logger::log(Logger::DEBUG, "emitting configureAccount signal.\n");
	emit configureAccount(username, password, host1, host2);
}
