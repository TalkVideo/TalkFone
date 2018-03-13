#define SOFTPHONE_VERSION				"2.1.0-Linux-x86"
#define MAC_BINARY_DIR					"Kiax.app/Content/MacOS"
#define LOG_LEVEL 						LOG_LEVEL_INFO
#define PROFILE_SUBDIR					"/.kiax/"
#define DB_NAME							"persist.db"
#define NAMING_SERVICE_NAME				"Naming"
#define STORAGE_SERVICE_NAME			"Storage"
#define DEFAULT_CDR_MODULE_NAME			"CDRModuleImpl"
#define DEFAULT_CONTACTS_MODULE_NAME	"ContactsModuleImpl"
#define DEFAULT_ACCOUNTS_MODULE_NAME	"AccountsModuleImpl"
#define DEFAULT_SIGNALING_MODULE_NAME	"SignalingModuleIAX2"
#define DEFAULT_CONFIGURATION_MODULE_NAME "ConfigurationModuleImpl"
#define DEFAULT_CALLS_NUMBER			3
#define MAX_CDR_RECORDS					10
#define DEFAULT_REGISTER_HOST			"localhost"
#define SECOND_REGISTER_HOST			"localhost"
#define THIRD_REGISTER_HOST				"localhost"
#define ENABLE_TOOLBAR					true
#define PRODUCT_NAME					"Kiax Softphone"
#define PRODUCT_LICENSE					"Licensed under GPLv3"
#define DEFAULT_SERVICE_NAME			"Kiax"
#define MENU_SERVICE_LABEL				tr("VoIP")
#define GUEST_ACCOUNT					""
#define GUEST_PASSWORD					""
#define CALL_RATES_TEXT					tr("Click to view call rates")
#define ENABLE_SPEEDTEST				false
#define ENABLE_BALANCE					false
#define ENABLE_CALL_RATES				false
#define ENABLE_WEB_SERVICES				false
#define ENABLE_SERVER_FIELD				true
#define INVITE_FRIEND_URL				"http://localhost"
#define CALL_RATES_URL					"http://localhost/webcallingrates"
#define GET_ACCOUNT_URL					"http://www.forschung-direkt.eu"
#define LOGIN_URL						"https://localhost/users/login"
#define CHANGE_PASSWORD_URL				"https://localhost/users/password"
#define JSON_BALANCE_REQUEST_URL		"https://localhost/profiles/billing"
#define HTTP_DOWNLOAD_SPEEDTEST_URL		"http://localhost/speed-test-download"
#define REGISTRATION_SUCCESS_FORM_NAME	"mixvoip_SIGNUPSUCCESS"
#define PASS_CHANGE_SUCCESS_FORM_NAME	"mixvoip_PASSWORDCHANGESUCCESS"
#define FORM_NAME_JS_EXPRESSION			"document.forms[0].name"
#define USERNAME_JS_EXPRESSION			"document.forms[0].elements[0].value"
#define PASSWORD_JS_EXPRESSION			"document.forms[0].elements[1].value"
#define IAX2_CAPABILITIES_SPEEX			IAXC_FORMAT_SPEEX
#define IAX2_CAPABILITIES_ULAW			IAXC_FORMAT_ULAW
#define IAX2_CAPABILITIES_ILBC			IAXC_FORMAT_ILBC
#define ENABLE_ILBC_AND_GSM				true
#define IAX2_CAPABILITIES_GSM			IAXC_FORMAT_GSM
#define IAX2_FILTERS					IAXC_FILTER_DENOISE | IAXC_FILTER_CN
#define DEFAULT_LAN_CODEC				IAXC_FORMAT_GSM
#define DEFAULT_INTERNET_CODEC			IAXC_FORMAT_GSM
#define DEFAULT_WINDOW_POSITION_X		300
#define DEFAULT_WINDOW_POSITION_Y		200
#define DEFAULT_WINDOW_SIZE_WIDTH		190
#define DEFAULT_WINDOW_SIZE_HEIGHT		500
#define REGISTER_DIALOG_WIDTH			500
#define REGISTER_DIALOG_HEIGHT			500
#define CHANGE_PASS_DIALOG_WIDTH		500
#define CHANGE_PASS_DIALOG_HEIGHT		500
#define ULAW_FRIENDLY_NAME				tr("ULAW (Uncompressed voice)")
#define SPEEX_FRIENDLY_NAME				tr("Speex (Dynamic voice compression)")
#define GSM_FRIENDLY_NAME				tr("GSM (Low traffic, GSM-voice quality)")
#define ILBC_FRIENDLY_NAME				tr("iLBC (Internet Low-Bitrate codec)")
#define HTTP_API_PORT			54345
#define ENABLE_HTTP_CONFIGURATION		false
#define USE_HTTP_CONFIGURATION			false	/* set to false if webkit forms should be the default (if enabled) */
#define HTTP_GET_ACCOUNT_URL			"http://www.forschung-direkt.eu"
#define HTTP_CHANGE_PASSWORD_URL		"http://localhost/changepass"
#define HTTP_CONFIG_ACTION_CONFIGURE_ACCOUT		"newuser"
#define HTTP_CONFIG_ACTION_CHANGE_PASSWORD		"changePassword"
#define HTTP_CONFIG_PARAM_SESSION		"sessionkey"
#define HTTP_CONFIG_PARAM_USERNAME		"username"
#define HTTP_CONFIG_PARAM_PASSWORD		"password"
#define HTTP_CONFIG_PARAM_HOST			"server1"
#define HTTP_CONFIG_PARAM_FALLBACK_HOST	"server2"
#define HTTP_CONFIG_200OK_CONTENT		"<div/>"
#define AUTO_CREATE_GUEST_ACCOUNT		true
#define ENABLE_LOGIN_SCREEN				false
#define ENABLE_NEW_ACCOUNT_DIALOG		true
#define ALLOW_CALLS_WITH_NO_ACC			true
#define AUTO_DEFAULTS_WINDOW_SHOW		false
#define JSON_USERS_ONLINE_REQUEST_URL	"https://localhost"
#define ENABLE_SHOW_HIDE_COMMANDS		true
#define CALL_RATES_FONT					8
#define SINGLE_CONNECT_DISCONNECT		true
#define ABOUT_URL						"<a href='http://www.kiax.org'>http://www.kiax.org</a>"
#define SHOW_DIALPAD_TEXT		tr("Show Dialpad")
#define HIDE_DIALPAD_TEXT		tr("Hide Dialpad")
#define NOTIFICATION_ENABLED		false
#define NOTIFICATION_SETTINGS_ENABLED	false
#define NOTIFICATION_REQUEST_URL	"https://localhost"
#define JSON_USERNAME_PARAMETER		"username"
#define JSON_PASSWORD_PARAMETER		"password"
#define JSON_SOFTPHONEVERSION_PARAMETER	"softphoneversion"
#define JSON_NOTIFICATION_TIMER		900000	/* 15 minutes */
#define JSON_NOTIFICATION_ANONYMOUS	true
#define CREATE_ECHO_TEST_CONTACT	false
#define ECHO_TEST_CONTACT_NUMBER	"+35220333000"
#define ECHO_TEST_CONTACT_NAME		"Voice Echo Test"
#define STATUS_CONNECTED_LABEL		tr("Connected ")
#define STATUS_DISCONNECTED_LABEL	tr("Disconnected ")
#define STATUS_TIMEDOUT_LABEL		tr("Timeout ")
#define CALL_WINDOW_STAYS_ON_TOP	true
#define HTTP_ACTION_PARAM_NUMBER		"number"
#define HTTP_CONFIG_ACTION_CALL			"call"
/* Merged from Kiax2Agent project */
#define AGENT_SOFTPHONE_REDIRECT_PORT 54345
#define AGENT_SOFTPHONE_REDIRECT_HOST "127.0.0.1"
#define AGENT_DEFAULT_SERVER_REDIRECT_HOST "api.mixvoip.com"
#define AGENT_ROOT_API_URL "https://api.dev.mixvoip.com/users/active/API"
#define AGENT_DEFAULT_SERVER_REDIRECT_PORT 80
/* ROUTING VALUES CORRESPOND TO GUI COMBO BOX ORDER */
#define AGENT_ROUTING_SOFTPHONE 0
#define AGENT_ROUTING_SERVER 1
#define AGENT_ROUTING_LOCAL 2 /* indicates that http requests are sent to local pbx */
/****/
// #define TABBED_LAYOUT /* uncomment to enable the alternative TABBED layout */
#define SHOW_STATUS_BUTTON 		true
#define SHOW_LOGIN_SCREEN_ON_DISCONNECT		false
#define ALWAYS_USE_LOGIN_SCREEN 	false
#define ENABLE_ONLINE_USERS		false
#define KIAX_MODE			true
#define SUBMIT_BALANCE_PASSWORD		false
#define BALANCE_XML			false
#define BALANCE_CLICKABLE		false
#define BALANCE_LABEL_TEXT		tr("Balance:")
#define BALANCE_DEFAULT_CURRENCY	"\u20ac"
#define CREDIT_LINK_IN_SERVICE_MENU	false
#define ADVANCED_SETTINGS		false
#define HTTP_CDR_FETCH_URL		"" 


