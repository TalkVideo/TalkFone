#include "Kiax2MainWindow.h"
#include <sstream>
  
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <QCryptographicHash>
#include <QRegExp>
 
inline double fround(double n, unsigned d)
{
return floor(n * pow(10., d) + .5) / pow(10., d);
}
  
Kiax2MainWindow::Kiax2MainWindow(Kiax2Application* app)
{
	instance = this;
	appTranslator = NULL;
	i18n("es");
	
#if defined(MACXDEP)
	chdir(MAC_BINARY_DIR);
#endif
	application = app;

    int argc = app->argc();
    char** args = app->argv();
	
	Logger::init(LOG_LEVEL);
	
    if (argc>1) {
        char* exten=NULL;

		for (int i=1;i<argc;i++) {
			if (strcmp(args[i],"--debug")==0) {
				int dlevel=QString(args[i+1]).toInt();
				Logger::setDebugLevel(dlevel);
			}				
		}
	}
	
	Core::instance();
	api = APIBinderDefault::instance();

	lastNotification = "no notification";
	lastNotificationUrl = "no notification";
    
	if (ENABLE_HTTP_CONFIGURATION)
	{
		initApiServer(); // confServer = new Kiax2ConfigServer(NULL);
	}
	else
	{
		apiServer = NULL;
	}
	
	Logger::log(Logger::DEBUG, "Initializing qt callback handler..\n");
	
	IAX2CallbackHandlerQt* qtCallbackHandler = new IAX2CallbackHandlerQt(this);
	((SignalingModuleIAX2*)SignalingModuleIAX2::instance())->setIAX2CallbackHandler(qtCallbackHandler);
	api->addSignalHandler(this);

	Logger::log(Logger::DEBUG, "Adjusting devices..\n");
	adjustDevices();

	
	if (ENABLE_SPEEDTEST) 
	{
		Logger::log(Logger::DEBUG, "Initializing connection meter..\n");
		connectionMeter = new Kiax2ConnectionMeter(this);
	}
	
	  
	defaultAccount = new Account(); // initialize default account
#ifdef USE_WEBFORMS
	registerDialog = new Kiax2RegisterDialog(this, GET_ACCOUNT_URL);
	changePasswordDialog = new Kiax2ChangePassDialog(this, CHANGE_PASSWORD_URL);	
#endif
	Logger::log(Logger::DEBUG, "Setting up ui widgets..\n");
	setupUi(this); 
 	app->setQuitOnLastWindowClosed(false);
	if (ENABLE_LOGIN_SCREEN)
	{
		loginScreen = new Kiax2LoginScreen(centralwidget);
		loginScreen->setVisible(false);
		loginScreen->setFrameShape(QFrame::NoFrame);
		loginScreen->setFrameShadow(QFrame::Sunken);
		loginScreen->setLineWidth(-1);
		loginScreen->setMidLineWidth(-1);
		mainLayout->removeWidget(loginScreen);  
	}
	if (ENABLE_CALL_RATES)
	{
		actionCallRates = new QAction(this);
    		actionCallRates->setObjectName(QString::fromUtf8("actionCallRates"));
    		QIcon ratesIcon;
    		ratesIcon.addPixmap(QPixmap(QString::fromUtf8("icons/about.png")), QIcon::Normal, QIcon::Off);
    		actionCallRates->setIcon(ratesIcon);
    		actionCallRates->setMenuRole(QAction::TextHeuristicRole);
		actionCallRates->setText(CALL_RATES_TEXT);
		menuHelp->insertAction(actionAbout, actionCallRates);
		menuHelp->insertSeparator(actionAbout);
 
	}
	if (DISABLE_CONNECT)
	{
		menuService->removeAction(actionConnect);
		actionConnect->setEnabled(false);
	}
	if (CREDIT_LINK_IN_SERVICE_MENU)
	{
			actionCredit = new QAction(this);
    		actionCredit->setObjectName(QString::fromUtf8("actionCredit"));
    		actionCredit->setMenuRole(QAction::TextHeuristicRole);
    		QIcon ratesIcon;
    		ratesIcon.addPixmap(QPixmap(QString::fromUtf8("icons/call.png")), QIcon::Normal, QIcon::Off);
    		actionCredit->setIcon(ratesIcon);
		actionCredit->setText(tr("Buy Minutes"));
		if (!DISABLE_ACCOUNT_MENU)
			menuAccount->addAction(actionCredit);
		connect(actionCredit, SIGNAL(triggered(bool)), this, SLOT(balanceMenuClicked()));
		
		//menuService->insertSeparator(menuAccount);
 
	}
			
	if (ENABLE_ONLINE_USERS)
	{
		jsonUsersOnlineRequestUrl = QString(JSON_USERS_ONLINE_REQUEST_URL);
		onlineUsersButton = new QLabel(onlineStatusFrame);
		onlineUsersButton->setText("");
		onlineUsersButton->setObjectName(QString::fromUtf8("onlineUsersButton"));
		QPalette buttonPalette;
		QBrush buttonBrush(QColor(71, 71, 71, 255));
		buttonBrush.setStyle(Qt::SolidPattern);
		buttonPalette.setBrush(QPalette::Active, QPalette::Text, buttonBrush);
		buttonPalette.setBrush(QPalette::Inactive, QPalette::Text, buttonBrush);
		onlineUsersButton->setPalette(buttonPalette);
		//onlineUsersButton->setFlat(true);
		statusLayout->addWidget(onlineUsersButton, 0, 3, 1, 1);
		Logger::log(Logger::DEBUG, "Initializing json UsersOnline widget..\n");
		jsonUsersOnline = new Kiax2JSONUsersOnline(this);
	}
	if (ENABLE_BALANCE)
	{
		Logger::log(Logger::DEBUG, "Initializing json balance widget..\n");
		jsonBalance = new Kiax2JSONBalance(this);
	}
	else
	{ 
		balanceLabel->setVisible(false); 
		statusLayout->removeWidget(balanceLabel);
		balanceLabel->setVisible(false);
	}

	if (NOTIFICATION_ENABLED)
	{
		jsonNotification = new Kiax2JSONNotification(this);
		jsonNotification->setVersion(QString(SOFTPHONE_VERSION));
	}



	if (ENABLE_WEB_SERVICES)
	{
		if (!DISABLE_ACCOUNT_MENU) 
		{
		    menuAccount->addAction(actionRegister);
			//menuAccount->addAction(actionPassword);
			menuAccount->addSeparator();
		}
	}
	
	if(DISABLE_ACCOUNT_MENU)
	{
		menuService->clear();
		menuService->addAction(actionSettings);
		menuService->addAction(actionExit);
	}

	if (!DISABLE_ACCOUNT_CONFIGURE_MENU)
		if (!DISABLE_ACCOUNT_MENU)
			menuAccount->addAction(actionAccount);

	Logger::log(Logger::DEBUG, "Initializing call records widget..\n");
#if !defined(TABBED_LAYOUT)
		callRecords = new QListWidget(centralwidget);
		callRecords->setObjectName(QString::fromUtf8("callRecords"));
		callRecords->setFrameShape(QFrame::Box);
#ifdef DEFAULT_CALL_RECORDS_HEIGHT
		callRecords->setMinimumSize(0,DEFAULT_CALL_RECORDS_HEIGHT);
#endif
		callRecords->setVisible(false);
#else
		callRecords = cdrList;
#endif
		callRecords->setContextMenuPolicy(Qt::CustomContextMenu);
		statusButton->setText(STATUS_DISCONNECTED_LABEL); 

	window()->layout()->setSpacing(0);
		
	sysTray = new QSystemTrayIcon(this);
	sysTray->setIcon(QIcon("icons/disconnectedsmall.png"));
	sysTray->setVisible(true);
	sysTray->setToolTip(DEFAULT_SERVICE_NAME);
	searchComboBox->lineEdit()->setText(tr("Search contacts.."));
#ifdef TABBED_LAYOUT
		cdrSearchComboBox->lineEdit()->setText(tr("Search call history.."));
		callButtonsGroup = new QButtonGroup(callsFrame);
		callButtonsGroup->setExclusive(true);
#endif
	numberEdit->setText(tr("Type a number to dial.."));
	contactsList->setSortingEnabled(true);
	callRecords->setSortingEnabled(false); 
	contactsList->setContextMenuPolicy(Qt::CustomContextMenu);

	contactContextMenu = new QMenu(contactsList);
	contactContextMenu->addAction(actionCall);
	contactContextMenu->addSeparator();
	contactContextMenu->addAction(actionEdit_Contact);
	contactContextMenu->addSeparator();
	contactContextMenu->addAction(actionDelete_Contact);

#ifndef REMOTE_CDR
	cdrContextMenu = new QMenu(callRecords);
	cdrContextMenu->addAction(actionCall);
	if (!DISABLE_HISTORY_ADD_CONTACT)
	{
		cdrContextMenu->addSeparator();
		cdrContextMenu->addAction(actionAdd_Contact);
	}
#endif
	statusMenu = new QMenu(statusButton);
	statusMenu->addAction(actionConnect);
	statusMenu->addAction(actionDisconnect);
	statusButton->setMenu(statusMenu);

	sysTrayMenu = new QMenu();
	if (!DISABLE_CONNECT)
	{
		sysTrayMenu->addAction(actionConnect);
		sysTrayMenu->addAction(actionDisconnect);
		sysTrayMenu->addSeparator();
	}
	
#if !defined(MACXDEP)
	if (ENABLE_SHOW_HIDE_COMMANDS)
	{
		sysTrayMenu->addAction(actionHide);
		sysTrayMenu->addAction(actionShow);
		sysTrayMenu->addSeparator();
	}
#endif	

	sysTrayMenu->setTitle(DEFAULT_SERVICE_NAME);
	sysTrayMenu->addAction(actionExit);
	sysTray->setContextMenu(sysTrayMenu);

#if !defined(MACXDEP)
	setWindowIcon(QIcon("icons/applogo.png"));
#endif

	actionDisconnect->setEnabled(false);
	if (SINGLE_CONNECT_DISCONNECT)
		actionDisconnect->setVisible(false);
	
	ringInTone = NULL;
	ringTimer = new QTimer();
	if (ENABLE_FEED_TIMER)
	{
		feedTimer = new QTimer();
		feedTimer->start(FEED_TIMEOUT);
	}
	if (ENCODE_TEL_EXTEN)
	{
		jsonSession = new Kiax2JSONNotification(this);
		sessionTimer = new QTimer();
		sessionTimer->start(SESSION_TIMEOUT);
	}
	if (ENABLE_BALANCE)
	{
		balanceTimer = new QTimer();
		balanceTimer->setSingleShot(true);
	}
	if (NOTIFICATION_ENABLED)
	{
		if (NOTIFICATION_SETTINGS_ENABLED)
		{
			if (paramRequestNotification())
			{
				notificationTimer = new QTimer();
				notificationTimer->start(JSON_NOTIFICATION_TIMER);
			}
		}
		else {
			notificationTimer = new QTimer();
			notificationTimer->start(JSON_NOTIFICATION_TIMER);
		}
	}

	encodeSession = generateSession();

	dialpadButton->setChecked(DEFAULT_DIALPAD_OPEN);
	Logger::log(Logger::DEBUG, "Connecting widgets..\n");
	connect( app, SIGNAL( focusChanged(QWidget*, QWidget*) ), this, SLOT( widgetFocusChanged(QWidget*, QWidget*) ) );
	connect( app, SIGNAL( applicationActivated() ), this, SLOT( applicationActivated() ) );
    connect( actionAbout, SIGNAL( triggered( bool ) ), this, SLOT( aboutAction_activated() ) );
    connect( actionCall, SIGNAL( triggered( bool ) ), this, SLOT( callAction_activated() ) );
    connect( actionAccount, SIGNAL( triggered( bool ) ), this, SLOT( accountAction_activated() ) );
    connect( actionRegister, SIGNAL( triggered( bool ) ), this, SLOT( registerAction_activated() ) );
    connect( actionPassword, SIGNAL( triggered( bool ) ), this, SLOT( changePasswordAction_activated() ) );
    connect( actionAdd_Contact, SIGNAL( triggered( bool ) ), this, SLOT( addContactAction_activated() ) );
    connect( actionEdit_Contact, SIGNAL( triggered( bool ) ), this, SLOT( editContactAction_activated() ) );
    connect( actionDelete_Contact, SIGNAL( triggered( bool ) ), this, SLOT( deleteContactAction_activated() ) );
    connect( actionConnect, SIGNAL( triggered( bool ) ), this, SLOT( connectAction_activated() ) );
    connect( actionDisconnect, SIGNAL( triggered( bool ) ), this, SLOT( disconnectAction_activated() ) );
    connect( actionHide, SIGNAL( triggered( bool ) ), this, SLOT( showKiaxWindow() ) );
    connect( actionShow, SIGNAL( triggered( bool ) ), this, SLOT( showKiaxWindow() ) );
    connect( actionInvite_Friend, SIGNAL( triggered( bool ) ), this, SLOT( inviteFriendAction_activated() ) );
	connect( searchComboBox, SIGNAL( editTextChanged ( QString ) ), this, SLOT (searchList( QString )) );
	connect( searchComboBox, SIGNAL( editTextChanged ( QString ) ), this, SLOT (checkComboText( QString )) );
	connect( searchComboBox, SIGNAL( activated ( QString ) ), this, SLOT (searchBoxActivated( QString )) );
#ifdef TABBED_LAYOUT
		connect( cdrSearchComboBox, SIGNAL( editTextChanged ( QString ) ), this, SLOT (searchList( QString )) );
		connect( cdrSearchComboBox, SIGNAL( editTextChanged ( QString ) ), this, SLOT (checkComboText( QString )) );
		connect( cdrSearchComboBox, SIGNAL( activated ( QString ) ), this, SLOT (searchBoxActivated( QString )) );
		connect( hangupButton, SIGNAL( clicked() ), this, SLOT ( hangupButtonClicked()));
#endif
	connect( numberEdit, SIGNAL( returnPressed () ), this, SLOT (numberEditActivated()) );
	connect( dialButton, SIGNAL( clicked () ), this, SLOT (numberEditActivated()) );
	connect( dialpadButton, SIGNAL( toggled ( bool ) ), this, SLOT (showDialpad( bool )) );
	connect( contactsList, SIGNAL( itemActivated(QListWidgetItem* ) ), this, SLOT ( callAction_activated() ) );
	connect( callRecords, SIGNAL( itemActivated(QListWidgetItem* ) ), this, SLOT ( callRecord_activated() ) );
	connect( actionExit, SIGNAL( triggered( bool ) ), this, SLOT ( exitKiax() ) );
	connect( actionSettings, SIGNAL( triggered( bool ) ), this, SLOT ( settingsAction_activated() ) );
	connect( contactsList, SIGNAL (customContextMenuRequested ( const QPoint & )), this, SLOT (showContactContextMenu(const QPoint&)));	
#ifndef REMOTE_CDR	
	connect( callRecords, SIGNAL (customContextMenuRequested ( const QPoint & )), this, SLOT (showCdrContextMenu(const QPoint&)));	
#endif
	connect( sysTray, SIGNAL( activated(QSystemTrayIcon::ActivationReason) ), this, SLOT ( sysTrayActivated(QSystemTrayIcon::ActivationReason) ) );
	connect(ringTimer, SIGNAL(timeout()), this, SLOT(ring()));
	connect(listSwitchButton, SIGNAL(toggled(bool)), this, SLOT(listSwitch()));
	connect(statusButton, SIGNAL(toggled(bool)), statusButton, SLOT(showMenu()));
	Logger::log(Logger::DEBUG, "Connecting widgets.. done\n");
	if (ENABLE_CALL_RATES)
		connect(actionCallRates, SIGNAL(triggered(bool)), this, SLOT(callRatesButtonClicked()));
	if (ENABLE_SPEEDTEST)
	{
		connect(connectionMeter, SIGNAL(downloadComplete(bool, uint)), this, SLOT (speedTestComplete(bool, uint)));
	}
	if (ENABLE_BALANCE)
	{
		connect(jsonBalance, SIGNAL(requestInitiated()), this, SLOT(jsonBalanceRequestInitiated()));
		connect(jsonBalance, SIGNAL(requestComplete(bool, QString)), this, SLOT(jsonBalanceRequestComplete(bool, QString)));
		connect(jsonBalance, SIGNAL(requestAborted()), this, SLOT(jsonRequestAborted()));
		connect(jsonBalance, SIGNAL(requestError()), this, SLOT(jsonRequestError()));
		connect(balanceTimer, SIGNAL(timeout()), this, SLOT(balanceTimeout()));
		if (BALANCE_CLICKABLE)
		connect(balanceLabel, SIGNAL(clicked()), this, SLOT(balanceLabelClicked()));
	}
	if (ENABLE_ONLINE_USERS)
	{
		connect(jsonUsersOnline, SIGNAL(requestInitiated()), this, SLOT(jsonUsersOnlineRequestInitiated()));
		connect(jsonUsersOnline, SIGNAL(requestComplete(bool, QString)), this, SLOT(jsonUsersOnlineRequestComplete(bool, QString)));
		connect(jsonUsersOnline, SIGNAL(requestAborted()), this, SLOT(jsonRequestAborted()));
		connect(jsonUsersOnline, SIGNAL(requestError()), this, SLOT(jsonRequestError()));
	}
	if (ENCODE_TEL_EXTEN)
	{
		connect(sessionTimer, SIGNAL(timeout()), this, SLOT(sessionTimeout()));		
		connect(jsonSession, SIGNAL(notificationAvailable(QString, QString)), this, SLOT(jsonSessionUpdated(QString, QString)));
	}
	if (NOTIFICATION_ENABLED)
	{
		connect(jsonNotification, SIGNAL(notificationAvailable(QString, QString)), this, SLOT(jsonNotificationAvailable(QString, QString)));
		connect(sysTray, SIGNAL(messageClicked()), this, SLOT(notificationBalloonClicked()));
		if (NOTIFICATION_SETTINGS_ENABLED) {
			if (paramRequestNotification())
			{
			connect(notificationTimer, SIGNAL(timeout()), this, SLOT(notificationTimeout()));
			}
		} else {
			connect(notificationTimer, SIGNAL(timeout()), this, SLOT(notificationTimeout()));
		}
	}
	if (ENABLE_FEED_TIMER)
	{
		connect(feedTimer, SIGNAL(timeout()), this, SLOT(updateFeedIdle()));
	}
	if (ENABLE_LOGIN_SCREEN)
	{
		connect(loginScreen, SIGNAL(account()), this, SLOT(registerAction_activated()));
		connect(loginScreen, SIGNAL(login(QString, QString)), this, SLOT(login(QString, QString)));
	}

#if !defined(TABBED_LAYOUT)	
//	if (!ENABLE_BALANCE&&(!ENABLE_CALL_RATES)&&(!ENABLE_ONLINE_USERS)) //clean unbranded kiax
	if (KIAX_MODE) //clean unbranded kiax
	{
		statusLayout->removeWidget(onlineUsersButton);
		statusLayout->removeWidget(balanceLabel);	
		mainFrameLayout->removeWidget(dialPadButtonsFrame);
		statusLayout->removeItem(onlineStatusSpacer);
		statusLayout->addWidget(dialPadButtonsFrame, 0, 1, 1, 1);
	} else 
#endif
	if (!ENABLE_BALANCE)
	{
		statusLayout->removeWidget(balanceLabel);	
	}
	if (!ENABLE_ONLINE_USERS)
	{
		statusLayout->removeWidget(onlineUsersButton);
#if !defined(TABBED_LAYOUT)
		statusLayout->removeItem(onlineStatusSpacer);
#endif
	}
	
#ifdef USE_WEBFORMS
	connect(registerDialog, SIGNAL(registrationSuccessful(QString, QString)), this, SLOT(webRegistrationSuccessful(QString, QString)));
	connect(changePasswordDialog, SIGNAL(passwordChangeSuccessful(QString, QString)), this, SLOT(webPasswordChangeSuccessful(QString, QString)));
#endif	
	Logger::log(Logger::DEBUG, "Connecting dialpad buttons..\n");
	connect(dtmf0Button, SIGNAL (clicked()), this, SLOT(dtmf0()));
	connect(dtmf1Button, SIGNAL (clicked()), this, SLOT(dtmf1()));
	connect(dtmf2Button, SIGNAL (clicked()), this, SLOT(dtmf2()));
	connect(dtmf3Button, SIGNAL (clicked()), this, SLOT(dtmf3()));
	connect(dtmf4Button, SIGNAL (clicked()), this, SLOT(dtmf4()));
	connect(dtmf5Button, SIGNAL (clicked()), this, SLOT(dtmf5()));
	connect(dtmf6Button, SIGNAL (clicked()), this, SLOT(dtmf6()));
	connect(dtmf7Button, SIGNAL (clicked()), this, SLOT(dtmf7()));
	connect(dtmf8Button, SIGNAL (clicked()), this, SLOT(dtmf8()));
	connect(dtmf9Button, SIGNAL (clicked()), this, SLOT(dtmf9()));
	connect(dtmfStarButton, SIGNAL (clicked()), this, SLOT(dtmfStar()));
	connect(dtmfHashButton, SIGNAL (clicked()), this, SLOT(dtmfHash()));
	connect(this, SIGNAL (allCallsHungup()), this, SLOT(allCallsHungupSlot()));

	Logger::log(Logger::DEBUG, "Connecting dialpad buttons..done\n");
	
	Logger::log(Logger::DEBUG, "Refreshing Contacts\n");
#ifdef USE_WEBFEED
	connect(feedWebView, SIGNAL(linkClicked(const QUrl&)), this, SLOT(feedLinkClicked(const QUrl&)));
	connect(feedWebView, SIGNAL(loadStarted()), this, SLOT(feedLoaded()));
#endif
	refreshContactList("");
	

	Logger::log(Logger::DEBUG, "Refreshing Call records done.\n");

#if defined(MACXDEP)
	setUnifiedTitleAndToolBarOnMac (true);
#endif 
	
	checkDefaultAccount();
	Logger::log(Logger::DEBUG, "Refreshing Call records\n");
	populateCallRecords("");
	serviceMenuTitle = MENU_SERVICE_LABEL;
	menuService->setTitle(MENU_SERVICE_LABEL);
	toolBar->setVisible(ENABLE_TOOLBAR);
	
	if ((ENABLE_LOGIN_SCREEN)&&(ALWAYS_USE_LOGIN_SCREEN))
	{
		disableUse();
	}
	else
	{
		if ((ENABLE_LOGIN_SCREEN)&&(readParameter(std::string("LoggedIn"), "false").compare("true")!=0))
			disableUse();
	}
	
	if (paramConnect())
	{
		QString usrname = QString::fromStdString(defaultAccount->username);
		QString passwd = QString::fromStdString(defaultAccount->password);
		
		if (ENABLE_LOGIN_SCREEN)
		{
			loginScreen->setUsername(usrname);
			loginScreen->setPassword(passwd);
		} 
			if ((!DISABLE_CONNECT)&&(usrname!="")&&(passwd!=""))
				connectAction_activated();
	}
	
		std::string paramNetworkAuto("NetworkAuto"); 
		if (readParameter(paramNetworkAuto, "true").compare("false")!=0)
			testNetwork(false);
	setCodecs();

#if !defined(TABBED_LAYOUT)
		std::string paramShowDialpadFrame("ShowDialpadFrame");
		std::string showDialpadDefault = DEFAULT_DIALPAD_OPEN ? "true" : "false";

		if (readParameter(paramShowDialpadFrame, showDialpadDefault).compare("true")==0)
		{
			showDialpad(true);
			dialpadButton->setChecked(true);
		}
		else
		{
			showDialpad(false);
			dialpadButton->setChecked(false);
		}
#else
		showDialpad(true);
		dialpadButton->setVisible(false);
		listSwitchButton->setVisible(false);
#endif
	
	if (DISABLE_DIALPAD_FEATURE)
	{
		showDialpad(false);
		dialpadButton->setVisible(false);
	}

	if (!SHOW_STATUS_BUTTON)
	{
		statusButton->setVisible(false);
	}
	else
	{
		statusButton->setVisible(true);
	}
	
    exitOnCallEnd = false;

	parseCommandParameters(argc, args);

	std::string paramMainWinOnTop("MainWinOnTop");
	setAlwaysOnTop(readParameter(paramMainWinOnTop, "false").compare("true")==0);
	std::string paramDialWinOnTop("DialWinOnTop");
	callWindowsOnTop = readParameter(paramDialWinOnTop, "true").compare("true")==0;
	
	contactsList->setFocus(Qt::OtherFocusReason);
	
		if (paramShow())
	{
		window()->setVisible(true);
		actionShow->setEnabled(false);
		actionHide->setEnabled(true);
	}
	else
	{
		actionShow->setEnabled(true);
		actionHide->setEnabled(false);
	}
#ifdef USE_WEBFEED
	updateFeedIdle();
#endif	
	restoreGeometry();
}

