#ifndef KIAX2CHANGEPASSDIALOG_H
#define KIAX2CHANGEPASSDIALOG_H

#include "ui_registerdialog.h"
#include <string>
#include "PConstants.h"
#include "PConfiguration.h"
#include <QUrl>
#include "Logger.h"
#include <QWebPage>
#include <QWebFrame>

#include <QtWebKit/QWebView>
#include <QList>
#include <QNetworkReply>
#include <QSslError>

#define STATE_CHANGE_NONE				0
#define STATE_CHANGE_LOAD_PROMPT		2
#define STATE_CHANGE_PROMPT				3
#define STATE_CHANGE_PROCESS			4
#define STATE_CHANGE_PROCESS_ERROR		5
#define STATE_CHANGE_PROCESS_SUCCESSFUL	6

class Kiax2ChangePassDialog : public QDialog
{

  Q_OBJECT

public:
  Kiax2ChangePassDialog(QWidget* parent, QString url = CHANGE_PASSWORD_URL);
  ~Kiax2ChangePassDialog();
  void loadPage();
  void setUsername(QString userlogin);
  /*$PUBLIC_FUNCTIONS$*/

public slots:
  /*$PUBLIC_SLOTS$*/

protected:
  /*$PROTECTED_FUNCTIONS$*/

protected slots:
  /*$PROTECTED_SLOTS$*/
  virtual void loadStarted();
  virtual void loadFinished();
  virtual void slotSslErrors(QNetworkReply* replay, const QList<QSslError>& error);

signals:

   void passwordChangeSuccessful(QString username, QString password);
  
private:
	QString username;
	Ui::RegisterDialog ui;
	QString url;
	int state;
};

#endif

