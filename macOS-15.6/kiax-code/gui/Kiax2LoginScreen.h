#ifndef KIAX2LOGINSCREEN_H
#define KIAX2LOGINSCREEN_H

#include "ui_loginscreen.h"
#include <string>


class Kiax2LoginScreen : public QFrame
{

  Q_OBJECT

public:
  Kiax2LoginScreen(QWidget* parent);
  ~Kiax2LoginScreen();
  void setUsername(QString username);
  void setPassword(QString password);
  void setLoginStatus(QString status);
  /*$PUBLIC_FUNCTIONS$*/

public slots:
  /*$PUBLIC_SLOTS$*/
  void getAccount();
  void loginPressed();
  
protected:
  /*$PROTECTED_FUNCTIONS$*/

signals:

	void login(QString username, QString password);
	void account();
	
protected slots:
  /*$PROTECTED_SLOTS$*/

private:

	Ui::LoginScreen ui;
};

#endif

