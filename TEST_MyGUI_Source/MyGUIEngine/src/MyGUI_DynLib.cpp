/*!
	@file
	@author		Denis Koronchik
	@author		Georgiy Evmenov
	@date		09/2007
	@module
*/
#include "MyGUI_DynLib.h"
#include "MyGUI_Common.h"

#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
#include <Windows.h>
#endif

namespace MyGUI
{
	DynLib::DynLib( const std::string& name )
	{
		mName = name;
		mInstance = NULL;
	}


	DynLib::~DynLib()
	{
	}


	void DynLib::load()
	{
		// Log library load
		MYGUI_LOG(Info, "Loading library " << mName);

		mInstance = (DYNLIB_HANDLE)DYNLIB_LOAD( mName.c_str() );

		MYGUI_ASSERT(null != mInstance, "Could not load dynamic library '" << mName << "'. System Error: " << dynlibError());
	}


	void DynLib::unload()
	{
		// Log library unload
		MYGUI_LOG(Info, "Unloading library " << mName);

		if( DYNLIB_UNLOAD( mInstance ) )
		{
			MYGUI_EXCEPT("Could not unload dynamic library '" << mName << "'. System Error: " << dynlibError());
		}
	}

	void* DynLib::getSymbol( const std::string& strName ) const throw()
	{
		return (void*)DYNLIB_GETSYM( mInstance, strName.c_str() );
	}

	std::string DynLib::dynlibError( void )
	{
	#ifdef WIN32
		LPVOID lpMsgBuf;
		FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			GetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR) &lpMsgBuf,
			0,
			NULL
			);
		std::string ret = (char*)lpMsgBuf;
		// Free the buffer.
		LocalFree( lpMsgBuf );
		return ret;
	#else
		return "no unix error function defined yet";
	#endif
	}
} // namespace MyGUI
