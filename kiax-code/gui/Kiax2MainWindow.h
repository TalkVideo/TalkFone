#ifndef KIAX2MAINWINDOW_H
#define KIAX2MAINWINDOW_H

#include <stdarg.h>
#include <stdlib.h>
#include <math.h>
#include "iaxclient.h"
#include <map>
#include <vector>
#include <string>
#include <QtGui>
#include "Core.h"
#include "APIBinderDefault.h"
#include "ui_kiax2windowAlt.h"
#include "Kiax2AboutDialog.h"
#include "Kiax2ContactDialog.h"
#include "Kiax2AccountDialog.h"
#ifdef USE_WEBKIT
#include "Kiax2RegisterDialog.h"
#include "Kiax2ChangePassDialog.h"
#endif
#include "Kiax2CallDialog.h"
#include "Kiax2ListWidgetItem.h"
#include "Kiax2SettingsDialog.h"
#include "Kiax2ContactListItemWidget.h"
#include "Kiax2ConnectionMeter.h"
#include "Kiax2JSONBalance.h"
#include "Kiax2JSONUsersOnline.h"
#include "IAX2CallbackHandlerQt.h"
#include <QSystemTrayIcon>
#include <QTimer>
#include <QDesktopServices>
#include <QUrl>
#include <QTimer>
#include <QDateTime>
#include <QTranslator>
#include <QPushButton>
#include "Kiax2LoginScreen.h"
#include "Kiax2Application.h"
#include "Kiax2JSONNotification.h"
#include "Kiax2DispatcherServer.h"
#include "Kiax2DispatcherThread.h"
#include "Kiax2JSONSupernode.h"
#include "Kiax2JSONServers.h"
#include "Kiax2Servers.h"
#include "Kiax2CallAppearance.h"
#include "Kiax2CallButton.h"
#include <QButtonGroup>
#include "Kiax2DotTelDialog.h"
#include <QList>
#ifdef USE_DOTTEL	
#include "CppDotTelUtils.h"
#endif

class Kiax2MainWindow : public QMainWindow, public SignalingCallback, private Ui::MainWindow
{

  Q_OBJECT

public:
  Kiax2MainWindow(Kiax2Application* app);
  virtual ~Kiax2MainWindow();
  
  IAX2CallbackHandlerQt* iaxCallback;
  
  /* Signal Handler methods */
  virtual void registrationAccepted(Account* account);
  virtual void registrationTimedout(Account* account);
  virtual void registrationRejected(Account* account);
  virtual void incomingCall(int callNumber, std::string callerId);
  virtual void ringing(int callNumber);
  virtual void outgoingCall(int callNumber);
  virtual void callInactive(int callNumber);
  virtual void callTransferred(int callNumber);
  virtual void messageReceived(std::string message);
  virtual void messageSent(std::string message);
  virtual void activeCallChanged(int callNumber);
  virtual void callComplete(int callNumber);
  virtual void inputOutputLevels(float inputLevel, float outputLevel);
  
  /* Utilities for startup and configuration */
  bool paramShow();
  bool paramRequestNotification();
  bool paramConnect();
  void setAlwaysOnTop(bool always);
  void addContact(Contact* contact);
  void addCDR(CDR* cdr);
  void saveParameter(std::string paramName, std::string paramValue);
  std::string readParameter(std::string paramName, std::string defaultResult = "0");
  std::string intToStr(int value);
  int strToInt(std::string value);
  void adjustDevices();
  void setCodecs();
  void setDefaultCodec(int defaultCodec);
  void i18n(QString localeStr);
  void saveGeometry();
  void restoreGeometry();
  void reloadJSONServers();
  void dialWithCallAppearance(Kiax2CallAppearance* dialog);
  static Kiax2MainWindow* instance;
  static Kiax2Servers* servers;
  static QString username;
  static QString password;
  static int routing; 
  map<int, Kiax2CallAppearance*> callMap;
  void queueCall(QString number, Account* account = NULL);
  
signals:
   void allCallsHungup();

public slots:

  void hangupCalls(int* callsList, int size);
  void applicationActivated();
  void widgetFocusChanged(QWidget* old, QWidget* now);
  void aboutAction_activated();
  void accountAction_activated();
  void registerAction_activated();
  void changePasswordAction_activated();
  void inviteFriendAction_activated();
  void callAction_activated();
  void callRecord_activated();
  void addContactAction_activated();
  void editContactAction_activated();
  void deleteContactAction_activated();
  void connectAction_activated();
  void disconnectAction_activated();
  void settingsAction_activated();
  void showKiaxWindow();
  void searchList(QString searchFilter);
  void refreshContactList(QString searchFilter);
  void populateCallRecords(QString searchFilter); 
  void refreshCallRecords(QString searchFilter);
  void showContactContextMenu(const QPoint& point);  
  void showCdrContextMenu(const QPoint& point);   
  void openCallDialog(QListWidget* listWidget, Kiax2CallAppearance* dialog = NULL, Contact* dialContact = NULL, Account* account = NULL);
  void openCallDialog(Kiax2CallAppearance* dialog, Contact* dialContact = NULL);
  void openCallDialog(Contact* dialContact, Account* account);  
  void openCallDialog(Contact* dialContact = NULL);
  void sysTrayActivated(QSystemTrayIcon::ActivationReason reason);  
  void checkComboText(QString text);
  void searchBoxActivated(QString text);
  void numberEditActivated();
  void showDialpad(bool visible);  
  void listSwitch();
  void ring();
  void exitKiax();
  void speedTestComplete(bool aborted, uint downloadSpeed);
  void jsonBalanceRequestInitiated();
  void jsonBalanceRequestComplete(bool reqCancelled, QString reqData);
  void jsonUsersOnlineRequestInitiated();
  void jsonUsersOnlineRequestComplete(bool reqCancelled, QString reqData);
  void jsonRequestAborted();
  void jsonRequestError();
  void jsonNotificationAvailable(QString notificationMessage, QString notificationUrl);
  void jsonSessionUpdated(QString notificationMessage, QString notificationUrl);
  void jsonServersRequestInitiated();
  void jsonServersRequestComplete(bool error, QString jsonMessage);
  void jsonServersRequestAborted();
  void jsonServersRequestError();
  
