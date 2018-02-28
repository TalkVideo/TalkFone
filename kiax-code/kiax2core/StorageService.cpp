#include "StorageService.h"
#include <dirent.h>
#include <unistd.h>
#include <cstdlib>
#ifdef WIN32DEP
#include <windows.h>
#include "setupapi.h"
#include <dbt.h>						// For DeviceChange.
#include <winioctl.h>
#endif
#ifdef MACXDEP
#include <fcntl.h>
#include <sys/stat.h>
#endif

#ifdef WIN32DEP

typedef enum _STORAGE_BUS_TYPE {
    BusTypeUnknown = 0x00,
    BusTypeScsi,
    BusTypeAtapi,
    BusTypeAta,
    BusType1394,
    BusTypeSsa,
    BusTypeFibre,
    BusTypeUsb,
    BusTypeRAID,
    BusTypeMaxReserved = 0x7F
} STORAGE_BUS_TYPE, *PSTORAGE_BUS_TYPE;

typedef struct _STORAGE_DEVICE_DESCRIPTOR {
  ULONG  Version;
  ULONG  Size;
  UCHAR  DeviceType;
  UCHAR  DeviceTypeModifier;
  BOOLEAN  RemovableMedia;
  BOOLEAN  CommandQueueing;
  ULONG  VendorIdOffset;
  ULONG  ProductIdOffset;
  ULONG  ProductRevisionOffset;
  ULONG  SerialNumberOffset;
  STORAGE_BUS_TYPE  BusType;
  ULONG  RawPropertiesLength;
  UCHAR  RawDeviceProperties[1];

} STORAGE_DEVICE_DESCRIPTOR, *PSTORAGE_DEVICE_DESCRIPTOR;

// retrieve the properties of a storage device or adapter. 
typedef enum _STORAGE_QUERY_TYPE {
  PropertyStandardQuery = 0,
  PropertyExistsQuery,
  PropertyMaskQuery,
  PropertyQueryMaxDefined

} STORAGE_QUERY_TYPE, *PSTORAGE_QUERY_TYPE;

// retrieve the properties of a storage device or adapter. 
typedef enum _STORAGE_PROPERTY_ID {
  StorageDeviceProperty = 0,
  StorageAdapterProperty,
  StorageDeviceIdProperty

} STORAGE_PROPERTY_ID, *PSTORAGE_PROPERTY_ID;

// retrieve the properties of a storage device or adapter. 
typedef struct _STORAGE_PROPERTY_QUERY {
  STORAGE_PROPERTY_ID  PropertyId;
  STORAGE_QUERY_TYPE  QueryType;
  UCHAR  AdditionalParameters[1];

} STORAGE_PROPERTY_QUERY, *PSTORAGE_PROPERTY_QUERY;

#define IOCTL_STORAGE_QUERY_PROPERTY   CTL_CODE(IOCTL_STORAGE_BASE, 0x0500, METHOD_BUFFERED, FILE_ANY_ACCESS)

BOOL GetDisksProperty(HANDLE hDevice, PSTORAGE_DEVICE_DESCRIPTOR pDevDesc)
{
	STORAGE_PROPERTY_QUERY	Query;	// input param for query
	DWORD dwOutBytes;				// IOCTL output length
	BOOL bResult;					// IOCTL return val

	// specify the query type
	Query.PropertyId = StorageDeviceProperty;
	Query.QueryType = PropertyStandardQuery;

	// Query using IOCTL_STORAGE_QUERY_PROPERTY 
	bResult = ::DeviceIoControl(hDevice,			// device handle
			IOCTL_STORAGE_QUERY_PROPERTY,			// info of device property
			&Query, sizeof(STORAGE_PROPERTY_QUERY),	// input data buffer
			pDevDesc, pDevDesc->Size,				// output data buffer
			&dwOutBytes,							// out's length
			(LPOVERLAPPED)NULL);					

	return bResult;
}



#endif

static int findObjectsCallback(void *NotUsed, int argc, char **argv, char **azColName)
{
  for(int i=0; i<argc; i++)
  {
    string rowid = string(argv[i]);
    StorageService::instance()->rowids.push_back(rowid);
  }
  return 0;
}

static int cdrCallback(void *NotUsed, int argc, char **argv, char **azColName)
{
  StorageService* instance = StorageService::instance();
  std::string rowid = string(argv[0]);
  std::string cdrTime = string(argv[1]);
  std::string cdrInfo = string(argv[2]);
  std::string srcExt = string(argv[3]);
  std::string dstExt = string(argv[4]);
  std::string callState = string(argv[5]);
  std::string direction = string(argv[6]);
  std::string duration = string(argv[7]);
  std::string dstName = string(argv[8]);
  
  CDR* cdr = dynamic_cast<CDR*> (instance->objectsPool.at(instance->objectsPool.size()-1));
  cdr->setObjectId(atol(rowid.data()));
  cdr->cdrTime = cdrTime;
  cdr->cdrInfo = cdrInfo;
  cdr->srcExt = srcExt;
  cdr->dstExt = dstExt;
  cdr->callState = callState;
  cdr->direction = direction;
  cdr->duration = duration;
  cdr->dstName = dstName;
  return 0;
}