#define ENCODE_TEL_EXTEN		false
#define TEL_EXTEN_ENCODING		""
#define SESSION_TIMEOUT			600000
#define DISABLE_ACCOUNT_MENU		false
#define DISABLE_CONNECT			false
#define SESSION_REQUEST_URL		"https://localhost/general/telonline"
#define IDLE_FEED_URL			"https://localhost/general/telcontentidle"
#define CALL_FEED_URL			"https://localhost/general/telcontentcall"
#define ENABLE_SESSION_UPDATE		false
#define DISABLE_ACCOUNT_CONFIGURE_MENU	false
#define DISABLE_DIALPAD_FEATURE		false
#define DISABLE_HISTORY_ADD_CONTACT	false
#define DISABLE_CODEC_SETTING		false
#define DISABLE_AUTOCONNECT_SETTING	false
#define DISABLE_USERNAME_TITLE		false
#define FEED_TIMEOUT			30000	/* half a minute */
#define ENABLE_FEED_TIMER		false
#define ENABLE_CALL_FEED		false
// #define DEFAULT_CALL_RECORDS_HEIGHT	80
#define USB_NAME_PATTERN		"USB"
#define ALLOW_ONLY_TEL			false
#define HOME_PROFILE			true
#define DEFAULT_DIALPAD_OPEN	true

