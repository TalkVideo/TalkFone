#include "CppDotTelUtils.h"
#include <iostream>

CppDotTelUtils::CppDotTelUtils()
{
}

CppDotTelUtils::~CppDotTelUtils()
{
}

bool CppDotTelUtils::init()
{
	return initDotTel();
}

void CppDotTelUtils::destroy()
{
	destroyDotTel();
}

vector<CppTelRecord> CppDotTelUtils::getData(int dataType, string domainName)
{
	int reccount;
	vector<CppTelRecord> results;
	struct TelRecord* c_results = getTelData(dataType, (char*) domainName.c_str(), &reccount);
	if (c_results!=NULL)
	{
		for (int i=0; i<reccount; i++)
		{
			CppTelRecord record;
			record.content = c_results[i].content;
			record.ph_type = c_results[i].ph_type;
			if (c_results[i].descriptor!=NULL)
				record.descriptor = c_results[i].descriptor;
			results.push_back(record);
		}
	 deleteTelArray(c_results, reccount);
	}
	return results;
}

vector<CppTelRecord> CppDotTelUtils::getNumbers(string domainName)
{
	return getData(TEL_DATA_TEL, domainName);
}

vector<CppTelRecord> CppDotTelUtils::getDescriptions(string domainName)
{
	return getData(TEL_DATA_TXT, domainName);
}

string CppDotTelUtils::phTypeToStr(enum phone_type p_type)
{
	string result;
	char* c_result;
	phoneTypeToStr(p_type, &c_result);
	if (c_result!=NULL)
		result = c_result;
	free(c_result);
	return result;
}

