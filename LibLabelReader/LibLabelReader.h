#pragma once

#ifdef _WIN32
	#ifdef LIBLABELREADER_EXPORTS
		#define LIBLABELREADER_API	__declspec(dllexport)
	#else
		#define LIBLABELREADER_API	__declspec(dllimport)
	#endif
#else
	#define LIBLABELREADER_API
#endif // _WIN32
