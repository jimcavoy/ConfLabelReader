#pragma once
#include <map>

#include "LibLabelReader.h"

namespace lcss
{
	class ProgramMapTable;
}

class Pid2TypeMap
{
public:
	enum class STREAM_TYPE
	{
		UNKNOWN,
		H264,
		HDMV,
		VIDEO,
		AUDIO,
		KLVA,
		$EXI
	};

public:
	Pid2TypeMap();
	~Pid2TypeMap();

private:
	typedef std::map<unsigned short, Pid2TypeMap::STREAM_TYPE> map_type;

public:
	void update(const lcss::ProgramMapTable& pmt);
	STREAM_TYPE packetType(unsigned short pid);
	UINT16 hasStreamType(STREAM_TYPE st);

private:
	map_type pid2type_;
};
