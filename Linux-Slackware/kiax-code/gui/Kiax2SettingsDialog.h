#ifndef KIAX2SETTINGSDIALOG_H
#define KIAX2SETTINGSDIALOG_H

#include "ui_settingsdialog.h"
#include <string>
#include <QString>
#include <QStringList>
#include "PConstants.h"
#include "PConfiguration.h"
#include "iaxclient.h"
#include <map>



class Kiax2SettingsDialog : public QDialog
{

  Q_OBJECT

public:
  Kiax2SettingsDialog(QWidget* parent);
  ~Kiax2SettingsDialog();
  void detectDevices();
  void restoreSettings();
  int reversedMapFind(std::map<int, int> map, int deviceNumber);
  bool isDialWinOnTop();
  bool isMainWinOnTop();

public slots:
	void saveSettings();
	void autoChecked(bool checked);
	void onStartupChecked(bool checked);
	void hideChecked(bool checked);
	void networkAutoChecked(bool checked);

protected:
  /*$PROTECTED_FUNCTIONS$*/

protected slots:
  /*$PROTECTED_SLOTS$*/

private:

	Ui::SettingsDialog ui;
    std::map<int, int> inputMap;
    std::map<int, int> outputMap;
    std::map<int, int> ringMap;
    std::map<int, int> codecMap;
	QWidget* app;
    bool mainWinOnTop;
    bool dialWinOnTop;

};

#endif

