#ifndef KIAX2APPLICATION_H
#define KIAX2APPLICATION_H

#include <QApplication>
#include <QEvent>
#include <QSharedMemory>
#include <QLocalServer>

class Kiax2Application : public QApplication
{

  Q_OBJECT

public:

  Kiax2Application(int &argc, char *argv[], const QString uniqueKey);
  
  ~Kiax2Application();
  virtual bool event(QEvent* event);
  
  bool isRunning();
  bool sendMessage(const QString &message);
 
#if defined(MACXDEP)  
  virtual bool macEventFilter(EventHandlerCallRef caller, EventRef event);
#endif

public slots:
	void receiveMessage();

signals: 
  void applicationActivated();
  void messageAvailable(QString message);

private:
  bool _isRunning;
  QString _uniqueKey;
  QSharedMemory sharedMemory;
  QLocalServer *localServer;

  static const int timeout = 1000;
};

#endif