void Kiax2MainWindow::parseCommandParameters(int &argc, char *argv[])
{
    if (argc>1) {
        char* exten=NULL;

        char* command = argv[1]; /* command*/
        if ((strcmp(command,"dial")==0)||(strcmp(command,"--debug")==0)) {
            for (int i=2;i<argc;i++) {
                if (strcmp(argv[i],"--extension")==0) {
                    exten=argv[i+1];
                    i++;
                    Logger::log(Logger::DEBUG,"command line extension %s\n", exten);
                }
				if (strcmp(argv[i],"--exitOnEnd")==0) {
                    exitOnCallEnd = true;
                    Logger::log(Logger::DEBUG,"kiax will exit after call ends\n", exten);
                }				
            }
        } 
		else if (strstr(command,"voip://")!=0) 
		{		
			QUrl url(command);
			QString kiaxCommand = url.host(); // command is encoded as hostname
			if (kiaxCommand=="call")
			{
				QString number = url.queryItemValue("number");
				if (number!="")
				{
					exten = new char[number.toStdString().length()];
					strncpy(exten, (char*)number.toStdString().data(), number.toStdString().length());
					exten[number.toStdString().length()]=0;
				} 
			}
		}
		else {
            fprintf(stderr, "usage: kiax [dial --extension exten [--exitOnEnd] ] [--debug level]\n");
			exit(0);
        }
		
        if (exten) {
			Logger::log(Logger::DEBUG, "Dialing extension %s\n", exten);
			Contact* contact = new Contact();
			contact->setObjectId(0); 
			contact->extension = std::string(exten);
			contact->contactName = std::string(exten);
			Logger::log(Logger::DEBUG, "extension = %s, contactName = %s\n", contact->extension.data(), contact->contactName.data());
#if !defined(TABBED_LAYOUT)
			Kiax2CallDialog* callDialog = new Kiax2CallDialog(this, api, contact);
			Kiax2CallAppearance* callAppearance = callDialog;
#else
			Kiax2CallButton* callButton = new Kiax2CallButton(this, api, contact);
			Kiax2CallAppearance* callAppearance = callButton;
#endif
			//callDialog->setWindowFlags(flags);
#if !defined(TABBED_LAYOUT)
			callDialog->setAlwaysOnTop(callWindowsOnTop);
#endif
			int callNumber = api->dial(contact->extension, defaultAccount);
			callAppearance->initiateCall(CALL_DIRECTION_OUTGOING);
			Logger::log(Logger::DEBUG, "Kiax2MainWindow::openCallDialog: new call number is %d\n", callNumber);
			if (callNumber>-1)
			{
				callMap[callNumber] = callAppearance;
				callAppearance->setCallNumber(callNumber);
#if !defined(TABBED_LAYOUT)				
				callAppearance->appearance->show();
				callAppearance->appearance->raise();
#else
				Logger::log(Logger::DEBUG, "adding button to exclusive button group\n");
				callButtonsGroup->addButton((QPushButton*)callAppearance->appearance);
				Logger::log(Logger::DEBUG, "adding button to frame layout\n");
				((QBoxLayout*)callsFrame->layout())->addWidget(callAppearance->appearance);
				((QPushButton*)callAppearance->appearance)->setChecked(true);
				Logger::log(Logger::DEBUG, "added button to frame layout\n");
				tabWidget->setCurrentIndex(0);
#endif
			}
        }

	} 
}	

