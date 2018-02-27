#include "CDRModuleImpl.h"

#include "Core.h"

CDRModule* CDRModule::singleton;

AbstractModule* CDRModuleImpl::instance()
{
	if (singleton==NULL)
	{
		singleton = new CDRModuleImpl();
	}
	return singleton;
}

CDRModuleImpl::CDRModuleImpl()
{
	moduleName = "CDRModuleImpl";
	moduleType = "CDRModule";
}

void CDRModuleImpl::init()
{
	Core* core = Core::instance();
	storageService = core->storageService;
	Logger::log(Logger::INFO, "Module %s initialized.\n", moduleName.data());
}

void CDRModuleImpl::dispose()
{
    Logger::log(Logger::INFO, "Module %s disposed.\n", moduleName.data());
}

void CDRModuleImpl::logCDR(CDR* cdr)
{
	bool createResult = (dynamic_cast<StorageService*>(storageService))->createObject(cdr);
	if (createResult)
		Logger::log(Logger::DEBUG, "CDR created successfully.\n");
	else
		Logger::log(Logger::SEVERE, "CDR failed\n");	
}

std::vector<CDR*> CDRModuleImpl::getCDRs(std::string filter)
{
	Logger::log(Logger::DEBUG, "getCDRs(%s) IN\n", filter.data());
	StorageService* storage = dynamic_cast<StorageService*> (storageService);
	std::string objType = "CDR";
	std::vector<Serializable*> cdrs;
	std::vector<CDR*> cdrs_;
	string sqlFilter = "srcExt like '%" + filter + "%' OR dstExt like '%" + 
							filter + "%' OR callState like '%" + filter + "%' OR cdrInfo like '%" +
							filter + "%' OR dstName like '%" +
							filter + "%' order by rowid asc";
	cdrs = storage->findObject(objType, sqlFilter);
	 
	for (unsigned int i = 0; i<cdrs.size(); i++) {
		CDR* cdr = dynamic_cast<CDR*> (cdrs.at(i));
		cdrs_.push_back(cdr);
		Logger::log(Logger::DEBUG, "Found object CDR objectId=%d\n",
				cdr->getObjectId());
	}
	Logger::log(Logger::DEBUG, "getCDRs(%s) OUT\n", filter.data());
	return cdrs_;
}

std::vector<CDR*> CDRModuleImpl::getCDRs()
{
	Logger::log(Logger::DEBUG, "getCDRs() IN\n");
	StorageService* storage = dynamic_cast<StorageService*> (storageService);
	std::string objType = "CDR";
	std::vector<CDR*> cdrs_;
	std::string filter = "rowid>=0 order by rowid asc";
	for (unsigned int i = 0; i<cdrs.size(); i++) {
		CDR* cdr = dynamic_cast<CDR*> (cdrs.at(i));
		if (cdr!=NULL)
		{
			delete cdr;
			cdr = NULL;
		}
	}
	cdrs.clear();
	cdrs = storage->findObject(objType, filter);

	for (unsigned int i = 0; i<cdrs.size(); i++) {
		CDR* cdr = dynamic_cast<CDR*> (cdrs.at(i));
		cdrs_.push_back(cdr);
		Logger::log(Logger::DEBUG, "Found object CDR objectId=%d\n",
				cdr->getObjectId());
	}
	Logger::log(Logger::DEBUG, "getCDRs() OUT\n");
	return cdrs_;
}