static int contactCallback(void *NotUsed, int argc, char **argv, char **azColName)
{
  StorageService* instance = StorageService::instance();
  string rowid = string(argv[0]);
  string contactName = string(argv[1]);
  string extension = string(argv[2]); 
  Contact* contact = dynamic_cast<Contact*>(instance->objectsPool.at(instance->objectsPool.size()-1));
  contact->setObjectId(atol(rowid.data()));
  contact->contactName = contactName;
  contact->extension = extension;
  return 0;
}

static int accountCallback(void *NotUsed, int argc, char **argv, char **azColName)
{
  StorageService* instance = StorageService::instance();
  string rowid = string(argv[0]);
  string serviceName = string(argv[1]);
  string username = string(argv[2]);
  string password = string(argv[3]);
  string host1 = string(argv[4]);
  string host2 = string(argv[5]);
  string host3 = string(argv[6]);
  Account* acc = dynamic_cast<Account*> (instance->objectsPool.at(instance->objectsPool.size()-1));
  acc->setObjectId(atol(rowid.data()));
  acc->serviceName = serviceName;
  acc->username = username;
  acc->password = password;
  acc->host1 = host1;
  acc->host2 = host2;
  acc->host3 = host3;
  return 0;
}

static int configParamCallback(void *NotUsed, int argc, char **argv, char **azColName)
{
  StorageService* instance = StorageService::instance();
  string rowid = string(argv[0]);
  string paramName = string(argv[1]);
  string paramType = string(argv[2]);
  string paramValue = string(argv[3]);
  ConfigurationParameter* param = dynamic_cast<ConfigurationParameter*> (instance->objectsPool.at(instance->objectsPool.size()-1));
  param->setObjectId(atol(rowid.data()));
  param->name = paramName;
  param->paramType = paramType;
  param->value = paramValue;
  return 0;
}

StorageService* StorageService::singleton;

StorageService::~StorageService()
{
	delete singleton;
	singleton = NULL;
}

StorageService* StorageService::instance()
{
	std::string fullDbPath;
	if (StorageService::singleton==NULL)
	{
		string profileDir = StorageService::touchProfileDir(HOME_PROFILE);
		fullDbPath = profileDir + DB_NAME;
		StorageService::singleton = new StorageService(fullDbPath);
	}
	return StorageService::singleton;
}

int touchDir(const char* dirName);