  void notificationBalloonClicked();
  void balanceTimeout();
  void sessionTimeout();
  void notificationTimeout();
  void balanceLabelClicked();
  void balanceMenuClicked();
  void webRegistrationSuccessful(QString username, QString password);
  void webPasswordChangeSuccessful(QString username, QString password);
  void showStatusMenu();
  void callRatesButtonClicked();
  void dtmf0();
  void dtmf1();
  void dtmf2();
  void dtmf3();
  void dtmf4();
  void dtmf5();
  void dtmf6();
  void dtmf7();
  void dtmf8();
  void dtmf9();
  void dtmfStar();
  void dtmfHash();
  void httpConfigureAccount(QString username, QString password, QString host1, QString host2);
  void httpChangePassword(QString username, QString password);
  void httpCall(QString number);  
  void httpHangupAndCall(QString number); 
  void httpHangupAll(); 
  void httpCallWithAccount(QString number, Account* account);
  bool containsUsb(QString name);  
   void httpHangup();  
 void enableUse();
  void disableUse();
  void login(QString username, QString password);
  void receiveMessage(QString ipcMessage);
	void updateFeed(QString host, QString number);
	void updateFeedIdle();
 void allCallsHungupSlot();
#ifdef TABBED_LAYOUT
  void hangupButtonClicked();
#endif
#ifdef USE_WEBFEED
  void feedLinkClicked(const QUrl& link);
  void feedLoaded();
#endif
protected:

  QMenu* contactContextMenu;
  QMenu* cdrContextMenu;
  QMenu* statusMenu;
  QMenu* sysTrayMenu;
  QSystemTrayIcon* sysTray;
  virtual void closeEvent(QCloseEvent* event);
  QTimer* ringTimer;
  virtual void changeEvent(QEvent* event);
  
private:

	Qt::WindowFlags flags;
	Ui::MainWindow ui;
	map<int, Kiax2CallDialog*> callDialogs;
	APIBinder* api;
	Account* defaultAccount;

	void checkDefaultAccount();
	void startRing();
	void stopRing();
    iaxc_sound*  initTone(int F1, int F2, int Dur, int Len, int Repeat);
	struct iaxc_sound* ringInTone;
	Kiax2ConnectionMeter* connectionMeter;
	bool isLocalNetwork();
	void testNetwork(bool testSpeed = false);
	void updateIdentity(QString username, QString password, QString host1 = "", QString host2 = "");
	void appendNumberToNumberEdit(QString number);
	void reconnect();
	void initApiServer();
	QString deviceHash(std::map<int, std::string> devices);
	QString generateSession();
	QString decodeExtension(QString number, bool decodeHost = false, bool decodeExtension = false);
	QString readFeedUrl();
	void saveFeedUrl(QString url);
#ifdef USE_DOTTEL	
	QString openDotTelDialog(QString domain);
	QString encodeExtension(QString mode, QString extension, QString host, QString session);
#endif	
	Kiax2JSONBalance* jsonBalance;
	Kiax2JSONUsersOnline* jsonUsersOnline;
	Kiax2JSONNotification* jsonNotification;
	Kiax2JSONNotification* jsonSession;
	QString jsonBalanceRequestUrl;
	QString jsonUsersOnlineRequestUrl;
	void updateBalanceParams();
	QTimer* balanceTimer;
	QTimer* notificationTimer;
	QTimer* sessionTimer;
	QTimer* feedTimer;
	uint lastBalanceUpdateByClick;
#ifdef USE_WEBKIT	
	Kiax2RegisterDialog* registerDialog;
	Kiax2ChangePassDialog* changePasswordDialog;
#endif
	QString serviceMenuTitle;
	Qt::WindowFlags windowFlags;
	QTranslator* appTranslator;
	Kiax2Application* application;
	QListWidget* callRecords;
	Kiax2DispatcherServer* apiServer;
	Kiax2JSONServers* jsonServers;	
	//QPushButton* callRatesButton;
	QAction* actionCallRates;
	QAction* actionCredit;
	QLabel* onlineUsersButton;
	Kiax2LoginScreen* loginScreen;
	QRect mainWindowGeometry;
	bool exitOnCallEnd;
	void parseCommandParameters(int &argc, char *argv[]);
	bool readAccountData();	
	QString notificationUrl;
	QString lastNotification;
	QString lastNotificationUrl;
	bool callWindowsOnTop;
	QButtonGroup* callButtonsGroup;
	QString encodeSession;
	QList<Contact*> callQueue;
};

#endif

