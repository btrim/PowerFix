// PowerFix.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#pragma comment(lib, "powrprof.lib")


int _tmain(int argc, _TCHAR* argv[])
{

	GUID* activeScheme = NULL;
	GUID* swapScheme = NULL;

	if (ERROR_SUCCESS != PowerGetActiveScheme(NULL, &activeScheme) )
	{
		return 1;
	}
	
	DWORD bufferSize = 0;
	ULONG index = 0;
	UCHAR * buffer = NULL;

	if ( ERROR_MORE_DATA == PowerEnumerate(NULL,NULL, NULL, ACCESS_SCHEME,index,NULL,&bufferSize) )
	{
		buffer = new UCHAR[bufferSize];
		while ( swapScheme == NULL && ERROR_SUCCESS == PowerEnumerate(NULL,NULL, NULL, ACCESS_SCHEME,index,buffer,&bufferSize) )
		{
			GUID * schemeID = (GUID*)buffer;
			if ( !IsEqualGUID(*activeScheme,*schemeID) )
			{
				swapScheme = schemeID;
			}
			index++;
		}
	}

	if ( swapScheme != NULL )
	{
		if ( ERROR_SUCCESS != PowerSetActiveScheme(NULL,swapScheme) )
		{
			fprintf(stderr, "Unable to swap active power scheme.");
		}
		if ( ERROR_SUCCESS != PowerSetActiveScheme(NULL,activeScheme) )
		{
			fprintf(stderr, "Unable to reset active power scheme.");
		}
	}

	delete [] buffer;
	LocalFree(activeScheme);

	return 0;
}