void Kiax2MainWindow::changeEvent(QEvent* event)
{

}

void Kiax2MainWindow::setAlwaysOnTop(bool always)
{
        if (always)
        {
                Qt::WindowFlags winFlags = flags;
                winFlags |= Qt::WindowStaysOnTopHint;
                setWindowFlags(winFlags);

        }
        else
        {
                setWindowFlags(flags);
        }
}



void Kiax2MainWindow::login(QString username, QString password)
{
	if ((username!="")&&(password!=""))
	{
		vector<Account*> accountsResult = api->getAccounts();
		if (accountsResult.size()>0)
		{
			accountsResult[0]->username = username.toStdString();
			accountsResult[0]->password = password.toStdString();
			api->updateObject(accountsResult[0]);
		}
		else
		{
			Account* account = new Account();
			account->username = username.toStdString();
			account->password = password.toStdString();
			account->host1 = DEFAULT_REGISTER_HOST;
			account->host2 = SECOND_REGISTER_HOST;
			account->host3 = THIRD_REGISTER_HOST;
			account->serviceName = DEFAULT_SERVICE_NAME;
			api->updateObject(account);
			delete account;
			account = NULL;
		}
		
		checkDefaultAccount();
		reconnect();
		loginScreen->setLoginStatus(tr("Logging in.."));		
	}
}

void Kiax2MainWindow::enableUse()
{
	if (!ALLOW_CALLS_WITH_NO_ACC) 
	{
		mainWidgetFrame->setEnabled(true);
		actionAdd_Contact->setEnabled(true);
		actionAccount->setEnabled(true);
		actionAccount->setVisible(true);
	}	
	if (ENABLE_LOGIN_SCREEN)
	{ 
		loginScreen->setVisible(false);
		mainWidgetFrame->setVisible(true);
		mainLayout->removeWidget(loginScreen);
		mainLayout->addWidget(mainWidgetFrame, 0, 0, 1, 1);	
		onlineStatusFrame->setVisible(true);
		dialPadButtonsFrame->setVisible(true);
	}
}

void Kiax2MainWindow::disableUse()
{
	if (!ALLOW_CALLS_WITH_NO_ACC)
	{ 
		mainWidgetFrame->setEnabled(false);
		actionAccount->setEnabled(true);
		actionConnect->setEnabled(false);
		if (SINGLE_CONNECT_DISCONNECT)
		{
			actionConnect->setVisible(false);
		}
		actionAccount->setVisible(true);
		actionAdd_Contact->setEnabled(false);
	}
	if (ENABLE_LOGIN_SCREEN)
	{
		loginScreen->setLoginStatus("");
		loginScreen->setVisible(true);
		mainWidgetFrame->setVisible(false);
		mainLayout->removeWidget(mainWidgetFrame);
		mainLayout->addWidget(loginScreen, 0, 0, 1, 1);
		onlineStatusFrame->setVisible(false);
		dialPadButtonsFrame->setVisible(false);
	}		
}

void Kiax2MainWindow::i18n(QString localeStr) {
    if (appTranslator) {
        application->removeTranslator(appTranslator);
        delete appTranslator;
        appTranslator = NULL;
    }

    appTranslator = new QTranslator(0);
    appTranslator->load( QString( "kiax2_" ) + localeStr, "./" );
    application->installTranslator(appTranslator);
}

#ifdef USE_WEBFEED
void Kiax2MainWindow::feedLinkClicked(const QUrl& link)
{
	Logger::log(Logger::DEBUG, "feed link clicked %s\n", link.toString().toStdString().data());
	if (link.toString().startsWith("httplocal://"))
	{
		QString linkStr = link.toString();
		linkStr.replace(QString("httplocal://"), QString("http://"));
		Logger::log(Logger::DEBUG, "opening url in webkit %s\n", linkStr.toStdString().data());
		feedWebView->load(QUrl(linkStr));
	} else
	{
		Logger::log(Logger::DEBUG, "opening url in external browser %s\n", link.toString().toStdString().data());
		QDesktopServices::openUrl(link);
	}
}

void Kiax2MainWindow::feedLoaded()
{
	Logger::log(Logger::DEBUG, "feed loading ..\n");
	feedWebView->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
}
#endif

void Kiax2MainWindow::setDefaultCodec(int defaultCodec)
{
		switch (defaultCodec)
		{
			case 0:
			{
				iaxc_set_formats(IAXC_FORMAT_ULAW,IAX2_CAPABILITIES_ULAW);
				break;
			}
			case 1:
			{
				iaxc_set_formats(IAXC_FORMAT_SPEEX,IAX2_CAPABILITIES_SPEEX);
				break;
			}		
			case 2:
			{
				iaxc_set_formats(IAXC_FORMAT_GSM,IAX2_CAPABILITIES_GSM);
				break;
			}		
			case 3:
			{
				iaxc_set_formats(IAXC_FORMAT_ILBC,IAX2_CAPABILITIES_ILBC);
				break;
			}		
		}		
}

void Kiax2MainWindow::httpConfigureAccount(QString username, QString password, QString host1, QString host2)
{
	Logger::log(Logger::DEBUG, "httpConfigureAccount() : Configuration request received: username=%s, password=%s, host1=%s, host2=%s\n",
								username.toStdString().data(),
								password.toStdString().data(),
								host1.toStdString().data(),
								host2.toStdString().data());
	setVisible(true);
	raise();
	updateIdentity(username, password, host1, host2);
}

void Kiax2MainWindow::httpChangePassword(QString username, QString password)
{
	Logger::log(Logger::DEBUG, "httpConfigureAccount() : Configuration request received: username=%s, password=%s\n",
								username.toStdString().data(),
								password.toStdString().data());

	updateIdentity(username, password);
}

void Kiax2MainWindow::setCodecs()
{
	std::string paramCodec("DefaultCodec");
	int defaultCodec = strToInt(readParameter(paramCodec, "1"));
	saveParameter(paramCodec, intToStr(defaultCodec));
	Logger::log(Logger::DEBUG, "Setting user prefered codec %d\n", defaultCodec);
	setDefaultCodec(defaultCodec);
}

void Kiax2MainWindow::adjustDevices()
{

  struct iaxc_audio_device *devices;
  struct iaxc_audio_device *device;
  int devNumber,input,output,ring;
  std::map<int, std::string> deviceMap;
  
  std::string paramOutputIndex("OutputDeviceIndex");
  std::string paramOutputName("OutputDeviceName");
  std::string paramInputIndex("InputDeviceIndex");
  std::string paramInputName("InputDeviceName");
  std::string paramRingIndex("RingDeviceIndex");
  std::string paramRingName("RingDeviceName");  
  std::string paramDeviceHash("DeviceHash");  
  
  iaxc_audio_devices_get(&devices, &devNumber, &input, &output, &ring);
  device = devices;
  for(int i=0; i<devNumber; i++)
  {
    std::string devName(device->name);
	deviceMap[i] = devName;
    device++;
  }

  std::string outputName = readParameter(paramOutputName);
  int outputIndex = strToInt(readParameter(paramOutputIndex));
  std::string inputName = readParameter(paramInputName);
  int inputIndex = strToInt(readParameter(paramInputIndex));
  std::string ringName = readParameter(paramRingName);
  int ringIndex = strToInt(readParameter(paramRingIndex));
  QString devHash = QString::fromStdString(readParameter(paramDeviceHash,""));

  QString currentDevHash = deviceHash(deviceMap);
  long devCapabilities;
  if ((deviceMap[inputIndex].compare(inputName)==0)&&
		(deviceMap[outputIndex].compare(outputName)==0)&&
		(deviceMap[ringIndex].compare(ringName)==0)&&(currentDevHash==devHash))
  {
	iaxc_audio_devices_set(inputIndex, outputIndex, ringIndex);
  }
  else // there is change in device order
  {
	// check if there are usb devices
		device = devices;
	  for(int i=0; i<devNumber; i++)
	  {
	    devCapabilities = device->capabilities;
	    QString devName(device->name);
	    if(devCapabilities & IAXC_AD_INPUT) 
		{	
			if (containsUsb(devName))
				inputIndex = i;
		}
	    if(devCapabilities & IAXC_AD_OUTPUT) 
		{
			if (containsUsb(devName))
				outputIndex = i;
		}
	    if(devCapabilities & IAXC_AD_RING)
		{
			if (containsUsb(devName))
				ringIndex = i;
		}
	    device++;
	  }
	  iaxc_audio_devices_set(inputIndex, outputIndex, ringIndex);

  }

  	saveParameter(paramInputIndex, intToStr(inputIndex));
	saveParameter(paramInputName, deviceMap[inputIndex]);
	saveParameter(paramOutputIndex, intToStr(outputIndex));
	saveParameter(paramOutputName, deviceMap[outputIndex]);
	saveParameter(paramRingIndex, intToStr(ringIndex));
	saveParameter(paramRingName, deviceMap[ringIndex]);
    saveParameter(paramDeviceHash, currentDevHash.toStdString());
}

QString Kiax2MainWindow::deviceHash(std::map<int, std::string> devices)
{
	QString devConcat;
	QCryptographicHash hash(QCryptographicHash::Md5);
	for (int i=0;i< devices.size();i++)
	{
		devConcat+=QString::fromStdString(devices[i]);
	}
	hash.addData(devConcat.toStdString().data());
	QByteArray ba = hash.result();	
	QString md5sum(ba.toHex());	

	return md5sum;
}

bool Kiax2MainWindow::containsUsb(QString name)
{
	QString pattern = QString::fromStdString(USB_NAME_PATTERN);
	QRegExp rx(pattern);	
	rx.setCaseSensitivity(Qt::CaseInsensitive);
	if (!rx.isValid())
		return false;
	else
	{
		int index = rx.indexIn(name);
		return (index>-1);
	}
	return false;
}

bool Kiax2MainWindow::paramShow()
{
	std::string paramStr = readParameter(std::string("ShowMainWindow"), "true");
	if (paramStr.compare("")==0) 
		return true;
	else 
		return (paramStr.compare("false")!=0); 
}

bool Kiax2MainWindow::paramConnect()
{
	std::string paramStr = readParameter(std::string("ConnectOnStart"), "true");
	if (paramStr.compare("")==0) 
		return true;
	else 
		return (paramStr.compare("false")!=0);
}

bool Kiax2MainWindow::paramRequestNotification()
{
	std::string paramStr = readParameter(std::string("RequestVersionCheck"), "true");
	if (paramStr.compare("")==0) 
		return true;
	else 
		return (paramStr.compare("false")!=0);
}

void Kiax2MainWindow::saveGeometry()
{
	saveParameter(std::string("GeometryX"), QString::number(mainWindowGeometry.x()).toStdString());
	saveParameter(std::string("GeometryY"), QString::number(mainWindowGeometry.y()).toStdString());
	saveParameter(std::string("GeometryW"), QString::number(mainWindowGeometry.width()).toStdString());
	saveParameter(std::string("GeometryH"), QString::number(mainWindowGeometry.height()).toStdString());
}

void Kiax2MainWindow::restoreGeometry()
{
	QRect newGeometry;
	std::string geomX = readParameter(std::string("GeometryX"), intToStr(DEFAULT_WINDOW_POSITION_X));
	int geomXInt = QString::fromStdString(geomX).toInt(); 
	std::string geomY = readParameter(std::string("GeometryY"), intToStr(DEFAULT_WINDOW_POSITION_Y));
	int geomYInt = QString::fromStdString(geomY).toInt();
	std::string geomW = readParameter(std::string("GeometryW"), intToStr(DEFAULT_WINDOW_SIZE_WIDTH));
	int geomWInt = QString::fromStdString(geomW).toInt();
	std::string geomH = readParameter(std::string("GeometryH"), intToStr(DEFAULT_WINDOW_SIZE_HEIGHT));
	int geomHInt = QString::fromStdString(geomH).toInt();
	newGeometry.setX(geomXInt);
	newGeometry.setY(geomYInt);
	newGeometry.setWidth(geomWInt);
	newGeometry.setHeight(geomHInt);
	this->setGeometry(newGeometry);
	mainWindowGeometry = newGeometry;
}

Kiax2MainWindow::~Kiax2MainWindow()
{
	Core::destroy();
	if (ENABLE_SPEEDTEST)
	{
		delete connectionMeter;
	}
#ifdef USE_WEBFORMS	
	delete registerDialog;
	delete changePasswordDialog;
#endif	
	delete contactContextMenu;
	delete sysTrayMenu;
	delete sysTray;
	delete ringTimer;
	if (ENCODE_TEL_EXTEN)
	{
		delete sessionTimer;
		delete jsonSession;
	}
	if (ENABLE_BALANCE)
	{
		delete jsonBalance;
		delete balanceTimer;
	}
	if (ENABLE_ONLINE_USERS)
	{
		delete jsonUsersOnline;
	}	
	if (NOTIFICATION_ENABLED)
	{
		delete jsonNotification;
	}
	if (ENABLE_FEED_TIMER)
	{
		delete feedTimer;
	}
	delete defaultAccount;
	if (ENABLE_CALL_RATES)
		delete actionCallRates;
	if (ENABLE_LOGIN_SCREEN)
		delete loginScreen;
}

QString Kiax2MainWindow::readFeedUrl()
{
	std::string paramStr = readParameter(std::string("FeedUrl"), IDLE_FEED_URL);
	return QString::fromStdString(paramStr);
}

void Kiax2MainWindow::saveFeedUrl(QString url)
{
	saveParameter(std::string("FeedUrl"), url.toStdString());
}

void Kiax2MainWindow::updateFeedIdle()
{
#ifdef USE_WEBFEED
		if (callMap.size()>0)
		{
			return;
		}
		QString url(IDLE_FEED_URL);
		saveParameter(std::string("IdleFeedUrl"), url.toStdString());
		Logger::log(Logger::DEBUG, "Updating call feed (idle) with url %s\n", url.toStdString().data());
		feedWebView->load(QUrl(url));
#endif
}

