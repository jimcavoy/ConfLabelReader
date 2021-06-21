#pragma once

#ifdef LIBLABELREADER_EXPORTS
#define LIBLABELREADER_API	__declspec(dllexport)
#else
#define LIBLABELREADER_API	__declspec(dllimport)
#endif