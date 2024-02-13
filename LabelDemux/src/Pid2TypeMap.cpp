#ifdef WIN32
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <LabelDemux/pch.h>
#include "Pid2TypeMap.h"

#include <mp2tp/mp2tp.h>

namespace
{
	char TAG_HDMV[] = { (char)0x48, (char)0x44, (char)0x4D, (char)0x56, (char)0xFF, (char)0x1B, (char)0x44, (char)0x3F, 0 };
	char TAG_HDPR[] = { (char)0x48, (char)0x44, (char)0x50, (char)0x52, (char)0xFF, (char)0x1B, (char)0x67, (char)0x3F, 0 };
}



Pid2TypeMap::Pid2TypeMap()
{
}


Pid2TypeMap::~Pid2TypeMap()
{
}

/// <summary>
/// Updates this based on the Program Map Table.
/// </summary>
/// <param name="pmt">The Program Map Table.</param>
void Pid2TypeMap::update(const lcss::ProgramMapTable& pmt)
{
	pid2type_.clear();
	for (auto pe : pmt)
	{
		switch (pe.stream_type())
		{
		case 0x06:
		case 0x15:
		{
			char value[255]{};
			char format_identifier[5]{};

			for (auto desc : pe)
			{
				// registration_descriptor
				if (desc.tag() == 0x05)
				{
					desc.value((BYTE*)value);
					strncpy(format_identifier, value, 4);
					break;
				}
			}

			if (strcmp(format_identifier, "KLVA") == 0 || strlen(format_identifier) == 0)
			{
				pid2type_.insert({ pe.pid(), STREAM_TYPE::KLVA });
			}
			else if (strcmp(format_identifier, "$EXI") == 0)
			{
				pid2type_.insert({ pe.pid(), STREAM_TYPE::$EXI });
			}
		}
		break;
		case 0x1B:
		{
			char value[255]{};
			for (auto desc : pe)
			{
				// registration_descriptor
				if (desc.tag() == 0x05)
				{
					desc.value((BYTE*)value);
					break;
				}
			}

			if (strcmp(value, TAG_HDMV) == 0 || strcmp(value, TAG_HDPR) == 0)
			{
				pid2type_.insert({ pe.pid(), STREAM_TYPE::HDMV });
			}
			else
			{
				pid2type_.insert({ pe.pid(), STREAM_TYPE::H264 });
			}
		}
		break;
		case 0x02:
		{
			pid2type_.insert({ pe.pid(), STREAM_TYPE::VIDEO });
		}
		break;
		case 0x03:
		case 0x04:
		{
			pid2type_.insert({ pe.pid(), STREAM_TYPE::AUDIO });
		}
		break;
		}
	}

}

/// <summary>
/// Call this function to determine the stream type of a program element references by its ID.
/// </summary>
/// <param name="pid">The program element ID.</param>
/// <returns>Returns the stream type of a program element; otherwise, return UNKNOWN</returns>
Pid2TypeMap::STREAM_TYPE Pid2TypeMap::packetType(unsigned short pid)
{
	STREAM_TYPE type = STREAM_TYPE::UNKNOWN;

	map_type::iterator it = pid2type_.find(pid);
	if (it != pid2type_.end())
	{
		type = it->second;
	}

	return type;
}

/// <summary>
/// Query to find a program element with a stream type.
/// </summary>
/// <param name="st">A stream type.</param>
/// <returns>Return program element's ID (PID) that has this stream type; otherwise, return 0 if no program element has this stream type</returns>
UINT16 Pid2TypeMap::hasStreamType(Pid2TypeMap::STREAM_TYPE st)
{
	map_type::iterator it;
	for (it = pid2type_.begin(); it != pid2type_.end(); ++it)
	{
		if (it->second == st)
			return it->first;
	}
	return 0;
}
