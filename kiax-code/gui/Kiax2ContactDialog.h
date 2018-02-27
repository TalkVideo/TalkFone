#ifndef KIAX2CONTACTDIALOG_H
#define KIAX2CONTACTDIALOG_H

#include "ui_contactdialog.h"
#include <string>

class Kiax2ContactDialog : public QDialog
{

  Q_OBJECT

public:
  Kiax2ContactDialog(QWidget* parent);
  virtual ~Kiax2ContactDialog();
  /*$PUBLIC_FUNCTIONS$*/
  std::string getContactNameText();
  std::string getExtensionText();
  void setContactNameText(std::string text);
  void setExtensionText(std::string text);
public slots:
  /*$PUBLIC_SLOTS$*/
void verifyAccept();

protected:
  /*$PROTECTED_FUNCTIONS$*/

protected slots:
  /*$PROTECTED_SLOTS$*/

private:

	Ui::ContactDialog ui;
  
};

#endif

