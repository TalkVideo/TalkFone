#ifndef KIAX2REGISTERDIALOG_H
#define KIAX2REGISTERDIALOG_H

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

#define STATE_REGISTER_NONE					0
#define STATE_REGISTER_LOAD_PROMPT			2
#define STATE_REGISTER_PROMPT				3
#define STATE_REGISTER_PROCESS				4
#define STATE_REGISTER_PROCESS_ERROR		5
#define STATE_REGISTER_PROCESS_SUCCESSFUL	6

class Kiax2RegisterDialog : public QDialog
{

  Q_OBJECT

public:
  Kiax2RegisterDialog(QWidget* parent, QString url = GET_ACCOUNT_URL);
  ~Kiax2RegisterDialog();
  void loadPage();
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

   void registrationSuccessful(QString username, QString password);
   void passwordChangeSuccessful(QString username, QString password);
   
private:

	Ui::RegisterDialog ui;
	QUrl* url;
	int state;
};

#endif

