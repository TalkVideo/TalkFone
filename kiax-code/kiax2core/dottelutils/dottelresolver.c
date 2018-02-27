#include "dottelutils.h"

int main(int argc, char *argv[])
{
	if (argc==2)
	{
		struct TelRecord* numbers = NULL; // array of numbers //
		struct TelRecord* descriptions = NULL; // array of descriptions //
		int length = 0;
		
		// init resolver, winsock, push DNS servers //
		bool status = initDotTel();
		
		if (!status)
		{
			printf("Resolver could not be initialized. Exiting.\n");
			return 0;
		}
		
		int i = 0;		
		
		// resolve numbers //
		numbers = getTelNumbers((char*)argv[1], &length);
		
		if (numbers!=NULL)
		{
			printf("Total number of discovered tel numbers is %d.\n", length);

			for (i=0; i<length; i++) {
				
				char* phonetypestr = (char*)malloc(8*sizeof(char*));
				
				// convert phone type to null-terminated string //
				phoneTypeToStr(numbers[i].ph_type, &phonetypestr);
				
				printf("   Number (%d): %s", i+1, numbers[i].content);
				printf(", Type: %s", phonetypestr);
				
				if (numbers[i].descriptor!=NULL)
					printf(", Label: %s\n", numbers[i].descriptor);
				else
					printf("\n");
				
				free(phonetypestr);
				phonetypestr = NULL;
						
			}
		}
		
		// cleanup results //
		deleteTelArray(numbers, length);
		
		// resolve descriptions //
		descriptions = getTelDescription((char*)argv[1], &length);
		if (descriptions!=NULL)
		{
			printf("Total number of discovered tel descriptions is %d.\n", length);
			for (i=0; i<length; i++)			
				printf("   Description (%d): %s\n", i+1, descriptions[i].content);
		}
		
		// cleanup results //
		deleteTelArray(descriptions, length);
	}
	
	// cleanup resover, winsock //
	destroyDotTel();
	
	return 0;
}

