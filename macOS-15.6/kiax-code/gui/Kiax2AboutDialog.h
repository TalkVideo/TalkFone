#ifndef KIAX2ABOUTDIALOG_H
#define KIAX2ABOUTDIALOG_H

#include "ui_aboutdialog.h"

class Kiax2AboutDialog : public QDialog
{

  Q_OBJECT

public:
  Kiax2AboutDialog(QWidget* parent);
  virtual ~Kiax2AboutDialog();
  /*$PUBLIC_FUNCTIONS$*/

public slots:
  /*$PUBLIC_SLOTS$*/
	void urlActivated(const QString & link);
protected:
  /*$PROTECTED_FUNCTIONS$*/

protected slots:
  /*$PROTECTED_SLOTS$*/

private:

	Ui::AboutDialog ui;
  
};

#endif

