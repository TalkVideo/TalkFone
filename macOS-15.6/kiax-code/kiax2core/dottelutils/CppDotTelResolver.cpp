#include "CppDotTelUtils.h"
#include <iostream>

int main(int argc, char *argv[])
{
	if (argc==2)
	{
		vector<CppTelRecord> numbers;
		vector<CppTelRecord> descriptions;
		
		CppDotTelUtils utils;
		
		// init resolver, winsock, push DNS servers //
		bool status = utils.init();
		
		if (!status)
		{
			cout << "Resolver could not be initialized. Exiting." << endl;
			return 0;		
		}
		
		// resolve numbers //
		numbers = utils.getNumbers(string(argv[1]));
		
		if (numbers.size() > 0)
		{
			int length = numbers.size();
			cout << "Total number of discovered tel numbers is " << length << endl;
			for (int i=0; i<length; i++) {
			
				// convert phone type to std::string
				string phonetypestr = utils.phTypeToStr(numbers[i].ph_type);

				cout << "   Number (" << i+1 << "): " << numbers[i].content;
				cout << ", Type: " << phonetypestr;
				
				if (numbers[i].descriptor!="")
					cout << ", Label: " << numbers[i].descriptor << endl;
				else
					cout << endl;				
			}
		}
		
		// resolve descirptions //
		descriptions = utils.getDescriptions(string(argv[1]));
		
		if (descriptions.size() > 0)
		{
			int length = descriptions.size();
			cout << "Total number of discovered tel descriptions is " << length << endl;
			for (int i=0; i<length; i++)			
				cout << "   Description (" << i+1 << "): " << descriptions[i].content << endl;
		}
		
		// cleanup resolver, winsock //
		utils.destroy();
		
	}
	
	return 0;
}