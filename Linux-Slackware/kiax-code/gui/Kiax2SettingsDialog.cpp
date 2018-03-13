//#include "Kiax2SettingsDialog.h"
#include "Kiax2MainWindow.h"

Kiax2SettingsDialog::Kiax2SettingsDialog(QWidget* parent) : QDialog(parent)
{
	app = parent;
	ui.setupUi(this);
#if defined(MACXDEP)
	setWindowFlags(Qt::Sheet);
#endif

	ui.routingComboBox->setVisible(ENABLE_HTTP_CONFIGURATION);
	ui.routingLabel->setVisible(ENABLE_HTTP_CONFIGURATION);
	if ((!NOTIFICATION_ENABLED)||(!NOTIFICATION_SETTINGS_ENABLED))
	{
		ui.versionInfoCheckBox->setVisible(false); 
	}
#if !defined(WIN32DEP)
	ui.startupComboBox->setVisible(false);
	ui.startupLabel->setVisible(false);
#endif
	connect(ui.saveButton, SIGNAL(clicked()), this, SLOT(saveSettings()));
	connect(ui.cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
	connect(ui.autoCheckBox, SIGNAL(toggled(bool)), this, SLOT(autoChecked(bool)));
	connect(ui.networkAutoCheckBox, SIGNAL(toggled(bool)), this, SLOT(networkAutoChecked(bool)));
	int entries = ui.codecComboBox->count();
	for (int i=0;i< entries;i++)
	{
		ui.codecComboBox->removeItem(0);
	}
	QStringList items;
	items << ULAW_FRIENDLY_NAME;
	items << SPEEX_FRIENDLY_NAME;
	if (ENABLE_ILBC_AND_GSM) 
	{	
		items << GSM_FRIENDLY_NAME;
		items << ILBC_FRIENDLY_NAME;
	}
	ui.codecComboBox->addItems(items);
	if (!ADVANCED_SETTINGS)
		ui.tabWidget->removeTab(1);
	detectDevices();
	restoreSettings(); 
	if (DISABLE_CODEC_SETTING)
	{
		ui.networkGroupBox->setVisible(false);
	}
	
	if (DISABLE_AUTOCONNECT_SETTING)
	{
		ui.startupCheckBoxes->setVisible(false);
		resize(420, 220);
	}
}

Kiax2SettingsDialog::~Kiax2SettingsDialog()
{
}

int Kiax2SettingsDialog::reversedMapFind(std::map<int, int> map, int deviceNumber)
{
	std::map<int, int>::iterator mapIterator;
	for (mapIterator= map.begin(); mapIterator != map.end(); mapIterator++)
	{
		if ((*mapIterator).second == deviceNumber)
			return (*mapIterator).first;
	}
	return -1;
}

void Kiax2SettingsDialog::restoreSettings()
{
	std::string paramOutputIndex("OutputDeviceIndex");
	std::string paramOutputName("OutputDeviceName");
	std::string paramInputIndex("InputDeviceIndex");
	std::string paramInputName("InputDeviceName");
	std::string paramRingIndex("RingDeviceIndex");
	std::string paramRingName("RingDeviceName");
	std::string paramHide("ShowMainWindow");
	std::string paramConnect("ConnectOnStart");
	std::string paramAuto("StateAuto");
	std::string paramNetworkAuto("NetworkAuto");
	std::string paramCodec("DefaultCodec");
	std::string paramVersionCheck("RequestVersionCheck");
	std::string paramStartup("RunOnStartup");
	std::string paramRouting("Routing");
	std::string paramRoutingDestination("RoutingDestination");
	std::string paramDialWinOnTop("DialWinOnTop");
	std::string paramMainWinOnTop("MainWinOnTop");
	
	Kiax2MainWindow* mw = (Kiax2MainWindow*) app;
	
	bool autoChecked = mw->readParameter(paramAuto, "true").compare("true")==0?true:false;	
	bool hideChecked = mw->readParameter(paramHide, "false").compare("true")==0?false:true;
	bool connectChecked = mw->readParameter(paramConnect, "true").compare("true")==0?true:false;
	bool dialWinChecked = mw->readParameter(paramDialWinOnTop, "true").compare("true")==0?true:false;
	bool mainWinChecked = mw->readParameter(paramMainWinOnTop, "false").compare("true")==0?true:false;
	bool networkAutoChecked = mw->readParameter(paramNetworkAuto, "true").compare("true")==0?true:false;
	bool requestVersionChecked = mw->readParameter(paramVersionCheck, "true").compare("true")==0?true:false;
	int codecNumber = mw->strToInt(mw->readParameter(paramCodec, "0"));
	std::string startupStr = mw->readParameter(paramStartup, "0");
	std::string routingStr = mw->readParameter(paramRouting, "0");
	int runOnStartup = (QString::fromStdString(startupStr)).toInt();
	int routing = (QString::fromStdString(routingStr)).toInt();
	
	ui.hideCheckBox->setChecked(hideChecked);
	ui.onStartupCheckBox->setChecked(connectChecked);
	ui.autoCheckBox->setChecked(autoChecked);
	ui.networkAutoCheckBox->setChecked(networkAutoChecked);
	ui.codecComboBox->setCurrentIndex(codecNumber);
	ui.versionInfoCheckBox->setChecked(requestVersionChecked);
	ui.startupComboBox->setCurrentIndex(runOnStartup);
	ui.routingComboBox->setCurrentIndex(routing);
	ui.mainWinOnTop->setChecked(mainWinChecked);
	ui.dialWinOnTop->setChecked(dialWinChecked);

	mainWinOnTop = mainWinChecked;
	dialWinOnTop = dialWinChecked;


	std::string outputName = mw->readParameter(paramOutputName);
	int outputIndex = mw->strToInt(mw->readParameter(paramOutputIndex, "0"));
	std::string inputName = mw->readParameter(paramInputName);
	int inputIndex = mw->strToInt(mw->readParameter(paramInputIndex, "0"));
	std::string ringName = mw->readParameter(paramRingName);
	int ringIndex = mw->strToInt(mw->readParameter(paramRingIndex, "0"));
	
	int inputComboIndex = reversedMapFind(inputMap, inputIndex);
	int outputComboIndex = reversedMapFind(outputMap, outputIndex);
	int ringComboIndex = reversedMapFind(ringMap, ringIndex);
	
	std::string inputDeviceName = ui.inputDeviceComboBox->itemText(inputComboIndex).toStdString();
	std::string outputDeviceName = ui.outputDeviceComboBox->itemText(outputComboIndex).toStdString();
	std::string ringDeviceName = ui.ringDeviceComboBox->itemText(ringComboIndex).toStdString();
	
	Logger::log(Logger::DEBUG, "inputName = %s, comboText = %s\n", inputName.data(), inputDeviceName.data());
	Logger::log(Logger::DEBUG, "outputName = %s, comboText = %s\n", outputName.data(), outputDeviceName.data());
	Logger::log(Logger::DEBUG, "ringName = %s, comboText = %s\n", ringName.data(), ringDeviceName.data());
	
	if ((outputName.compare(outputDeviceName)==0)&&
		(inputName.compare(inputDeviceName)==0)&&
		(ringName.compare(ringDeviceName)==0))
	{
		ui.inputDeviceComboBox->setCurrentIndex(inputComboIndex);
		ui.outputDeviceComboBox->setCurrentIndex(outputComboIndex);
		ui.ringDeviceComboBox->setCurrentIndex(ringComboIndex);
	}
}

void Kiax2SettingsDialog::detectDevices()
{

  struct iaxc_audio_device *devices;
  int devNumber,input,output,ring;
  long devCapabilities;

  iaxc_audio_devices_get(&devices, &devNumber, &input, &output, &ring);

  QStringList inputDevices;
  QStringList outputDevices;
  QStringList ringDevices;

  inputMap.clear();
  outputMap.clear();
  ringMap.clear();
  
  int inputCount = 0;
  int outputCount = 0;
  int ringCount = 0;
  
  for(int i=0; i<devNumber; i++)
  {
    devCapabilities = devices->capabilities;
    QString devName(devices->name);
    if(devCapabilities & IAXC_AD_INPUT) 
	{
		inputDevices << devName;
		inputMap[inputCount] = i;
		inputCount++;
	}
    if(devCapabilities & IAXC_AD_OUTPUT) 
	{
		outputDevices << devName;
		outputMap[outputCount] = i;
		outputCount++;
	}
    if(devCapabilities & IAXC_AD_RING)
	{
		ringDevices << devName;
		ringMap[ringCount] = i;
		ringCount++;
	}
    devices++;
  }
  
  ui.inputDeviceComboBox->insertItems(0, inputDevices);
  ui.outputDeviceComboBox->insertItems(0, outputDevices);
  ui.ringDeviceComboBox->insertItems(0, ringDevices);

}

void Kiax2SettingsDialog::saveSettings()
{
	Kiax2MainWindow* mw = (Kiax2MainWindow*) app;
	int input, output, ring;
	input = ui.inputDeviceComboBox->currentIndex();
	output = ui.outputDeviceComboBox->currentIndex();
	ring = ui.ringDeviceComboBox->currentIndex();
	iaxc_audio_devices_set(inputMap[input], outputMap[output], ringMap[ring]);
	std::string paramOutputIndex("OutputDeviceIndex");
	std::string paramOutputName("OutputDeviceName");
	std::string paramInputIndex("InputDeviceIndex");
	std::string paramInputName("InputDeviceName");
	std::string paramRingIndex("RingDeviceIndex");
	std::string paramRingName("RingDeviceName");
	std::string paramHide("ShowMainWindow");
	std::string paramConnect("ConnectOnStart");
	std::string paramAuto("StateAuto");
	std::string paramNetworkAuto("NetworkAuto");
	std::string paramCodec("DefaultCodec");
	std::string paramVersionCheck("RequestVersionCheck");
	std::string paramStartup("RunOnStartup");
	std::string paramRouting("Routing");
	std::string paramDialWinOnTop("DialWinOnTop");
	std::string paramMainWinOnTop("MainWinOnTop");
	std::string paramRoutingDestination("RoutingDestination");



	int runOnStartup = ui.startupComboBox->currentIndex();
	int routing = ui.routingComboBox->currentIndex();
	
	mw->saveParameter(paramInputIndex, mw->intToStr(inputMap[input]));
	mw->saveParameter(paramInputName, ui.inputDeviceComboBox->currentText().toStdString());
	mw->saveParameter(paramOutputIndex, mw->intToStr(outputMap[output]));
	mw->saveParameter(paramOutputName, ui.outputDeviceComboBox->currentText().toStdString());
	mw->saveParameter(paramRingIndex, mw->intToStr(ringMap[ring]));
	mw->saveParameter(paramRingName, ui.ringDeviceComboBox->currentText().toStdString());
	mw->saveParameter(paramHide, ui.hideCheckBox->isChecked()?"false":"true");
	mw->saveParameter(paramDialWinOnTop, ui.dialWinOnTop->isChecked()?"true":"false");
	mw->saveParameter(paramMainWinOnTop, ui.mainWinOnTop->isChecked()?"true":"false");
	mw->saveParameter(paramConnect, ui.onStartupCheckBox->isChecked()?"true":"false");
	mw->saveParameter(paramAuto, ui.autoCheckBox->isChecked()?"true":"false");
	mw->saveParameter(paramNetworkAuto, ui.networkAutoCheckBox->isChecked()?"true":"false");
	mw->saveParameter(paramCodec, mw->intToStr(ui.codecComboBox->currentIndex()));
	int defaultCodec = ui.codecComboBox->currentIndex();
	mw->setDefaultCodec(defaultCodec);
	mw->saveParameter(paramVersionCheck, ui.versionInfoCheckBox->isChecked()?"true":"false");	
	mw->saveParameter(paramStartup, QString::number(runOnStartup).toStdString());
	mw->saveParameter(paramRouting, QString::number(routing).toStdString());
	mw->routing = routing;

	mainWinOnTop = ui.mainWinOnTop->isChecked();
	dialWinOnTop = ui.dialWinOnTop->isChecked();

	accept();
}

bool Kiax2SettingsDialog::isMainWinOnTop()
{
	return mainWinOnTop;
}

bool Kiax2SettingsDialog::isDialWinOnTop()
{
	return dialWinOnTop;
}

void Kiax2SettingsDialog::autoChecked(bool checked)
{
	if (checked) 
	{
		ui.hideCheckBox->setEnabled(false);
		ui.onStartupCheckBox->setEnabled(false);
	}
	else
	{
		ui.hideCheckBox->setEnabled(true);
		ui.onStartupCheckBox->setEnabled(true);

	}
	
}

void Kiax2SettingsDialog::networkAutoChecked(bool checked)
{
	if (checked) 
	{
		ui.codecComboBox->setEnabled(false);
		ui.codecLabel->setEnabled(false);
	}
	else
	{
		ui.codecComboBox->setEnabled(true);
		ui.codecLabel->setEnabled(true);

	}
	
}

void Kiax2SettingsDialog::onStartupChecked(bool checked)
{
}

void Kiax2SettingsDialog::hideChecked(bool checked)
{
}
