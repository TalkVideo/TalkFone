#include "Kiax2Application.h"
#include "Logger.h"
#ifdef MACXDEP
#include <Carbon/Carbon.h>
#endif
#if defined(WIN32DEP) || defined(LINUXDEP)
#include <unistd.h>
#endif
#include <QLocalSocket>

Kiax2Application::Kiax2Application(int &argc, char *argv[], const QString uniqueKey) : QApplication(argc, argv), _uniqueKey(uniqueKey)
{
	Logger::init(Logger::DEBUG);

#ifdef MACXDEP
	chdir("Kiax.app/Contents/MacOS");
#endif
#if defined(WIN32DEP) || defined(LINUXDEP)
	Logger::log(Logger::DEBUG,"Application dir path: %s\n", applicationDirPath().toStdString().data());
	chdir(applicationDirPath().toStdString().data());
#endif
	sharedMemory.setKey(_uniqueKey);
	if (sharedMemory.attach())
		_isRunning = true;
	else
	{
		_isRunning = false;
		// create shared memory.
		if (!sharedMemory.create(1))
		{
			qDebug("Unable to create single instance.");
			return;
		}
		// create local server and listen to incomming messages from other instances.
		localServer = new QLocalServer(this);
		connect(localServer, SIGNAL(newConnection()), this, SLOT(receiveMessage()));
		localServer->listen(_uniqueKey);
	}

}

Kiax2Application::~Kiax2Application()
{
}

bool Kiax2Application::event(QEvent* event)
{
	//Logger::log(Logger::DEBUG, "Event %d\n", event->type());
        switch (event->type()) {
			case QEvent::ApplicationActivated:
			{
				emit applicationActivated();
			}
			default:
				return QApplication::event(event);
		}	
	return true;
}

#if defined(MACXDEP)
bool Kiax2Application::macEventFilter(EventHandlerCallRef caller, EventRef event)
{
	// UInt32 event_kind = GetEventKind(event);
	UInt32 event_kind = 1;
	// UInt32 event_class = GetEventClass(event);
	switch (event_kind)
	{
		case 1:
			emit applicationActivated();
			return false;
			break;
		default:
			return false;
		
	}
	return false;
}
#endif

void Kiax2Application::receiveMessage()
{
	QLocalSocket *localSocket = localServer->nextPendingConnection();
	if (!localSocket->waitForReadyRead(timeout))
	{
		qDebug(localSocket->errorString().toLatin1());
		return;
	}
	QByteArray byteArray = localSocket->readAll();
	QString message = QString::fromUtf8(byteArray.constData());
	emit messageAvailable(message);
	localSocket->disconnectFromServer();
}

// public functions.

bool Kiax2Application::isRunning()
{
	return _isRunning;
}

bool Kiax2Application::sendMessage(const QString &message)
{
	if (!_isRunning)
		return false;
	QLocalSocket localSocket(this);
	localSocket.connectToServer(_uniqueKey, QIODevice::WriteOnly);
	if (!localSocket.waitForConnected(timeout))
	{
		qDebug(localSocket.errorString().toLatin1());
		return false;
	}
	localSocket.write(message.toUtf8());
	if (!localSocket.waitForBytesWritten(timeout))
	{
		qDebug(localSocket.errorString().toLatin1());
		return false;
	}
	localSocket.disconnectFromServer();
	return true;
}
