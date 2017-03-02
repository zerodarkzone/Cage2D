#ifdef _MSC_BUILD
#ifdef _DEBUG
//#include <vld.h>
#endif // _DEBUG
#endif // _MSC_BUILD

#include "AppDelegate.hpp"

int main( int argc, char **argv )
{
	UNREFERENCED_PARAMETER( argc );
	UNREFERENCED_PARAMETER( argv );
	{
		AppDelegate app;
		app.Run();
	}

#ifdef _DEBUG
	system( "PAUSE" );
#endif // _DEBUG

	return EXIT_SUCCESS;
}