void Kiax2MainWindow::updateFeed(QString host, QString exten)
{
#ifdef USE_WEBFEED
	QString url(CALL_FEED_URL);
	url += "?telhost=" + host + "&exten=" + exten;
	Logger::log(Logger::DEBUG, "Updating call feed with url %s\n", url.toStdString().data());
	saveParameter(std::string("CallFeedUrl"), url.toStdString());
	feedWebView->load(QUrl(url));
#endif
}

void Kiax2MainWindow::appendNumberToNumberEdit(QString number)
{
	if (numberEdit->text()==tr("Type a number to dial.."))
	{
		numberEdit->setText("");
	}
	
	QString editNumber = numberEdit->text();
	editNumber = editNumber + number;
	numberEdit->setText(editNumber);
	
}

void Kiax2MainWindow::balanceLabelClicked()
{
	if (ENABLE_BALANCE)
	{
		Logger::log(Logger::DEBUG, "balanceLabel clicked.\n");
		if (balanceLabel->text()!="")
		{

			uint currentTime = QDateTime::currentDateTime().toTime_t();
			if (currentTime>lastBalanceUpdateByClick+60)
			{
				jsonBalance->getJSONData(jsonBalanceRequestUrl);
				//jsonUsersOnline->getJSONData(jsonUsersOnlineRequestUrl);
				lastBalanceUpdateByClick = currentTime;
			}

		QString loginUrl = QString(LOGIN_URL) + "?username=" + QString::fromStdString(defaultAccount->username);
		if (SUBMIT_BALANCE_PASSWORD)
			loginUrl +=	"&password=" + QString::fromStdString(defaultAccount->password);
			
		Logger::log(Logger::DEBUG, "Opening URL %s\n", loginUrl.toStdString().data());
			QDesktopServices::openUrl(QUrl(loginUrl));	
		}
	}
}

void Kiax2MainWindow::balanceMenuClicked()
{
	if (ENABLE_BALANCE)
	{

		QString loginUrl = QString(LOGIN_URL) + "&username=" + QString::fromStdString(defaultAccount->username);
		if (SUBMIT_BALANCE_PASSWORD)
			loginUrl +=	"&password=" + QString::fromStdString(defaultAccount->password);
			
		Logger::log(Logger::DEBUG, "Opening URL %s\n", loginUrl.toStdString().data());
			QDesktopServices::openUrl(QUrl(loginUrl));	
	}
}

void Kiax2MainWindow::webRegistrationSuccessful(QString username, QString password)
{
	updateIdentity(username, password);
	Logger::log(Logger::DEBUG, "Kiax2MainWindow::registrationSuccessful, username = %s, password = %s", username.toStdString().data(), password.toStdString().data());
}

void Kiax2MainWindow::webPasswordChangeSuccessful(QString username, QString password)
{
	updateIdentity(username, password);
	Logger::log(Logger::DEBUG, "Kiax2MainWindow::passwordChangeSuccessful, username = %s, password = %s", username.toStdString().data(), password.toStdString().data());
}

void Kiax2MainWindow::updateIdentity(QString username, QString password, QString host1, QString host2)
{
	Logger::log(Logger::DEBUG, "updateIdentityu IN\n");
	vector<Account*> accountsResult = api->getAccounts();
	if (accountsResult.size()>0) 
	{
		int qresult = QMessageBox::Yes;
		if (accountsResult[0]->username.compare(GUEST_ACCOUNT)!=0)
		{
			qresult = QMessageBox::question(this, "Configuration Request", "Softphone identity configuration requested. Apply new settings?", QMessageBox::Yes | QMessageBox::No);
		}
		if (qresult == QMessageBox::Yes)
		{
			accountsResult[0]->username = username.toStdString();
			accountsResult[0]->password = password.toStdString();
			if (host1!="")
				accountsResult[0]->host1 = host1.toStdString();
			if (host2!="")
				accountsResult[0]->host2 = host2.toStdString();
			api->updateObject(accountsResult[0]);
			QMessageBox::information( this, DEFAULT_SERVICE_NAME,
                   tr("Account details changed."));
			checkDefaultAccount();
		}

	}
	else
	{
		Account* account = new Account();
		account->username = username.toStdString();
		account->password = username.toStdString();
		if (host1!="")
		{
			account->host1 = host1.toStdString();
		}
		else
		{
			account->host1 = DEFAULT_REGISTER_HOST;
		}
		if (host2!="")
		{
			account->host2 = host2.toStdString();
		}
		else
		{
			account->host2 = SECOND_REGISTER_HOST;
		}
		account->host3 = THIRD_REGISTER_HOST;
		account->serviceName = DEFAULT_SERVICE_NAME;
		api->updateObject(account);
		delete account;
		checkDefaultAccount();
	}

	reconnect(); 
	Logger::log(Logger::DEBUG, "updateIdentityu OUT\n");
}

void Kiax2MainWindow::reconnect()
{
	if (!DISABLE_CONNECT)
	{	
		disconnectAction_activated();
		connectAction_activated();
	}
	updateBalanceParams();
	balanceTimeout();
	notificationTimeout();
	reloadJSONServers();
}

QString Kiax2MainWindow::generateSession()
{
	QString result = QString::number(rand(), 16);
	if (ENCODE_TEL_EXTEN)
	{
		QString sessionUpdateUrl = QString(SESSION_REQUEST_URL) + "/" + result;
		if (ENABLE_SESSION_UPDATE)
		{
			jsonSession->getJSONData(QString(sessionUpdateUrl));
			Logger::log(Logger::DEBUG, "Session update URL is:%s\n", sessionUpdateUrl.toStdString().data());
		}
	}

	return result;
}

void Kiax2MainWindow::balanceTimeout()
{
	if (ENABLE_BALANCE)
	{
		if (callMap.size()<1)
		{
			jsonBalance->getJSONData(jsonBalanceRequestUrl);
			//jsonUsersOnline->getJSONData(jsonUsersOnlineRequestUrl);
		}
		else
			balanceTimer->stop();
	}

}

void Kiax2MainWindow::sessionTimeout()
{
	encodeSession = generateSession();
}

void Kiax2MainWindow::notificationTimeout()
{
	if (NOTIFICATION_ENABLED)
	{
		if (NOTIFICATION_SETTINGS_ENABLED)
		{
			if (paramRequestNotification())
			{
			jsonNotification->setUsername(QString(defaultAccount->username.data()));			    				
			jsonNotification->setPassword(QString(defaultAccount->password.data()));
			jsonNotification->getJSONData(QString(NOTIFICATION_REQUEST_URL));
			}
		}
		else 
		{
			jsonNotification->setUsername(QString(defaultAccount->username.data()));			    				
			jsonNotification->setPassword(QString(defaultAccount->password.data()));
			jsonNotification->getJSONData(QString(NOTIFICATION_REQUEST_URL));
		}
	}
}

void Kiax2MainWindow::ring()
{
	 ringInTone = initTone(1900, 2400, 400,500,20);
	 iaxc_play_sound(ringInTone, 1);
}

void Kiax2MainWindow::startRing()
{
  ring(); // start ringing, the timer will wait and then ring again
  ringTimer->start(4000);
}

void Kiax2MainWindow::stopRing()
{
  /* In case there was no ring signal.. we have to check*/
  if (ringInTone!= NULL) {
  	iaxc_stop_sound(ringInTone->id);
	delete ringInTone;
  }
  ringTimer->stop();
  ringInTone=NULL;
}

iaxc_sound* Kiax2MainWindow::initTone(int F1, int F2, int Dur, int Len, int Repeat)
{
  iaxc_sound* tone;
  tone = (iaxc_sound *)malloc(sizeof(iaxc_sound));
  tone->channel = 1;
  tone->len  = Len;
  tone->data = (short *)calloc(tone->len , sizeof(short));


  for( int i=0;i < Dur; i++ )
  {
    tone->data[i] = (short)(0x7fff*0.4*sin((double)i*F1*M_PI/8000))
                   + (short)(0x7fff*0.4*sin((double)i*F2*M_PI/8000));
  }

  tone->repeat = Repeat;
  return tone;

}

void Kiax2MainWindow::closeEvent(QCloseEvent* event)
{
#if !defined(MACXDEP)
	showKiaxWindow();
	event->accept();
#else
	mainWindowGeometry = geometry();
	setVisible(false);
	event->ignore();
#endif	
}

void Kiax2MainWindow::sysTrayActivated(QSystemTrayIcon::ActivationReason reason) 
{
#if !defined(MACXDEP)
	if (reason == QSystemTrayIcon::Trigger)
	{
		showKiaxWindow();
	}
#endif
}

void Kiax2MainWindow::listSwitch()
{
#if !defined(TABBED_LAYOUT)
		QGridLayout* mainLayout = gridLayout;
		if (contactsList->isVisible())
		{
			contactsList->setVisible(false);
			if (searchComboBox->lineEdit()->text()==tr("Search contacts.."))
				searchComboBox->lineEdit()->setText(tr("Search call history.."));	
			listSwitchButton->setFocus();
			mainLayout->removeWidget(contactsList);
			mainLayout->addWidget(callRecords, 1, 0, 1, 1);		
			listSwitchButton->setIcon(QIcon("icons/contacts.png"));
			listSwitchButton->setToolTip(tr("Click to view contact list"));
			callRecords->setVisible(true);
			callRecords->setFocus();
			if (searchComboBox->lineEdit()->text()!=tr("Search call history.."))
				refreshCallRecords(searchComboBox->lineEdit()->text());
		}
		else
		{
			contactsList->setVisible(true);
			callRecords->setVisible(false);
			if (searchComboBox->lineEdit()->text()==tr("Search call history.."))
			{
				searchComboBox->lineEdit()->setText(tr("Search contacts.."));		
				refreshContactList("");
			}else
				refreshContactList(searchComboBox->lineEdit()->text());
			
			listSwitchButton->setFocus();
			listSwitchButton->setIcon(QIcon("icons/calls.png"));	
			listSwitchButton->setToolTip(tr("Click to view call history"));
			mainLayout->addWidget(contactsList, 1, 0, 1, 1);
			contactsList->setFocus();
		}
#endif
}

void Kiax2MainWindow::showDialpad(bool visible)
{	
	dialpadFrame->setVisible(visible);
	dialpadButton->setText(visible?HIDE_DIALPAD_TEXT:SHOW_DIALPAD_TEXT);
}  

void Kiax2MainWindow::showKiaxWindow()
{
	if (window()->isVisible())
	{
		mainWindowGeometry = this->geometry();
		window()->setVisible(false);
		actionHide->setEnabled(false);
		actionShow->setEnabled(true);
	}
	else
	{
		window()->setVisible(true);
		this->setGeometry(mainWindowGeometry);
		window()->showNormal();
		actionHide->setEnabled(true);
		actionShow->setEnabled(false);		
	}
}

void Kiax2MainWindow::applicationActivated()
{
	if (!isVisible())
	{
		setGeometry(mainWindowGeometry);
		setVisible(true);
	}
}

