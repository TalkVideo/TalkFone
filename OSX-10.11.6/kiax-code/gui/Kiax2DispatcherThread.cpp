#include "Kiax2DispatcherThread.h"

#include <QtNetwork>
#include "Logger.h"
#include <QApplication>
#include "Kiax2MainWindow.h"
#include "Kiax2Servers.h"
#include <vector>

Kiax2Servers* Kiax2MainWindow::servers;
QString Kiax2MainWindow::username;
QString Kiax2MainWindow::password;
int Kiax2MainWindow::routing;
Kiax2MainWindow* Kiax2MainWindow::instance;

Kiax2DispatcherThread::Kiax2DispatcherThread(Kiax2DispatcherServer* dispatcherServer, QTcpSocket* clientConnection, QObject *parent)
    : QThread(parent)
{
	server = dispatcherServer;
	connection = clientConnection;
	retried = false;
	
	connect(connection, SIGNAL(readyRead()), this, SLOT(readMessage()));
	connect(connection, SIGNAL(disconnected()), this, SLOT(socketDisconnected()));
	Logger::log(Logger::DEBUG, "New TCP Connection THREAD - ID = %d.\n", QThread::currentThreadId());
	jsonSupernode = new Kiax2JSONSupernode(NULL);
	jsonLocal = new Kiax2JSONSupernode(NULL);	 
	connect(jsonSupernode, SIGNAL(requestComplete(bool, QString)),
			this, SLOT(requestCompleteSupernode(bool, QString)));
	connect(jsonLocal, SIGNAL(requestComplete(bool, QString)),
			this, SLOT(requestCompleteLocal(bool, QString)));
	
}

Kiax2DispatcherThread::~Kiax2DispatcherThread()
{
	delete connection;
	connection = NULL;
	delete jsonSupernode;
	jsonSupernode = NULL;
	delete jsonLocal;
	jsonLocal = NULL;
}

void Kiax2DispatcherThread::socketDisconnected()
{
	Logger::log(Logger::DEBUG, "Disconnected.\n");
	exit(0);
}

void Kiax2DispatcherThread::run()
{
	Logger::log(Logger::DEBUG, "QThread::run() IN\n");
	exec();
	Logger::log(Logger::DEBUG, "QThread::run() OUT\n");
}

int * Kiax2DispatcherThread::getNumbersList(int* numbersLength)
{
	*numbersLength = 0;
	map<int, Kiax2CallAppearance*> callMap = Kiax2MainWindow::instance->callMap;
	
	map<int, Kiax2CallAppearance*>::const_iterator itr;
	Logger::log(Logger::DEBUG, "callMap has size: %d\n", callMap.size());
	int * numbers = (int*)malloc(callMap.size() * sizeof(int));	
	int i = 0;
	for(itr = callMap.begin(); itr != callMap.end(); itr++){
		int activeCallNumber = itr->first;
		Logger::log(Logger::DEBUG, "getNumbersList:: checking activeCallNumber: %d\n", activeCallNumber);
		if (itr->second!=NULL)
		{
			numbers[i] = activeCallNumber;
			Logger::log(Logger::DEBUG, "getNumbersList:: activeCallNumber=%d not NULL: %d. i=%d\n", activeCallNumber, numbers[i], i);
			*numbersLength = i+1;
			i++;
		} else {
			Logger::log(Logger::DEBUG, "getNumbersList:: activeCallNumber=%d is NULL\n", activeCallNumber);
		}
	}
	Logger::log(Logger::DEBUG, "getNumbersList:: numbersLength: %d\n", *numbersLength);
	return numbers;

}