std::string StorageService::touchProfileDir(bool homeDir)
{
	char *path=new char[1024];
	size_t size = 1023;
	path=getcwd(path,size);
	Logger::log(Logger::DEBUG, "Current working directory is: %s\n", path);
	std::string resultDir;
	std::string strPath(path);
	const char* driveLetter = strPath.substr(0,2).data();
	
	unsigned int driveType = 1;
#ifdef WIN32DEP
	driveType = GetDriveTypeA(driveLetter);
	if (driveType==3)	//in case it registers itself as fixed drive, lets check if it is a usb drive again
	{
		HANDLE			hDevice;
		PSTORAGE_DEVICE_DESCRIPTOR pDevDesc;
		const WCHAR deviceNameFormat[]= L"\\\\.\\%c:";
		WCHAR deviceNameBuffer[300];
		swprintf(deviceNameBuffer, deviceNameFormat, driveLetter[0]);
		hDevice = CreateFile(deviceNameBuffer, 0,
							FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
		if (hDevice != INVALID_HANDLE_VALUE)
		{
			pDevDesc = (PSTORAGE_DEVICE_DESCRIPTOR)new BYTE[sizeof(STORAGE_DEVICE_DESCRIPTOR) + 512 - 1];
			pDevDesc->Size = sizeof(STORAGE_DEVICE_DESCRIPTOR) + 512 - 1;
			Logger::log(Logger::INFO, "Getting disk property..\n");
			if(GetDisksProperty(hDevice, pDevDesc))
			{
					Logger::log(Logger::INFO, "Checking bus type..\n");
					if(pDevDesc->BusType == BusTypeUsb)
					{
						Logger::log(Logger::INFO, "Bustype is USB!\n");
						driveType = 2; // so we are USB anyway, force orverwrite
					}
			}		
		}
		else
		{
			Logger::log(Logger::INFO, "Invalid device handle!\n");	
		}
	}
#endif
    Logger::log(Logger::DEBUG, "drive %s is of type %d\n", driveLetter, driveType);
	
	if ((driveType == 2)||(!HOME_PROFILE)) {
		resultDir.append(path);
		resultDir.append(PROFILE_SUBDIR);
		if (driveType==2)
			Logger::log(Logger::DEBUG, "drive is removable\n");
		if (!HOME_PROFILE)
			Logger::log(Logger::DEBUG, "using current working dir %s as storage folder.\n", resultDir.data());
	} else {
		Logger::log(Logger::DEBUG, "drive is not removable\n");
#ifdef WIN32DEP
		char* envVar = getenv("USERPROFILE");
#endif
#if defined(LINUXDEP) || defined(MACXDEP)
		char* envVar = getenv("HOME");
#endif
		if (envVar==NULL) 
			resultDir = ""; // default
		else
		{
			string profileDir(envVar);
			resultDir = profileDir + PROFILE_SUBDIR;
		}
		Logger::log(Logger::DEBUG, "DB Path:%s\n", resultDir.data());
	}
	Logger::log(Logger::DEBUG, "Returning final db directory = %s\n", resultDir.data());
	touchDir(resultDir.data());

	return resultDir;
	
}

int StorageService::touchDir(const char* dirName)
{
	int result = 0;
	DIR* pdir = opendir(dirName);
	
	if (!pdir)
	{
		Logger::log(Logger::DEBUG, "Creating directory %s\n", dirName);
#ifdef WIN32DEP
		result = mkdir(dirName);
#endif
#if defined(LINUXDEP) || defined(MACXDEP)
		result = mkdir(dirName,  S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
#endif
		if (result<0)
			Logger::log(Logger::DEBUG, "Could not create directory %s\n", dirName);
	}
	else 
		Logger::log(Logger::DEBUG, "Directory %s exists.\n", dirName);
	
	return result;
}

StorageService::StorageService(std::string& name) : AbstractService() 
{
	dbName = name;
	serviceType = SERVICE_TYPE_STORAGE;
	mutex = sqlite3_mutex_alloc(SQLITE_MUTEX_FAST);
}

bool StorageService::start()
{
	sqlite3_mutex_enter(mutex);
	int startResult;
	startResult = sqlite3_open(dbName.data(), &db);
	if( startResult ){
	    Logger::log(Logger::SEVERE, "Can't open database: %s\n", sqlite3_errmsg(db));
	    sqlite3_close(db);
	    serviceErrCode = 1; // unable to open database
		sqlite3_mutex_leave(mutex);
	    return false;
	}
	started = true;
	serviceErrCode = 0;
	sqlite3_mutex_leave(mutex);
	return true;
}

bool StorageService::stop()
{
	sqlite3_mutex_enter(mutex);
	sqlite3_close(db);
	started = false;
	serviceErrCode = 0;
	sqlite3_mutex_leave(mutex);
	return true;
}

bool StorageService::createObject(Serializable* proto)
{
	sqlite3_mutex_enter(mutex);
	char *zErrMsg = 0;
	int qeResult;
	if (proto == NULL)
	{
		serviceErrCode = 1; // unsupported type
		sqlite3_mutex_leave(mutex);
		return false;
	}
	string createStatement = proto->getCreateTypeQuery();
	qeResult = sqlite3_exec(db, createStatement.data(), 0, 0, &zErrMsg);
	if (qeResult)
	{
		serviceErrCode = 2; // create or insert failed
		sqlite3_mutex_leave(mutex);
		return false;
	}
	string insertStatement = proto->getCreateQuery();
	qeResult = sqlite3_exec(db, insertStatement.data(), 0, 0, &zErrMsg);
	sqlite3_int64 lastrowid = sqlite3_last_insert_rowid(db);
	if (qeResult)
	{
		serviceErrCode = 2; // create or insert failed
		sqlite3_mutex_leave(mutex);
		return false;
	}
	proto->setObjectId((long int) lastrowid);
	serviceErrCode = 0;
	sqlite3_mutex_leave(mutex);
    return true;
}

bool StorageService::updateObject(Serializable* proto)
{
	sqlite3_mutex_enter(mutex);
	char *zErrMsg = 0;
	int qeResult;
	if (proto == NULL)
	{
		serviceErrCode = 1; // unsupported type
		sqlite3_mutex_leave(mutex);
		return false;
	}
	Logger::log(Logger::DEBUG, "Updating object %s with objectId=%d\n", proto->getObjectType().data(), proto->getObjectId());
	if (proto->getObjectId()==0)
	{
		Logger::log(Logger::DEBUG, "Object %s will be created.\n", proto->getObjectType().data());
		bool createResult = createObject(proto);
		if (!createResult)
		{
			serviceErrCode = 2; // create or insert failed
			sqlite3_mutex_leave(mutex);
			return false;
		}
	}
	string updateStatement = proto->getStoreQuery();
	qeResult = sqlite3_exec(db, updateStatement.data(), 0, 0, &zErrMsg);
	if (qeResult)
	{
		serviceErrCode = 2; // create or insert failed
		sqlite3_mutex_leave(mutex);
		return false;
	}
	serviceErrCode = 0;
	sqlite3_mutex_leave(mutex);
    return true;
}

std::vector<Serializable*> StorageService::findObject(std::string& objectType, std::string& criteria)
{
	sqlite3_mutex_enter(mutex);
	char *zErrMsg = 0;
	int qeResult;
	std::vector<Serializable*> objects;
	string selectStatement = "select rowid from %s where %s";
	char query[1024];
	sprintf(query, selectStatement.data(), objectType.data(), 
							criteria.data());
	Logger::log(Logger::DEBUG, "find query = %s\n", query);
	qeResult = sqlite3_exec(db, query, findObjectsCallback, 0, &zErrMsg);
	if (qeResult)
	{
		serviceErrCode = 2; // create or insert failed
		sqlite3_mutex_leave(mutex);
		return objects;
	}

	std::vector<std::string>::iterator itVectorData;
	for(itVectorData = rowids.begin(); itVectorData != rowids.end(); itVectorData++)
	{
		std::string rowid = *(itVectorData);
		Serializable* newObject;
		if (objectType.compare("CDR")==0) 
		{
			newObject = new CDR();
			objectsPool.push_back(newObject);
			string loadStatement = newObject->getLoadQuery(atol(rowid.data()));
			qeResult = sqlite3_exec(db, loadStatement.data(), cdrCallback, 0, &zErrMsg);
			if (qeResult)
			{
				serviceErrCode = 2; // create or insert failed
				sqlite3_mutex_leave(mutex);
				return objects;
			}	
		}
		else if (objectType.compare("Contact")==0) 
		{
			newObject = new Contact();
			objectsPool.push_back(newObject);
			string loadStatement = newObject->getLoadQuery(atol(rowid.data()));
			qeResult = sqlite3_exec(db, loadStatement.data(), contactCallback, 0, &zErrMsg);
			if (qeResult)
			{
				serviceErrCode = 2; // create or insert failed
				sqlite3_mutex_leave(mutex);
				return objects;
			}	
		}
		else if (objectType.compare("Account")==0)
		{
			newObject = new Account();
			objectsPool.push_back(newObject);
			string loadStatement = newObject->getLoadQuery(atol(rowid.data()));
			qeResult = sqlite3_exec(db, loadStatement.data(), accountCallback, 0, &zErrMsg);
			if (qeResult)
			{
				serviceErrCode = 2; // create or insert failed
				sqlite3_mutex_leave(mutex);
				return objects;
			}	
		}
		else if (objectType.compare("ConfigurationParameter")==0) 
		{
			newObject = new ConfigurationParameter();
			objectsPool.push_back(newObject);
			string loadStatement = newObject->getLoadQuery(atol(rowid.data()));
			qeResult = sqlite3_exec(db, loadStatement.data(), configParamCallback, 0, &zErrMsg);
			if (qeResult)
			{
				serviceErrCode = 2; // create or insert failed
				sqlite3_mutex_leave(mutex);
				return objects;
			}	
		}
		else if ((objectType.compare("Configuration"))==0) 
		{
			newObject = new Configuration();
		}
		else
		{
			serviceErrCode = 1; // unsupported type
			sqlite3_mutex_leave(mutex);
			return objects;
		}
	
		objects.push_back(newObject);
	}

	rowids.erase(rowids.begin(), rowids.end());
	objectsPool.erase(objectsPool.begin(), objectsPool.end());
	serviceErrCode = 0;
	sqlite3_mutex_leave(mutex);
	return objects;
}

bool StorageService::deleteObject(Serializable* object)
{
	sqlite3_mutex_enter(mutex);
	char *zErrMsg = 0;
	int qeResult;
	if (object == NULL)
	{
		serviceErrCode = 1; // unsupported type
		sqlite3_mutex_leave(mutex);
		return false;
	}
	string deleteStatement = object->getDeleteQuery();
	qeResult = sqlite3_exec(db, deleteStatement.data(), 0, 0, &zErrMsg);
	if (qeResult)
	{
		serviceErrCode = 2; // create or insert failed
		sqlite3_mutex_leave(mutex);
		return false;
	}
	serviceErrCode = 0;
	sqlite3_mutex_leave(mutex);
	return true;
}
