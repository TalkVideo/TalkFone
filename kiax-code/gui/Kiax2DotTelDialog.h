#ifndef KIAX2DOTTELDIALOG_H
#define KIAX2DOTTELDIALOG_H

#include "ui_dotteldialog.h"
#include "PConstants.h"
#include "PConfiguration.h"
#include <vector>

using namespace std;

class Kiax2DotTelDialog : public QDialog
{

  Q_OBJECT

public:
  Kiax2DotTelDialog(QWidget* parent);
  ~Kiax2DotTelDialog();
  /*$PUBLIC_FUNCTIONS$*/

public slots:
  /*$PUBLIC_SLOTS$*/
	void dialButtonClicked();
	void cancelButtonClicked();
	void addTelNumber(QString number, QString description);
	void currentItemChanged(QListWidgetItem* current, QListWidgetItem* previous);
	QString getNumber();
	void addTelDescription(QString descr);	
protected:
  /*$PROTECTED_FUNCTIONS$*/

protected slots:
  /*$PROTECTED_SLOTS$*/

private:

	Ui::DotTelDialog ui;
	QString number;
	vector<QString> numbers;
};

#endif

