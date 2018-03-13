/* Constants */
#define LOG_LEVEL_NOTHING				0
#define LOG_LEVEL_INFO					1
#define LOG_LEVEL_ERROR					2
#define LOG_LEVEL_WARNING				3
#define LOG_LEVEL_DEBUG					4

/* Services */
#define SERVICE_TYPE_NAMING				"Naming"
#define SERVICE_TYPE_STORAGE			"Storage"
#define HARDCODED_CDR_MODULE			CDRModuleImpl
#define HARDCODED_ACCOUNTS_MODULE		AccountsModuleImpl
#define HARDCODED_CONTACTS_MODULE		ContactsModuleImpl
#define HARDCODED_SIGNALING_MODULE		SignalingModuleIAX2
#define HARDCODED_CONFIGURATION_MODULE	ConfigurationModuleImpl
#define HARDCODED_NAMING_SERVICE		NamingService
#define HARDCODED_STORAGE_SERVICE		StorageService

#define PROTOCOL_NONE					0;
#define PROTOCOL_IAX2					1;
#define PROTOCOL_SIP					2;
#define PROTOCOL_XMPP					3;

#define CALL_NONE						0
#define CALL_INACTIVE					1
#define CALL_RINGING					2
#define CALL_COMPLETE					3
#define CALL_INCOMING					4
#define CALL_OUTGOING					5

/* CDR Constants for fields direction and state */
#define CALL_DIRECTION_NONE				""
#define CALL_DIRECTION_OUTGOING			"out"
#define CALL_DIRECTION_INCOMING			"in"
#define CALL_STATE_NONE					""
#define CALL_STATE_ANSWERED				"answered"
#define CALL_STATE_UNANSWERED			"unanswered"
#define CALL_STATE_REJECTED				"rejected"

#define HTTP_COMMAND_SETUP_ACCOUNT		"configureAccount"
#define HTTP_COMMAND_CHANGE_PASSWORD	"changePassword"

