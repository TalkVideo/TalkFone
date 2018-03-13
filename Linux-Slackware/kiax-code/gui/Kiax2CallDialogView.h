#ifndef KIAX2CALLDIALOGVIEW_H
#define KIAX2CALLDIALOGVIEW_H

#include <QDialog>
#include <QWidget>
#include <QEvent>
#include <QCloseEvent>
#include <QShowEvent>

class Kiax2CallDialogView : public QDialog
{

  Q_OBJECT

public:
  Kiax2CallDialogView(QWidget* parent = NULL);
  virtual ~Kiax2CallDialogView();
  /*$PUBLIC_FUNCTIONS$*/

public slots:
  /*$PUBLIC_SLOTS$*/
  virtual void reject();

protected:
  /*$PROTECTED_FUNCTIONS$*/
	virtual void closeEvent(QCloseEvent* event);	
	virtual void changeEvent(QEvent* event);	
	virtual void showEvent ( QShowEvent * event );

signals:

	void dialogClosed();
	void dialogShown();
	void dialogChanged(bool active);
};

#endif