void Kiax2MainWindow::widgetFocusChanged(QWidget* old, QWidget* now)
{
	if (old!=NULL)
		Logger::log(Logger::DEBUG, "Old focus was %s\n", old->objectName().toStdString().data());
#if !defined(TABBED_LAYOUT)		
		if ((now==searchComboBox)&&((old!=callRecords)||(callRecords->isVisible())))
		{
			searchComboBox->lineEdit()->setText("");
		}
#else
		if (now==searchComboBox)
		{
			searchComboBox->lineEdit()->setText("");
		} else
		if (now==cdrSearchComboBox)
		{
			cdrSearchComboBox->lineEdit()->setText("");
		} else
		if (now==numberEdit)
		{
			numberEdit->setText("");
		}
		
#endif
#if !defined(TABBED_LAYOUT)
	else if ((now==numberEdit)&&((old!=contactsList)||(contactsList->isVisible())))
	{
		numberEdit->setText("");
	}
	else 
	{
		if (contactsList->isVisible()&&(searchComboBox->lineEdit()->text()==""))
			searchComboBox->lineEdit()->setText(tr("Search contacts.."));
#else
	else {	
		if ((tabWidget->currentIndex()==1)&&(searchComboBox->lineEdit()->text()==""))
			searchComboBox->lineEdit()->setText(tr("Search contacts.."));
		if ((tabWidget->currentIndex()==2)&&(cdrSearchComboBox->lineEdit()->text()==""))
			cdrSearchComboBox->lineEdit()->setText(tr("Search call history.."));
#endif
#if !defined(TABBED_LAYOUT)
		else
			if (searchComboBox->lineEdit()->text()=="")
				searchComboBox->lineEdit()->setText(tr("Search call history.."));

		if (numberEdit->text()=="")
			numberEdit->setText("Type a number to dial..");
#endif
	}
}

void Kiax2MainWindow::searchBoxActivated(QString number)
{
#ifdef USE_DOTTEL	
	QString tNumber = number;	
	if (ALLOW_ONLY_TEL&&(!number.endsWith(".tel", Qt::CaseInsensitive)))
	{
		tNumber += ".tel";
		searchComboBox->lineEdit()->setText(tNumber);
	}
	if (tNumber.endsWith(".tel", Qt::CaseInsensitive))
	{
		QString telNumber = tNumber;
		tNumber = openDotTelDialog(tNumber);
		if (tNumber==telNumber) return; //dont allow further dialing of .tel
		searchComboBox->lineEdit()->setText(tNumber);
	}
#endif

	openCallDialog(contactsList);
}

#ifdef USE_DOTTEL	

QString Kiax2MainWindow::encodeExtension(QString mode, QString extension, QString host, QString session)
{
	Logger::log(Logger::DEBUG, "encoding extension %s for host %s..\n", extension.toStdString().data(), host.toStdString().data());
	QString telhost;	
	QString fullExtension = extension;
	QCryptographicHash hash(QCryptographicHash::Md5);
	if (mode=="mixvoip")
	{
		if (host.endsWith(".tel"))
			telhost=host.remove(".tel", Qt::CaseInsensitive); // remove .tel ending
		// $domain##$number##$hash##$session
		QString rawData = telhost+extension+"-mixvoip";
		hash.reset();
		hash.addData(rawData.toStdString().data());
		QByteArray ba = hash.result();	
		QString md5sum(ba.toHex());	
		fullExtension = telhost + "##" + extension + "##" + md5sum + "##" + session;
		Logger::log(Logger::DEBUG, ".tel extension is: %s\n", telhost.toStdString().data());
	}
	Logger::log(Logger::DEBUG, "encoding extension %s for host %s done!\n", extension.toStdString().data(), host.toStdString().data());
	return fullExtension;
}

QString Kiax2MainWindow::openDotTelDialog(QString number)
{
		vector<CppTelRecord> numbers;
		vector<CppTelRecord> descriptions;
		CppDotTelUtils dottel;
		dottel.init();
		int length = 0;
			Kiax2DotTelDialog* dottelDialog = new Kiax2DotTelDialog(this);
			dottelDialog->setModal(true);
			numbers = dottel.getNumbers(number.toStdString());
			length = numbers.size();
			if (length>0)
			{
				for (int i=0; i<length; i++)
				{
					Logger::log(Logger::DEBUG, "Discovered telephone number: %s\nTotal number of tels is %d\n", numbers[0].content.data(), length);
					QString p_type = QString::fromStdString(dottel.phTypeToStr(numbers[i].ph_type));
					QString description = QString::fromStdString(numbers[i].descriptor);
					QString number = QString::fromStdString(numbers[i].content);
					QString nstring = number + " (" + p_type + ")";
					if (description!="")
						nstring += ", " + description;
					dottelDialog->addTelNumber(number, nstring);
				}
			}
			descriptions = dottel.getDescriptions(number.toStdString());
			dottel.destroy();
			length=descriptions.size();
			if (length>0)
			{
				for (int i=0; i<length; i++)
				{
					Logger::log(Logger::DEBUG, "Discovered tel descriptions: %s\nTotal number of descriptions is %d\n", descriptions[i].content.data(), length);
					dottelDialog->addTelDescription(QString::fromStdString(descriptions[i].content));
				}
			}
			if ( dottelDialog->exec() == QDialog::Accepted )
			{
				QString selectedNumber = dottelDialog->getNumber();
				if(ENCODE_TEL_EXTEN)
				{
					selectedNumber = encodeExtension(TEL_EXTEN_ENCODING, selectedNumber, number, encodeSession);
				}
				return selectedNumber;

			}
			delete dottelDialog;
		return number;
}
#endif		

void Kiax2MainWindow::numberEditActivated()
{	
	QString number = numberEdit->text();

#ifdef USE_DOTTEL	
	if (number.endsWith(".tel", Qt::CaseInsensitive))
	{
		QString telNumber = number;
		number = openDotTelDialog(number);
		if (number==telNumber) return; //dont allow further dialing of .tel
	}
#endif

#if !defined(TABBED_LAYOUT)
	if ((number!="")&&(number!=tr("Type a number to dial.."))) {
		Contact* contact = new Contact();
		contact->setObjectId(0);
		QString name = number;
		if ((ENCODE_TEL_EXTEN)&&(number.contains("##")))
		{
			name = decodeExtension(name, true, false);
		}
		contact->contactName = name.toStdString();
		contact->extension = number.toStdString();
		openCallDialog(contact);
	}
#else
	bool answer = false;
	
	if (callMap[api->getActiveCallNumber()])
		if (!callMap[api->getActiveCallNumber()]->callAnswered)
			answer = true;
	if(!answer)
	{
		if (tabWidget->currentIndex()==0)
		{
			if ((number!="")&&(number!=tr("Type a number to dial.."))) {
				Contact* contact = new Contact();
				contact->setObjectId(0);
				contact->contactName = number.toStdString();
				contact->extension = number.toStdString();
				openCallDialog(contact);
			}
		} 
		else 
			if (tabWidget->currentIndex()==1)
			{
					if (contactsList->currentRow()>-1)
						openCallDialog(contactsList);
			}
			else
			{ 
				if (callRecords->currentRow()>-1)
					openCallDialog(callRecords);
		
			}
	} else
	{
		api->answer(api->getActiveCallNumber());
		if (callMap[api->getActiveCallNumber()])
			((QPushButton*)callMap[api->getActiveCallNumber()]->appearance)->setChecked(true);
	}
			
#endif
}

#ifdef TABBED_LAYOUT
void Kiax2MainWindow::hangupButtonClicked()
{
	int activeCall = api->getActiveCallNumber();
	Logger::log(Logger::DEBUG, "Hanging up call %d\n", activeCall);
	if (callMap[activeCall])
		if (callMap[activeCall]->callAnswered)
			api->hangup(activeCall);
		else
			api->reject(activeCall);
}
#endif

void Kiax2MainWindow::httpCall(QString number)
{
		Contact* contact = new Contact();
		contact->setObjectId(0);
		contact->contactName = number.toStdString();
		contact->extension = number.toStdString();
		openCallDialog(contact, defaultAccount);
}

void Kiax2MainWindow::httpCallWithAccount(QString number, Account* account)
{
		Contact* contact = new Contact();
		contact->setObjectId(0);
		contact->contactName = number.toStdString();
		contact->extension = number.toStdString();
		openCallDialog(contact, account);
		delete account;
}

void Kiax2MainWindow::httpHangup()
{
	int callNumber = api->getActiveCallNumber();
	Kiax2CallAppearance* call = callMap[callNumber];
	if (call)
		api->hangup(callNumber);
}

void Kiax2MainWindow::httpHangupAndCall(QString number)
{
	httpHangupAll();
	httpCall(number);
}

void Kiax2MainWindow::httpHangupAll()
{
	Logger::log(Logger::DEBUG, "httpHangupAll IN\n");
	map<int, Kiax2CallAppearance*>::const_iterator itr;
	Logger::log(Logger::DEBUG, "callMap has size: %d\n", callMap.size());
	iaxc_dump_all_calls();
	for(itr = callMap.begin(); itr != callMap.end(); ++itr){
		int activeCallNumber = itr->first;
		if (callMap.count(activeCallNumber)>0)
		{
			Logger::log(Logger::DEBUG, "about to close callNumber %d\n", activeCallNumber);
			Kiax2CallAppearance* appearance = (Kiax2CallAppearance*)itr->second;
			if (appearance!=NULL) 
			{
				api->setActiveCall(activeCallNumber);
				appearance->closeAppearance();
			}
		}
	}
	Logger::log(Logger::DEBUG, "httpHangupAll OUT\n");
}

void Kiax2MainWindow::queueCall(QString number, Account* account)
{
	Account* acc = defaultAccount;
	if (account!=NULL)
		acc = account;
	Contact* contact = new Contact();
	contact->setObjectId(0);
	contact->contactName = number.toStdString();
	contact->extension = number.toStdString();
	contact->account = acc;
	callQueue.append(contact);
}

void Kiax2MainWindow::allCallsHungupSlot()
{
	Logger::log(Logger::DEBUG, "allCallsHungupSlot() all calls hungup.\n");
	int queueLength = callQueue.size();
	if (queueLength>0) {
		for (int i=0;i<queueLength;i++)
		{
			Contact* contact = callQueue.at(i);
			if (contact!=NULL)
			{
				Logger::log(Logger::DEBUG, "allCallsHungupSlot() callQueue size=%d\n", callQueue.size());
				openCallDialog(contact, contact->account);
			}
		}
	}
	callQueue.clear();
}

void Kiax2MainWindow::checkComboText(QString text)
{
	if ((text=="")&&(!(QApplication::focusWidget()==searchComboBox))) 
	{
		searchComboBox->lineEdit()->setText(tr("Search contacts.."));
	}
}

void Kiax2MainWindow::searchList(QString searchFilter)
{
	if ((searchFilter!=tr("Search contacts.."))&&(searchFilter!=tr("Search call history..")))
	{
		if (contactsList->isVisible())
			refreshContactList(searchFilter);
		else
			refreshCallRecords(searchFilter);
	}
}

void Kiax2MainWindow::refreshContactList(QString searchFilter)
{
	contactsList->clear();
	std::string filter = searchFilter.toStdString();
	Logger::log(Logger::DEBUG, "Contacts List filter is %s\n", filter.data());
	vector<Contact*> result = api->getContacts(filter);
	for (unsigned int i = 0; i<result.size(); i++) {
		Contact* contact = result[i];
		Kiax2ListWidgetItem* listWidgetItem = new Kiax2ListWidgetItem(contactsList, 1001, contact->getObjectId());
		QString contactName(contact->contactName.data());
		listWidgetItem->setText(contactName);
		contactsList->addItem(listWidgetItem);
	}
}

void Kiax2MainWindow::refreshCallRecords(QString searchFilter)
{
/*	if ((searchFilter!="")&&(searchFilter!=tr("Search call history..")))
	{
		QList<QListWidgetItem *> searchset = callRecords->findItems(searchFilter, Qt::MatchContains);
		vector<QListWidgetItem*> items;
        for (int i = 0; i < searchset.size(); ++i) {
             QListWidgetItem* pulledItem = callRecords->takeItem(callRecords->row(searchset.at(i)));
			 items.push_back(pulledItem);
        }		
		callRecords->clear();
        for (int i = 0; i < items.size(); ++i) {
			 QListWidgetItem* item = items[i];
			 callRecords->addItem(item);
        }
	}
	else
	{
		populateCallRecords(searchFilter);
	}
*/
		populateCallRecords(searchFilter);
}

void Kiax2MainWindow::populateCallRecords(QString searchFilter)
{
	if (Core::configuration["username"]!="")
	{
		callRecords->clear();
		std::string filter = searchFilter.toStdString();
		Logger::log(Logger::DEBUG, "Call records filter is %s\n", filter.data());
		vector<CDR*> result;
		result = api->getCDRs(filter);
#ifndef REMOTE_CDR
		if (result.size()>MAX_CDR_RECORDS)	
		{
			unsigned int toDelete = result.size()-MAX_CDR_RECORDS;
			for (unsigned int i = 0; i < toDelete; i++)
			{			
				api->deleteObject(result[i]);
			}
			result.erase(result.begin(), result.begin() + toDelete);
		}		
#endif
	for (unsigned int i = 0; i<result.size(); i++) {
			CDR* cdr = result[i];
			if (cdr->dstExt.find(searchFilter.toStdString())!=-1)
				addCDR(cdr);
		}
	}
}

void Kiax2MainWindow::addContact(Contact* contact)
{
		Kiax2ListWidgetItem* listWidgetItem = new Kiax2ListWidgetItem(contactsList, 1001, contact->getObjectId());
		QString contactName(contact->contactName.data());
		listWidgetItem->setText(contactName);
		contactsList->addItem(listWidgetItem);
		delete contact;
		contact = NULL;
}

void Kiax2MainWindow::addCDR(CDR* cdr)
{
	Kiax2ListWidgetItem* listWidgetItem = new Kiax2ListWidgetItem(callRecords, 1002, cdr->getObjectId(), cdr->callState, cdr->direction);
	QString typeStr;
	if ((cdr->callState.compare(CALL_STATE_UNANSWERED)==0)&&(cdr->direction.compare(CALL_DIRECTION_INCOMING)==0))
		typeStr = tr("Missed call");
	else 
	if ((cdr->callState.compare(CALL_STATE_ANSWERED)==0)&&(cdr->direction.compare(CALL_DIRECTION_INCOMING)==0))
		typeStr = tr("Incoming call");
	else
	if ((cdr->callState.compare(CALL_STATE_UNANSWERED)==0)&&(cdr->direction.compare(CALL_DIRECTION_OUTGOING)==0))
		typeStr = tr("Unanswered outgoing call");
	else
	if ((cdr->callState.compare(CALL_STATE_ANSWERED)==0)&&(cdr->direction.compare(CALL_DIRECTION_OUTGOING)==0))
		typeStr = tr("Outgoing call");
	else
	if ((cdr->callState.compare(CALL_STATE_NONE)==0)&&(cdr->direction.compare(CALL_DIRECTION_INCOMING)==0))
		typeStr = tr("Rejected incoming call");

	if (cdr->duration.compare("00:00:00")==0||cdr->duration.compare("")==0)
		listWidgetItem->setToolTip("Type: " + typeStr);
	else
		listWidgetItem->setToolTip("Duration: " + QString::fromStdString(cdr->duration) + ", Type: " + typeStr);
	QString destination;
	if (cdr->dstName.compare(cdr->dstExt.data())==0)
	{
		destination = QString::fromStdString(cdr->dstName);
	} 
	else {
		destination = QString::fromStdString(cdr->dstExt);
	}
	QString time(cdr->cdrTime.data());
	QString cost(cdr->cdrInfo.data());
	if (cost!="")
		listWidgetItem->setText(time + "\n" + destination + ", " + cost + " " + QString::fromUtf8(BALANCE_DEFAULT_CURRENCY));
	else
	 	listWidgetItem->setText(time + "\n" + destination );
	if ((ENCODE_TEL_EXTEN)&&(destination.contains("##", Qt::CaseSensitive)))
	{
		QString host = destination;
		int hashPos = destination.indexOf("##");
		host.truncate(hashPos);
		host += ".tel";
		destination.remove(0, hashPos + 2);
		QString telnumber = destination;
		hashPos = -1;
		hashPos = telnumber.indexOf("##");
		if (hashPos>-1)
			telnumber.truncate(hashPos);	
		listWidgetItem->setText(time + "\n" + host + " - " + telnumber );
	}	

	callRecords->insertItem(0,listWidgetItem);
	delete cdr;
	cdr = NULL;
}

std::string Kiax2MainWindow::intToStr(int value)
{
		stringstream out;
		out << value;
		return out.str();
}

int Kiax2MainWindow::strToInt(std::string value)
{
	std::istringstream strin(value);
	int i;
	strin >> i;
	return i;
}

void Kiax2MainWindow::openCallDialog(Contact* dialContact, Account* account)
{
	openCallDialog(contactsList, NULL, dialContact, account);
}

void Kiax2MainWindow::openCallDialog(Contact* dialContact)
{
	openCallDialog(contactsList, NULL, dialContact, NULL);
}

void Kiax2MainWindow::openCallDialog(Kiax2CallAppearance* dialog, Contact* dialContact)
{
	openCallDialog(contactsList, dialog, dialContact, NULL);
}

QString Kiax2MainWindow::decodeExtension(QString number, bool decodeHost, bool decodeExtension)
{
               QString telNumber = number;
		QString destination = number;
		QString host = number;
		int hashPos = number.indexOf("##");
		host.truncate(hashPos);
		host += ".tel";
		if (decodeHost)
		{
			Logger::log(Logger::DEBUG, "decodeExtension: returning host-%s\n", host.toStdString().data());
			return host;
		}		
		number.remove(0, hashPos + 2);
		QString telnumber = number;
		hashPos = -1;
		hashPos = telnumber.indexOf("##");
		if (hashPos>-1)
			telnumber.truncate(hashPos);
		if (decodeExtension)
		{
			Logger::log(Logger::DEBUG, "decodeExtension: returning extension-%s\n", telnumber.toStdString().data());
			return telnumber;
		}	
		QString telname= host + " - " + telnumber;
		return telname;
}

void Kiax2MainWindow::openCallDialog(QListWidget* listWidget, Kiax2CallAppearance* dialog, Contact* dialContact, Account* account)
{
	Account* callAccount = account;
	QString decodedHost;
	QString decodedExtension;  
	if (account==NULL)
		callAccount = defaultAccount;
	if (dialog==NULL)
	{
		std::string rowid;
		std::string contactObjectType = "Contact";
		std::string cdrObjectType = "CDR";
		Contact* contact = dialContact;
		//if (dialContact!=NULL)
		
		if (contact==NULL)
		if (listWidget->currentRow()>=0)
		{
			vector<Serializable*> contacts;
			Kiax2ListWidgetItem* listViewItem = (Kiax2ListWidgetItem*) listWidget->currentItem();	
			stringstream out;
			out << listViewItem->objectId;
			rowid = out.str();
			std::string criteria = "rowid=" + rowid;

			if (listWidget == callRecords)
			{
				vector<Serializable*> cdrs = api->findObject(cdrObjectType, criteria);		
				if (cdrs.size()>0)
				{
					CDR* cdr = (CDR*) cdrs[0];				
					std::string contactCriteria = "extension='" + cdr->dstExt+ "'";		
					contacts = api->findObject(contactObjectType, contactCriteria);
					if (contacts.size()<1)
					{
						contact = new Contact();
						contact->setObjectId(0);
						contact->contactName = cdr->dstName;
						contact->extension = cdr->dstExt;
					}
				}

			}
			else if (listWidget == contactsList)
			{
					contacts = api->findObject(contactObjectType, criteria);
			}
			
			if (contacts.size()>0)
			{
				Contact* oContact = (Contact*) contacts[0];
				contact = new Contact();
				contact->setObjectId(oContact->getObjectId());
				contact->contactName = oContact->contactName;
				contact->extension = oContact->extension;
	
#ifdef USE_DOTTEL   
	QString number(oContact->extension.data());
	if (ALLOW_ONLY_TEL&&(!number.endsWith(".tel", Qt::CaseInsensitive)))
	{
		number+=".tel";
	}
        if (number.endsWith(".tel", Qt::CaseInsensitive))
        {
                QString telnumber = openDotTelDialog(number);
		QString name=telnumber;
                if (number==telnumber) return; //dont allow further dialing of .tel
		if ((ENCODE_TEL_EXTEN)&&(number.contains("##")))
		{
			name = decodeExtension(telnumber, true, false);
		}
		decodedHost = decodeExtension(telnumber, true);
		decodedExtension = decodeExtension(telnumber, false, true);

		contact->contactName = decodedHost.toStdString();
		contact->extension = telnumber.toStdString();
        }
#endif

				for (unsigned int i = 0; i< contacts.size(); i++)
				{
					delete contacts[i];
				}
			}
		}
		else
		{
			std::string criteria = searchComboBox->currentText().toStdString();
			vector<Contact*> contacts = api->getContacts(criteria);
			if (contacts.size()==1)
			{
				contact = new Contact();
				contact->setObjectId(contacts[0]->getObjectId());
				contact->contactName = contacts[0]->contactName;
				contact->extension = contacts[0]->extension;
			}
			else
			{
				if ((searchComboBox->currentText().length()>0)&&(contact==NULL)) 
				{
					contact = new Contact();
					QString number = searchComboBox->currentText();
					QString name=number;
					if ((ENCODE_TEL_EXTEN)&&(number.contains("##")))
						name = decodeExtension(number, true, false);
					contact->contactName = name.toStdString();
					contact->extension = searchComboBox->currentText().toStdString();
				}
			}
		}
		
		if (contact!=NULL)
		{
			QString teldomain = QString::fromStdString(contact->extension);
			decodedHost = decodeExtension(teldomain, true);
			decodedExtension = decodeExtension(teldomain, false, true);

			if ((ENABLE_CALL_FEED)&&(ENCODE_TEL_EXTEN))
			{
				updateFeed(decodedHost, decodedExtension);
			}
#if !defined(TABBED_LAYOUT)		
			Kiax2CallDialog* callDialog = new Kiax2CallDialog(this, api, contact);
			Kiax2CallAppearance* callAppearance = callDialog;
			//callDialog->setWindowFlags(flags);
			callDialog->setAlwaysOnTop(callWindowsOnTop);
#else
			Kiax2CallButton* callButton = new Kiax2CallButton(this, api, contact);
			Kiax2CallAppearance* callAppearance = callButton;
			Logger::log(Logger::DEBUG, "call appearance is button\n");
#endif
			int callNumber = api->dial(contact->extension, callAccount);
			callAppearance->initiateCall(CALL_DIRECTION_OUTGOING);
			Logger::log(Logger::DEBUG, "Kiax2MainWindow::openCallDialog: new call number is %d\n", callNumber);
			if (callNumber>-1)
			{
				callMap[callNumber] = callAppearance;
				callAppearance->setCallNumber(callNumber);
#if !defined(TABBED_LAYOUT)		
				callAppearance->appearance->show();
#else
				Logger::log(Logger::DEBUG, "adding button to exclusive button group\n");
				callButtonsGroup->addButton((QPushButton*)callAppearance->appearance);
				Logger::log(Logger::DEBUG, "adding button to layout\n");
				((QBoxLayout*)callsFrame->layout())->addWidget(callAppearance->appearance);
				Logger::log(Logger::DEBUG, "added button to layout\n");
				((QPushButton*)callAppearance->appearance)->setChecked(true);
				tabWidget->setCurrentIndex(0);
#endif
			}
			else
			{
				delete callAppearance;
			}
		}
	} 
	else
	{
			Contact* contact = dialog->getContact();
			int callNumber = api->dial(contact->extension, callAccount);
			dialog->initiateCall(CALL_DIRECTION_OUTGOING);
			if (callNumber>-1)
			{
				callMap[callNumber] = dialog;
				dialog->setCallNumber(callNumber);
			}
	}
}

void Kiax2MainWindow::dialWithCallAppearance(Kiax2CallAppearance* dialog)
{
	if (dialog!=NULL)
	{
			Contact* contact = dialog->getContact();
			int callNumber = api->dial(contact->extension, defaultAccount);
			dialog->initiateCall(CALL_DIRECTION_OUTGOING);
			if (callNumber>-1)
			{
				callMap[callNumber] = dialog;
				dialog->setCallNumber(callNumber);
			}
	}
}

void Kiax2MainWindow::exitKiax()
{
	std::string showParamName("ShowMainWindow");
	std::string showDialpadFrame("ShowDialpadFrame");
	
	if (readParameter(std::string("StateAuto"), "true").compare("false")!=0)
	
	{

		if (!window()->isVisible()&&!AUTO_DEFAULTS_WINDOW_SHOW)
			saveParameter(showParamName, "false");
		else
		{
			saveParameter(showParamName, "true");
			mainWindowGeometry = this->geometry();
		}
		std::string connectParamName("ConnectOnStart");

		if (actionConnect->isEnabled())
			saveParameter(connectParamName, "false");
		else
			saveParameter(connectParamName, "true");
    }
	
	if (dialpadFrame->isVisible())
	{
		saveParameter(showDialpadFrame, "true");
	}
	else
	{
		saveParameter(showDialpadFrame, "false");
	}
	
	saveGeometry();
	
	QCoreApplication::quit();
}

void Kiax2MainWindow::saveParameter(std::string paramName, std::string paramValue)
{
	vector<ConfigurationParameter*> paramList = api->getParameters(paramName);
	ConfigurationParameter* param;
	if (paramList.size()>0)
	{
		param = paramList[0]->clone();
		param->name = paramName;
		param->value = paramValue;
		api->updateObject(param);		
	} 
	else
	{
		param = new ConfigurationParameter();
		param->name = paramName;
		param->value = paramValue;
		param->paramType = "string";
		api->createObject(param);
	}
	delete param;
		
}

std::string Kiax2MainWindow::readParameter(std::string paramName, std::string defaultResult)
{
	vector<ConfigurationParameter*> paramList = api->getParameters(paramName);
	ConfigurationParameter* param;
	if (paramList.size()>0)
	{
		param = paramList[0]->clone();
		Logger::log(Logger::DEBUG, "readParameter OUT\n");
		return param->value;
	} 
	return defaultResult;
}

void Kiax2MainWindow::aboutAction_activated()
{
	Kiax2AboutDialog aboutDialog(this);
	aboutDialog.exec();
}
 
void Kiax2MainWindow::testNetwork(bool testSpeed)
{
	Logger::log(Logger::DEBUG, "Detecting network connection speed..\n");
	std::string paramNetworkAuto("NetworkAuto");
	bool networkAuto = readParameter(paramNetworkAuto, "true").compare("false")!=0;
	if (networkAuto)
	{
		Logger::log(Logger::DEBUG, "Detecting network connection speed..\n");
		if (isLocalNetwork())
			iaxc_set_formats(IAXC_FORMAT_ULAW, IAX2_CAPABILITIES_ULAW);
		else if (testSpeed&&ENABLE_SPEEDTEST)
		{
			connectionMeter->downloadFile();
		}
	}
}

void Kiax2MainWindow::settingsAction_activated()
{
	Kiax2SettingsDialog settingsDialog(this);
	settingsDialog.exec();
	if (settingsDialog.result() == QDialog::Accepted)
	{
		testNetwork(ENABLE_SPEEDTEST);
		reloadJSONServers();
		if (settingsDialog.isMainWinOnTop())
		{
			Qt::WindowFlags winFlags = flags;
			winFlags |= Qt::WindowStaysOnTopHint;
			setWindowFlags(winFlags);
			setVisible(true);
		}
		else
		{
			setWindowFlags(flags);
			setVisible(true);
		}
    }
	callWindowsOnTop = settingsDialog.isDialWinOnTop();
}

void Kiax2MainWindow::speedTestComplete(bool aborted, uint downloadSpeed)
{
	if (aborted)
		Logger::log(Logger::DEBUG, "Test was aborted.\n");
	else
	{
		Logger::log(Logger::DEBUG, "Detected connection speed = %d KB/s\n", downloadSpeed);
		if (downloadSpeed > 100)
			iaxc_set_formats(DEFAULT_LAN_CODEC, DEFAULT_LAN_CODEC);
		else 
			iaxc_set_formats(DEFAULT_INTERNET_CODEC,DEFAULT_INTERNET_CODEC);
	}

}

bool Kiax2MainWindow::isLocalNetwork()
{
	QHostInfo info = QHostInfo::fromName(QString(defaultAccount->host1.data()));
	if (!info.addresses().isEmpty()) {
		QHostAddress address = info.addresses().first();
		quint32 ipaddress = address.toIPv4Address();
		quint8 firstByte = ipaddress >> 24;
		quint8 secondByte = (ipaddress >> 16) & 0x00FF;
		Logger::log(Logger::DEBUG, "Host address is: %s, first byte is %d, secondB byte is %d\n", address.toString().toStdString().data(), firstByte, secondByte);
		return ((firstByte == 10)||((firstByte == 192) && (secondByte == 168)));
	}
	return false;
}

void Kiax2MainWindow::connectAction_activated()
{
	api->registerAccount(defaultAccount);
}

void Kiax2MainWindow::disconnectAction_activated()
{
	if (SHOW_LOGIN_SCREEN_ON_DISCONNECT)
		disableUse();

	api->unregisterAccount(defaultAccount);
	actionConnect->setEnabled(true);
	actionDisconnect->setEnabled(false);
	if (SINGLE_CONNECT_DISCONNECT)
	{
		actionDisconnect->setVisible(false);
		actionConnect->setVisible(true);
	}
	statusButton->setText(STATUS_DISCONNECTED_LABEL);
	statusButton->setIcon(QIcon("icons/disconnectedsmall.png"));	
	sysTray->setIcon(QIcon("icons/disconnectedsmall.png"));	
	sysTray->setToolTip(QString(DEFAULT_SERVICE_NAME) + " - " + tr("Disconnected"));
	
}

void Kiax2MainWindow::checkDefaultAccount()
{
	vector<Account*> accountsResult = api->getAccounts();
	if (accountsResult.size()<1)
	{
		if (AUTO_CREATE_GUEST_ACCOUNT)
		{
			Account* account = new Account();
			account->serviceName = DEFAULT_SERVICE_NAME;
			account->host1 = DEFAULT_REGISTER_HOST;
			account->host2 = SECOND_REGISTER_HOST;
			account->host3 = THIRD_REGISTER_HOST;
			account->username = GUEST_ACCOUNT;  //"guest";
			account->password = GUEST_PASSWORD; //"guest";
			api->addAccount(account);
//			delete account;
			defaultAccount = account;
		}
		if (CREATE_ECHO_TEST_CONTACT)
		{
			vector<Contact*> contactsResult = api->getContacts();
			if (contactsResult.size()<1)
			{
				Contact* contact = new Contact();
				contact->contactName = std::string(ECHO_TEST_CONTACT_NAME);
				contact->extension = std::string(ECHO_TEST_CONTACT_NUMBER);
				api->addContact(contact);
				refreshContactList("");
			}
		}
		disableUse();
		if (ENABLE_WEB_SERVICES)
		{
			if (ENABLE_NEW_ACCOUNT_DIALOG)
			{
				int qresult = QMessageBox::question(this, "Got Account?", "Did you already get a " + QString(DEFAULT_SERVICE_NAME) + " account? Press 'Yes' to set enter account data, or 'No' to sign up.", QMessageBox::Yes | QMessageBox::No);	
				if (qresult!=QMessageBox::Yes)
				{
					registerAction_activated();
				} else
				{
					accountAction_activated();
				}
			}
		}
		else
			if (ENABLE_NEW_ACCOUNT_DIALOG)
				accountAction_activated();
	}
	else
	{ 
		defaultAccount->serviceName = accountsResult[0]->serviceName;
		defaultAccount->host1 = accountsResult[0]->host1;
		defaultAccount->host2 = accountsResult[0]->host2;
		defaultAccount->host3 = accountsResult[0]->host3;
		defaultAccount->username = accountsResult[0]->username;
		defaultAccount->password = accountsResult[0]->password;
		//enableUse();
		if (ENABLE_BALANCE)
		{
			updateBalanceParams();
			jsonBalance->getJSONData(jsonBalanceRequestUrl);
		}
		if (ENABLE_ONLINE_USERS)
		{
			jsonUsersOnline->getJSONData(jsonUsersOnlineRequestUrl);
		}
		if (NOTIFICATION_ENABLED)
		{
			notificationTimeout();
		}
	}
	if (!DISABLE_USERNAME_TITLE)
		setWindowTitle(QString::fromStdString(defaultAccount->username) + " - " + QString(DEFAULT_SERVICE_NAME));
	Core::configuration["username"] = defaultAccount->username;
} 

void Kiax2MainWindow::updateBalanceParams()
{
	if (ENABLE_BALANCE)
	{
		jsonBalanceRequestUrl = JSON_BALANCE_REQUEST_URL;
		jsonBalance->setUsername(QString(defaultAccount->username.data()));
		if (SUBMIT_BALANCE_PASSWORD) 
		{
			jsonBalance->setPassword(QString(defaultAccount->password.data()));
		}
		Logger::log(Logger::DEBUG, "jsonBalanceRequestUrl = %s\n", jsonBalanceRequestUrl.toStdString().data());	
	} 
}

void Kiax2MainWindow::registerAction_activated()
{
#ifdef USE_WEBFORMS
		registerDialog->loadPage();
		registerDialog->exec();
#else
		//QString session = Kiax2SessionManager::getSession();
		//Logger::log(Logger::DEBUG, "Opening %s with session = %s\n", HTTP_GET_ACCOUNT_URL, session.toStdString().data());
		QString url = QString(HTTP_GET_ACCOUNT_URL); // + "?" + HTTP_CONFIG_PARAM_SESSION + "=" + session;
		QDesktopServices::openUrl(QUrl(url));
#endif		
}

void Kiax2MainWindow::changePasswordAction_activated()
{
	if (ENABLE_HTTP_CONFIGURATION && USE_HTTP_CONFIGURATION)
	{
		//QString session = Kiax2SessionManager::getSession();
		QString url = QString(HTTP_CHANGE_PASSWORD_URL);// + "&" + HTTP_CONFIG_PARAM_SESSION + "=" + session;
		QDesktopServices::openUrl(QUrl(url));
	}
	else
	{
#ifdef USE_WEBFORMS	
		changePasswordDialog->setUsername(QString::fromStdString(defaultAccount->username));
		changePasswordDialog->loadPage();
		changePasswordDialog->exec();
#endif
	}
} 

void Kiax2MainWindow::accountAction_activated()
{
	Kiax2AccountDialog accountDialog(this);
	vector<Account*> accountsResult = api->getAccounts();
	if (accountsResult.size()>0)
	{
		accountDialog.setUsernameText(accountsResult[0]->username);
		accountDialog.setPasswordText(accountsResult[0]->password);
		if (ENABLE_SERVER_FIELD)
			accountDialog.setServerText(accountsResult[0]->host1);
		
	}
	if (accountDialog.exec()==QDialog::Accepted)
	{
		if (accountsResult.size()>0) 
		{
			accountsResult[0]->username = accountDialog.getUsernameText();
			accountsResult[0]->password = accountDialog.getPasswordText();
			if (ENABLE_SERVER_FIELD)
			{
				accountsResult[0]->host1 = accountDialog.getServerText();
			}

			api->updateObject(accountsResult[0]);
			checkDefaultAccount();
		}
		else
		{
			Account* account = new Account();
			account->username = accountDialog.getUsernameText();
			account->password = accountDialog.getPasswordText();
			if (ENABLE_SERVER_FIELD)
				account->host1 = accountDialog.getServerText();
			else
				account->host1 = DEFAULT_REGISTER_HOST;
			account->host2 = SECOND_REGISTER_HOST;
			account->host3 = THIRD_REGISTER_HOST;
			account->serviceName = DEFAULT_SERVICE_NAME;
			api->updateObject(account);
			delete account;
			account = NULL; 
			checkDefaultAccount();
		}
		reconnect();
		
	}
}

void Kiax2MainWindow::showContactContextMenu(const QPoint& point)
{
		QPoint xy(point.x(), point.y()+60);
		contactContextMenu->popup(mapToGlobal(xy));
}

void Kiax2MainWindow::showCdrContextMenu(const QPoint& point)
{
		QPoint xy(point.x(), point.y()+60);
		cdrContextMenu->popup(mapToGlobal(xy));
}

void Kiax2MainWindow::showStatusMenu()
{
		Logger::log(Logger::DEBUG, "statusButton clicked\n");
		QPoint xy(statusButton->geometry().x(), statusButton->geometry().y()+30);
		statusMenu->popup(mapToGlobal(xy));
}

void Kiax2MainWindow::addContactAction_activated()
{
	Kiax2ContactDialog contactDialog(this);
	if (callRecords->isVisible()&&(callRecords->currentRow()>=0))
	{
		std::string cdrObjectType = "CDR";
		Kiax2ListWidgetItem* listViewItem = (Kiax2ListWidgetItem*) callRecords->currentItem();	
		stringstream out;
		std::string rowid;
		out << listViewItem->objectId;
		rowid = out.str();
		std::string criteria = "rowid=" + rowid;
		vector<Serializable*> cdrs = api->findObject(cdrObjectType, criteria);		
		if (cdrs.size()>0)
		{
			CDR* cdr = (CDR*) cdrs[0];
			if (cdr->direction.compare(CALL_DIRECTION_INCOMING)==0)
			{
				contactDialog.setContactNameText(cdr->srcExt);
				contactDialog.setExtensionText(cdr->srcExt);
			} 
			else
			{
				contactDialog.setContactNameText(cdr->dstName);
				contactDialog.setExtensionText(cdr->dstExt);
			}
		} 
		
	}
	if (contactDialog.exec()==QDialog::Accepted)
	{
		Contact* contact = new Contact();
		contact->contactName = contactDialog.getContactNameText();
		contact->extension = contactDialog.getExtensionText();
		api->createObject(contact);
		addContact(contact);
	}
		
}

void Kiax2MainWindow::deleteContactAction_activated()
{
	Contact* contact = new Contact();
	Kiax2ListWidgetItem* listViewItem = (Kiax2ListWidgetItem*) contactsList->currentItem();
	contact->setObjectId(listViewItem->objectId);
	api->deleteObject(contact);
	delete contact;
	contact = NULL;
	refreshContactList("");		
}

void Kiax2MainWindow::inviteFriendAction_activated()
{
	QDesktopServices::openUrl(QUrl(INVITE_FRIEND_URL));
}

void Kiax2MainWindow::callRatesButtonClicked()
{
	if (ENABLE_CALL_RATES)
		QDesktopServices::openUrl(QUrl(CALL_RATES_URL));
}

void Kiax2MainWindow::callAction_activated()
{
	if (contactsList->isVisible())
	{
		openCallDialog(contactsList);
	}
	else
		openCallDialog(callRecords);
}

void Kiax2MainWindow::callRecord_activated()
{
	openCallDialog(callRecords);
}

void Kiax2MainWindow::editContactAction_activated()
{
	Kiax2ContactDialog contactDialog(this);
	Kiax2ListWidgetItem* listViewItem = (Kiax2ListWidgetItem*) contactsList->currentItem();	
	std::string rowid;
	stringstream out;
	out << listViewItem->objectId;
	rowid = out.str();
	std::string criteria = "rowid=" + rowid;
	std::string objectType = "Contact";
	vector<Serializable*> contacts = api->findObject(objectType, criteria);
	if (contacts.size()>0) 
	{
		Contact* contact = (Contact*) contacts[0];
		contactDialog.setContactNameText(contact->contactName);
		contactDialog.setExtensionText(contact->extension);
	}
	if (contactDialog.exec()==QDialog::Accepted)
	{
		if (contacts.size()>0) 
		{
			Contact* contact = (Contact*) contacts[0];
			contact->contactName = contactDialog.getContactNameText();
			contact->extension = contactDialog.getExtensionText();
			api->updateObject(contacts[0]);
			refreshContactList("");
		}
		
	}
}


void Kiax2MainWindow::registrationAccepted(Account* account)
{
	Logger::log(Logger::INFO, "Registration for account %s accepted.\n", account->host1.data());
	actionDisconnect->setEnabled(true);
	actionConnect->setEnabled(false);
	if (SINGLE_CONNECT_DISCONNECT)
	{
		actionDisconnect->setVisible(true);
		actionConnect->setVisible(false);
	} 
	statusButton->setText(STATUS_CONNECTED_LABEL);
	statusButton->setIcon(QIcon("icons/connectedsmall.png"));
	sysTray->setIcon(QIcon("icons/connectedsmall.png"));
	sysTray->setToolTip(QString(DEFAULT_SERVICE_NAME) + " - " + tr("Connected"));
	saveParameter(std::string("LoggedIn"), std::string("true"));
	enableUse();
	if (ENABLE_LOGIN_SCREEN)
		loginScreen->setLoginStatus("Login successful");

}
	
void Kiax2MainWindow::registrationTimedout(Account* account)	
{
	Logger::log(Logger::INFO, "Registration for account %s timed out.\n", account->host1.data());
	actionDisconnect->setEnabled(true);
	actionConnect->setEnabled(false);
	if (SINGLE_CONNECT_DISCONNECT)
	{
		actionDisconnect->setVisible(true);
		actionConnect->setVisible(false);
	}
	statusButton->setText(STATUS_TIMEDOUT_LABEL);
	statusButton->setIcon(QIcon("icons/timeoutsmall.png"));
	sysTray->setIcon(QIcon("icons/timeoutsmall.png"));
	sysTray->setToolTip(QString(DEFAULT_SERVICE_NAME) + " - " + tr("Timeout"));
	if (ENABLE_LOGIN_SCREEN)
		loginScreen->setLoginStatus("Login timed out");

}

void Kiax2MainWindow::registrationRejected(Account* account)
{
	Logger::log(Logger::INFO, "Registration for account %s rejected.\n", account->host1.data());
	actionConnect->setEnabled(true);
	actionDisconnect->setEnabled(false);
	if (SINGLE_CONNECT_DISCONNECT)
	{
		actionConnect->setVisible(true);
		actionDisconnect->setVisible(false);
	}
	statusButton->setText(STATUS_DISCONNECTED_LABEL);
	statusButton->setIcon(QIcon("icons/disconnectedsmall.png"));
	sysTray->setIcon(QIcon("icons/disconnectedsmall.png"));
	sysTray->setToolTip(QString(DEFAULT_SERVICE_NAME) + " - " + tr("Disconnected"));
	if (ENABLE_LOGIN_SCREEN)
	{
		if (!loginScreen->isVisible())
		{
			notificationUrl = "";
			sysTray->showMessage(DEFAULT_SERVICE_NAME, "Registration failed");
		}
		else
		{
				loginScreen->setLoginStatus(tr("Login rejected"));
		}
			//disableUse();
	}
	else
	{
		accountAction_activated();
	}
} 

void Kiax2MainWindow::incomingCall(int callNumber, std::string callerId)
{
	Logger::log(Logger::INFO, "Incoming call. Call number %d\n", callNumber);
	Contact* contact = new Contact();
	std::vector<Contact*> contactsByExt = api->getContacts(callerId);
	if (contactsByExt.size()>0)
	{
		contact->contactName = contactsByExt[0]->contactName; // take first contact
	}
	else 
	{
		contact->contactName = callerId;
	}
	contact->extension = callerId;
#if !defined(TABBED_LAYOUT)
	Kiax2CallDialog* callDialog = new Kiax2CallDialog(this, api, contact);
	Kiax2CallAppearance* callAppearance = callDialog;
	callDialog->setAlwaysOnTop(callWindowsOnTop);
#else
	Kiax2CallButton* callButton = new Kiax2CallButton(this, api, contact);
	Kiax2CallAppearance* callAppearance = callButton;
#endif	
	callMap[callNumber] = callAppearance;
	callAppearance->setCallNumber(callNumber);
	callAppearance->setCallState(CALL_RINGING);
	callAppearance->setCallDirection(CALL_INCOMING);
	callAppearance->initiateCall(CALL_DIRECTION_INCOMING);
#if !defined(TABBED_LAYOUT)
	callAppearance->appearance->window()->show();
#else
	Logger::log(Logger::DEBUG, "adding button to exclusive button group\n");
	callButtonsGroup->addButton((QPushButton*)callAppearance->appearance);
	Logger::log(Logger::DEBUG, "adding button to frame layout\n");
	((QBoxLayout*)callsFrame->layout())->addWidget(callAppearance->appearance);
	// ((QPushButton*)callAppearance->appearance)->setChecked(true);
	Logger::log(Logger::DEBUG, "added button to frame layout\n");
	tabWidget->setCurrentIndex(0);
#endif
}

void Kiax2MainWindow::ringing(int callNumber)
{
	Logger::log(Logger::INFO, "Ringing. Call number %d\n", callNumber);
	Kiax2CallAppearance* callWindow = callMap[callNumber];
	if (callWindow!=NULL)
	{
		callWindow->setCallState(CALL_RINGING);
		startRing();
	}
}

void Kiax2MainWindow::outgoingCall(int callNumber)
{
	Logger::log(Logger::INFO, "Outgoing call active. Call number %d\n", callNumber);
	Kiax2CallAppearance* callWindow = callMap[callNumber];
	if (callWindow!=NULL)
	{
		callWindow->setCallDirection(CALL_OUTGOING);
	}
}

void Kiax2MainWindow::hangupCalls(int* list, int size)
{
Logger::log(Logger::INFO, "hangupCalls IN\n");    	
        for (int i = 0; i < size; i++) {
		if (callMap[list[i]]!=NULL)
		{
			Logger::log(Logger::INFO, "Hanging up call number %d\n", list[i]); 
			api->setActiveCall(list[i]);
			api->hangup(list[i]);   	
			(callMap[list[i]])->closeAppearance();
			callInactive(list[i]);
		}
        }
	free(list);
	list = NULL;
Logger::log(Logger::INFO, "hangupCalls OUT\n"); 
}

void Kiax2MainWindow::callInactive(int callNumber)
{
	Logger::log(Logger::INFO, "Call became inactive. Call number %d\n", callNumber);    	
	Kiax2CallAppearance* callWindow = callMap[callNumber];
	map<int, Kiax2CallAppearance*>::const_iterator itr;
	if (callWindow!=NULL)
	{
		stopRing();
		callWindow->setCallState(CALL_INACTIVE);
		int deleted = callMap.erase(callNumber);
		Logger::log(Logger::DEBUG, "Call %d deleted. Number of deleted elements is %d\n", callNumber, deleted);
		callMap[callNumber] = NULL; // keep the pair though.
		int existingCalls = 0;
		for(itr = callMap.begin(); itr != callMap.end(); ++itr){
			int activeCallNumber = itr->first;
			if (callMap[activeCallNumber]!=NULL)
			{
				existingCalls++;
			}
		}
	if (existingCalls==0)
		emit allCallsHungup();
	}

	if (ENABLE_BALANCE)
		balanceTimer->start(300000);
	if (exitOnCallEnd)
		exitKiax();
	Logger::log(Logger::INFO, "callInactive(%d) out\n", callNumber);
}

void Kiax2MainWindow::callTransferred(int callNumber)
{
	Logger::log(Logger::INFO, "Call transferred. Call number %d\n", callNumber);    	
}

void Kiax2MainWindow::messageReceived(std::string message)
{
	Logger::log(Logger::INFO, "Message received. Message body: %s\n", message.data());    	
}
    
void Kiax2MainWindow::messageSent(std::string message)
{
	Logger::log(Logger::INFO, "Message sent. Message body: %s\n", message.data());    	
}

void Kiax2MainWindow::activeCallChanged(int callNumber)
{
	Logger::log(Logger::INFO, "Active call changed. New active number %d\n", callNumber); 
}

void Kiax2MainWindow::callComplete(int callNumber)
{
	Logger::log(Logger::INFO, "Call complete (established). Call number %d\n", callNumber);    	    	
	stopRing();
	if (callMap[callNumber]!=NULL)
		callMap[callNumber]->setCallState(CALL_COMPLETE);
}

void Kiax2MainWindow::inputOutputLevels(float inputLevel, float outputLevel)
{
	//Logger::log(Logger::DEBUG, "Intput level0 %f, output0 level %f\n", inputLevel, outputLevel);
	//Logger::log(Logger::DEBUG, "Intput level %f, output level %f\n", iaxc_input_level_get(), iaxc_output_level_get()); 
#if !defined(TABBED_LAYOUT)	
	int selectedCall = api->getActiveCallNumber();
	if ((callMap.count(selectedCall)>0)&&(callMap[selectedCall]!=NULL))
	{
		Kiax2CallDialog* appearance = (Kiax2CallDialog*)callMap[selectedCall];
		appearance->setInputOutputLevels(inputLevel, outputLevel);
		int micSliderLevel = appearance->getMicSliderValue();
		int audioSliderLevel = appearance->getAudioSliderValue();
		if ((int)(iaxc_input_level_get()*100)!=micSliderLevel)
		{
			appearance->setMicSliderValue((int)(fround(iaxc_input_level_get(),2)*100));
		}
		if ((int)(iaxc_output_level_get()*100)!=audioSliderLevel)
		{
			appearance->setAudioSliderValue((int)(fround(iaxc_output_level_get(),2)*100));
		}
	}
	
#endif
}

void Kiax2MainWindow::jsonBalanceRequestInitiated()
{
	if (ENABLE_BALANCE)
		balanceLabel->setText("");
}

void Kiax2MainWindow::jsonBalanceRequestComplete(bool reqCancelled, QString reqData)
{
	if (ENABLE_BALANCE)
	{
		QString balance;
		Logger::log(Logger::DEBUG, "jsonBalanceRequest: data is %s\n", reqData.toStdString().data());
		if (!BALANCE_XML)
		{
			balance = jsonBalance->getAccountBalance(reqData);
		}
		else {
			balance = jsonBalance->getAccountBalanceXML(reqData);
		}
		if ((!reqCancelled)&&(balance!=""))
		{
			balanceLabel->setVisible(true);
			balanceLabel->setText(QString(BALANCE_LABEL_TEXT) + " " +QString::fromUtf8(BALANCE_DEFAULT_CURRENCY) + " " + balance);
		}
		else
			balanceLabel->setVisible(false);
		

	}
}

void Kiax2MainWindow::jsonUsersOnlineRequestInitiated()
{
	if (ENABLE_ONLINE_USERS)
		onlineUsersButton->setText(tr("Connecting.."));
}

void Kiax2MainWindow::jsonUsersOnlineRequestComplete(bool reqCancelled, QString reqData)
{
	if (ENABLE_ONLINE_USERS)
	{
		Logger::log(Logger::DEBUG, "jsonUsersOnlineRequest: data is %s\n", reqData.toStdString().data());
		QString usersOnline = jsonUsersOnline->getUsersOnline(reqData);
	
		if (!reqCancelled)
			onlineUsersButton->setText(usersOnline + " " + tr("users online"));
		else
			onlineUsersButton->setText(" #");
	}
}

void Kiax2MainWindow::jsonSessionUpdated(QString notificationMessage, QString url)
{
	Logger::log(Logger::DEBUG, "Session updated.\n");
}

void Kiax2MainWindow::jsonNotificationAvailable(QString notificationMessage, QString url)
{
	if ((notificationMessage.compare(lastNotification)!=0)||(url.compare(lastNotificationUrl)!=0))
	{
		sysTray->showMessage(DEFAULT_SERVICE_NAME, notificationMessage);
		notificationUrl = url;
		lastNotification = notificationMessage;
		lastNotificationUrl = url;
	}
}

void Kiax2MainWindow::notificationBalloonClicked()
{
	if (notificationUrl!="")
		QDesktopServices::openUrl(QUrl(notificationUrl));
}

void Kiax2MainWindow::jsonRequestAborted()
{
}

void Kiax2MainWindow::jsonRequestError()
{
}

void Kiax2MainWindow::dtmf0()
{
	appendNumberToNumberEdit("0");
}

void Kiax2MainWindow::dtmf1()
{
	appendNumberToNumberEdit("1");
}

void Kiax2MainWindow::dtmf2()
{
	appendNumberToNumberEdit("2");
}

void Kiax2MainWindow::dtmf3()
{
	appendNumberToNumberEdit("3");
}

void Kiax2MainWindow::dtmf4()
{
	appendNumberToNumberEdit("4");
}

void Kiax2MainWindow::dtmf5()
{
	appendNumberToNumberEdit("5");
}

void Kiax2MainWindow::dtmf6()
{
	appendNumberToNumberEdit("6");
}

void Kiax2MainWindow::dtmf7()
{
	appendNumberToNumberEdit("7");
}

void Kiax2MainWindow::dtmf8()
{	
	appendNumberToNumberEdit("8");
}

void Kiax2MainWindow::dtmf9()
{
	appendNumberToNumberEdit("9");
}

void Kiax2MainWindow::dtmfStar()
{
	appendNumberToNumberEdit("*");
}

void Kiax2MainWindow::dtmfHash()
{
	appendNumberToNumberEdit("#");
}

void Kiax2MainWindow::jsonServersRequestError()
{
}

void Kiax2MainWindow::jsonServersRequestAborted()
{
}

void Kiax2MainWindow::jsonServersRequestComplete(bool error, QString jsonMessage)
{
	Logger::log(Logger::DEBUG, "Got JSON: %s\n", jsonMessage.toStdString().data());
	Kiax2Servers* apiServers = jsonServers->getServers(jsonMessage);
	if ((apiServers==NULL)||(error))
	{
		Logger::log(Logger::DEBUG, "json returned NULL result or error.");
		routing = AGENT_ROUTING_SOFTPHONE;
	} else {
		servers = apiServers;
		std::string paramSupernodeUser("SupernodeUser");
		std::string paramRoutingDestination1("RoutingDestination1");
		std::string paramRoutingDestination2("RoutingDestination2");
		std::string paramRoutingDestination3("RoutingDestination3");
		saveParameter(paramRoutingDestination1, servers->getServers()[0]);
		if (servers->getServers().size() > 1)
			saveParameter(paramRoutingDestination2, servers->getServers()[1]);
		if (servers->getServers().size()>2)
			saveParameter(paramRoutingDestination3, servers->getServers()[2]);
		saveParameter(paramSupernodeUser, servers->isSupernodeUser()?"true":"false");
		if (routing == -1)
		{
			if (servers->isSupernodeUser())
			{
				routing = AGENT_ROUTING_SOFTPHONE;
			}
			else
			{
				routing = AGENT_ROUTING_SERVER;
			}
			std::string paramRouting("Routing");
			saveParameter(paramRouting, QString::number(routing).toStdString());
		}
	}
}

void Kiax2MainWindow::jsonServersRequestInitiated()
{
	Logger::log(Logger::DEBUG, "Connecting to api server %s..\n", AGENT_ROOT_API_URL );
}

void Kiax2MainWindow::reloadJSONServers()
{
	if (ENABLE_HTTP_CONFIGURATION)
		if (readAccountData())
		{
			jsonServers->getJSONData(QString(AGENT_ROOT_API_URL) + "?username=" + username +  "&password=" + password);
		}
}

bool Kiax2MainWindow::readAccountData()
{
	APIBinder* api = APIBinderDefault::instance();

	vector<Account*> accountsResult = api->getAccounts();
	Account* account = NULL;
	
	if (accountsResult.size()>0)
	{
		account = accountsResult[0];
		username = QString::fromStdString(account->username);
		password = QString::fromStdString(account->password);
		return true;
	} else
	{
		return false;
	}
}

void Kiax2MainWindow::initApiServer()
{
		apiServer = new Kiax2DispatcherServer(NULL);
		jsonServers = new Kiax2JSONServers(this);
		
		connect(jsonServers, SIGNAL(requestInitiated()), this, SLOT(jsonServersRequestInitiated()));
		connect(jsonServers, SIGNAL(requestComplete(bool, QString)), this, SLOT(jsonServersRequestComplete(bool, QString)));
		connect(jsonServers, SIGNAL(requestAborted()), this, SLOT(jsonRequestAborted()));
		connect(jsonServers, SIGNAL(requestError()), this, SLOT(jsonRequestError()));
		connect(apiServer, SIGNAL (configureAccount(QString, QString, QString, QString)), this, SLOT( httpConfigureAccount(QString, QString, QString, QString)));
		connect(apiServer, SIGNAL (changePassword(QString, QString)), this, SLOT( httpChangePassword(QString, QString)));
		connect(apiServer, SIGNAL (call(QString)), this, SLOT( httpCall(QString)));
		connect(apiServer, SIGNAL (callWithAccount(QString, Account*)), this, SLOT( httpCallWithAccount(QString, Account*)));
		connect(apiServer, SIGNAL (hangup()), this, SLOT( httpHangup()));
		connect(apiServer, SIGNAL (hangupCalls(int*, int)), this, SLOT( hangupCalls(int*, int)));

		std::string paramRouting("Routing");
		std::string routingStr = readParameter(paramRouting, "-1");
		routing = (QString::fromStdString(routingStr)).toInt();
		
		std::string paramSupernodeUser("SupernodeUser");
		std::string paramRoutingDestination1("RoutingDestination1");
		std::string paramRoutingDestination2("RoutingDestination2");
		std::string paramRoutingDestination3("RoutingDestination3");
		std::string supernodeStr = readParameter(paramSupernodeUser, "true");
		bool supernodeUserBool = (supernodeStr.compare("true") == 0);
		std::string server1Str = readParameter(paramRoutingDestination1, AGENT_DEFAULT_SERVER_REDIRECT_HOST);
		std::string server2Str = readParameter(paramRoutingDestination2, AGENT_DEFAULT_SERVER_REDIRECT_HOST);
		std::string server3Str = readParameter(paramRoutingDestination3, AGENT_DEFAULT_SERVER_REDIRECT_HOST);		
		servers = new Kiax2Servers();
		servers->addServer(server1Str);
		servers->addServer(server2Str);
		servers->addServer(server3Str);
		servers->setSupernodeUser(supernodeUserBool);

		reloadJSONServers();
}


void Kiax2MainWindow::receiveMessage(QString ipcMessage)
{
	Logger::log(Logger::DEBUG, "Got IPC message: %s\n", ipcMessage.toStdString().data());
	if (ipcMessage.contains("voip://"))
	{
		int argc = 2;
		char *argv[2];
		argv[0] = "";
		int strsize = ipcMessage.toStdString().length();
		argv[1] = new char[ strsize + 1];
		strncpy(argv[1], ipcMessage.toStdString().data(), ipcMessage.toStdString().length());
		argv[1][strsize]=0;
		parseCommandParameters(argc, argv);
	}
}

int main(int argc, char * argv[])
{  

	Kiax2Application app(argc, argv, "Kiax");

	if (app.isRunning())
	{
		if (argc>1)
		{
			QString allParams;
			if (strstr(argv[1],"voip://"))
			{
				for (int i=1; i<=argc;i++)
				{
					allParams = allParams + QString(argv[i]);
				}
			}
			app.sendMessage(allParams);
		}	
		return 0;
	}

    Kiax2MainWindow win(&app);	

	QObject::connect(&app, SIGNAL(messageAvailable(QString)), &win, SLOT(receiveMessage(QString)));

	
	return app.exec();

	// connect message queue to the main window.

}
