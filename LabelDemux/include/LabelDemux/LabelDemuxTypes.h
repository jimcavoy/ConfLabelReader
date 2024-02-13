#pragma once

#include <cstdint>

#ifdef UINT64_MAX
typedef uint8_t BYTE;
typedef uint16_t UINT16;
typedef uint32_t UINT32;
typedef uint64_t UINT64;
#else
typedef unsigned char BYTE;
typedef unsigned short UINT16;
typedef unsigned int UINT32;
typedef unsigned long long UINT64;
#endif
