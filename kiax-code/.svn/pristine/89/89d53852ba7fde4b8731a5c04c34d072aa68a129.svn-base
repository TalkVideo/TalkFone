#ifndef KIAX2ACCOUNTDIALOG_H
#define KIAX2ACCOUNTDIALOG_H

#include "ui_accountdialog.h"
#include <string>
#include "PConstants.h"
#include "PConfiguration.h"
#include <QDesktopServices>
#include <QUrl>

class Kiax2AccountDialog : public QDialog
{

  Q_OBJECT

public:
  Kiax2AccountDialog(QWidget* parent);
  ~Kiax2AccountDialog();
  std::string getUsernameText();
  std::string getServerText();
  std::string getPasswordText();
  void setUsernameText(std::string text);
  void setServerText(std::string text);
  void setPasswordText(std::string text);
  /*$PUBLIC_FUNCTIONS$*/

public slots:
  /*$PUBLIC_SLOTS$*/
  void getAccount();

protected:
  /*$PROTECTED_FUNCTIONS$*/

protected slots:
  /*$PROTECTED_SLOTS$*/

private:

	Ui::AccountDialog ui;
};

#endif