void Kiax2DispatcherThread::readMessage()
{
	retried = false;
	QUrl url;
	Logger::log(Logger::DEBUG, "readMessage() IN\n");
	char buf[1024];
     qint64 lineLength = connection->readLine(buf, sizeof(buf));
	 connection->readAll();
	 QString path;
	 QList< QPair<QString, QString> > parameters;

     if (lineLength != -1) {
		QString messageStr(buf);
		//strip /n
		int crStart = messageStr.indexOf("HTTP");
		messageStr.remove(crStart-1, sizeof(buf));
		Logger::log(Logger::INFO, "GET without HTTP 1.1 ending %s\n", messageStr.toStdString().data());
		QUrl qurl(messageStr);
		path = qurl.path();
		// strip GET and /
		path.remove(0, 4);
		qurl.setPath(path);
		parameters = qurl.queryItems();
		url = qurl;
     }

	QString returnHtml = "</div>";
	QString reply200OK = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " + QString::number(returnHtml.length()) + "\n\n" + returnHtml;
	
	connection->write(reply200OK.toStdString().data(), reply200OK.length());
	connection->waitForBytesWritten();
	
	Logger::log(Logger::INFO,"retrying request:%s\n", url.toString(QUrl::RemoveScheme).toStdString().data());
	QString username;
	QString password;
	QString redirectServer;

	int numbersSize = 0;
	int* numbers = getNumbersList(&numbersSize);
	
	if ((Kiax2MainWindow::servers)&&(Kiax2MainWindow::routing==AGENT_ROUTING_SERVER))
	{
		Logger::log(Logger::DEBUG,"rotuing = AGENT_ROUTING_SERVER\n");
		Kiax2Servers* servers = Kiax2MainWindow::servers;
		redirectServer = QString::fromStdString(servers->getServers()[0]);
		username = Kiax2MainWindow::username;
		password = Kiax2MainWindow::password;



		if (servers->isSupernodeUser())
		{
			Logger::log(Logger::DEBUG,"supernode user\n");
			QString requestUrl = "https://" + redirectServer ;
			if ((path=="/call")||(path=="/hangupandcall"))
			{
				Logger::log(Logger::DEBUG,"path = /call\n");
				QString number = url.queryItemValue("number");
				requestUrl = requestUrl + "/calls/call" + "/" + username + "/" + number;
				if(username!="")
					requestUrl = requestUrl + "&username=" + username;
				else
					requestUrl = requestUrl + "&username=nouser";
				if (password!="")
					requestUrl = requestUrl + "&password=" + password;
				Logger::log(Logger::INFO, "composed url (supernode) %s\n", requestUrl.toStdString().data());
				if ((number!="")&&(number!="null"))		
					jsonSupernode->makeApiCall(requestUrl);
				if (path=="/hangupandcall")
				{
					server->emitHangupCalls(numbers, numbersSize);
				}
			}
		}
		else
		{
			Logger::log(Logger::DEBUG,"mixpbx user\n");
			if ((path=="/call")||(path=="/hangupandcall"))
			{
				Logger::log(Logger::DEBUG,"path = /call\n");
				QString number = url.queryItemValue("number");
				QString requestUrl = "https://" + redirectServer + "/click2dial.php?number=" + number;
				if(username!="") 
					requestUrl = requestUrl + "&username=" + username;
				if (password!="")
					requestUrl = requestUrl + "&password=" + password;
				Logger::log(Logger::INFO, "composed url (local) %s\n", requestUrl.toStdString().data());			
				if ((number!="")&&(number!="null"))
					jsonLocal->makeApiCall(requestUrl);
				if (path=="/hangupandcall")
				{
					server->emitHangupCalls(numbers, numbersSize);

				}
			}
		}
	}
	else if (Kiax2MainWindow::routing==AGENT_ROUTING_SOFTPHONE)
	{
			Logger::log(Logger::DEBUG,"rotuing = AGENT_ROUTING_SOFTPHONE\n");
			if ((path=="/call")||(path=="/hangupandcall"))
			{

				Logger::log(Logger::DEBUG,"path = call\n");
				if (Kiax2MainWindow::servers)
				{
					if (!Kiax2MainWindow::servers->isSupernodeUser())
					{
						Logger::log(Logger::DEBUG,"mixpbx user\n");
						std::string iaxServer1 = Kiax2MainWindow::servers->getServers()[0];
						std::string iaxServer2 = Kiax2MainWindow::servers->getServers()[1];
						QString number = url.queryItemValue("number");
						Account* account = new Account();
						account->host1 = iaxServer1;
						account->host2 = iaxServer2;
						account->username = username.toStdString();
						account->password = password.toStdString();
						if ((number!="")&&(number!="null"))
						{
							if (path=="/hangupandcall")
							{
								//Kiax2MainWindow::instance->queueCall(number, account);
								Kiax2MainWindow::instance->httpCallWithAccount(number, account);
							}
							else
							{
								Kiax2MainWindow::instance->httpCallWithAccount(number, account);
							}
						}
					}
					else {
						Logger::log(Logger::DEBUG,"supernode user\n");
						QString number = url.queryItemValue("number");
						if ((number!="")&&(number!="null"))
						{
							if (path=="/hangupandcall")
							{							
								Kiax2MainWindow::instance->httpCall(number);
							}
							else 
							{
								Kiax2MainWindow::instance->httpCall(number);
							}
						}
					}
				}					
				else {
					Logger::log(Logger::DEBUG,"no api servers present\n");
					QString number = url.queryItemValue("number");
					if ((number!="")&&(number!="null"))
					{
						if (path=="/hangupandcall")
						{							
							Kiax2MainWindow::instance->httpCall(number);
						}
						else 
						{
							Kiax2MainWindow::instance->httpCall(number);
						}
					}
				}	
				if (path=="/hangupandcall")
				{
					Logger::log(Logger::DEBUG, "hangupandcall - hanging up all calls (%d).\n", numbersSize);
					for (int i=0; i<numbersSize; i++) 
					{
						Logger::log(Logger::DEBUG, "hungupandcall - number %d\n", numbers[i]);
					}
					
					Kiax2MainWindow::instance->hangupCalls(numbers, numbersSize);
				}
		  }
	}
	if (path=="/hangup")
	{
		server->emitHangup();
	}		
	if (path=="/register")
	{
		QList< QPair<QString, QString> > parameters = url.queryItems();
		QString username;
		QString password;
		QString host1;
		QString host2;
		QString session;
		
		int paramSize = parameters.count();
		for (int i = 0; i<paramSize; i++)
		{
			QPair<QString, QString> paramPair = parameters[i];
			if (paramPair.first==HTTP_CONFIG_PARAM_USERNAME)
			{
				username = paramPair.second;
			}
			else if(paramPair.first == HTTP_CONFIG_PARAM_PASSWORD)
			{
				password = paramPair.second;
			}
			else if (paramPair.first == HTTP_CONFIG_PARAM_HOST)
			{
				host1 = paramPair.second;
			}
			else if (paramPair.first == HTTP_CONFIG_PARAM_FALLBACK_HOST)
			{
				host2 = paramPair.second;
			} 
			else if (paramPair.first == HTTP_CONFIG_PARAM_SESSION)
			{
				session = paramPair.second;
			}
		}
		Logger::log(Logger::DEBUG, "DispatcherThread username=%s, password=%s, host1=%s, host2=%s\n",
									username.toStdString().data(),
									password.toStdString().data(),
									host1.toStdString().data(),
									host2.toStdString().data());
		
		server->emitConfigureAccount(username, password, host1, host2);

	}
	
	Logger::log(Logger::DEBUG, "readMessage() OUT\n");
}

void Kiax2DispatcherThread::requestCompleteSlot()
{
	connection->disconnectFromHost();
    if (connection->state() != QAbstractSocket::UnconnectedState)
		connection->waitForDisconnected();																	
	emit runOut(this);	
}

void Kiax2DispatcherThread::connectionRefusedSlot(QUrl url)
{
	
}

void Kiax2DispatcherThread::requestCompleteSupernode(bool errorFlag, QString data)
{	
	QString result = jsonSupernode->getCallResult(data);
	if (errorFlag)
	{
		Logger::log(Logger::INFO, "supernode  api call failed.\n");
	}
	Logger::log(Logger::INFO, "Supernode API call returned message --%s--.\n", result.toStdString().data());
}

void Kiax2DispatcherThread::requestCompleteLocal(bool errorFlag, QString data)
{	
	QString result = jsonLocal->getCallResult(data);
	if (errorFlag)
	{
		Logger::log(Logger::INFO, "local pbx api call failed.\n");
	}
	Logger::log(Logger::INFO, "Local API call returned message --%s--.\n", result.toStdString().data());

}

