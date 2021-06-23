#ifdef WIN32
#define _CRT_SECURE_NO_WARNINGS
#endif
#include "pch.h"
#include "LabelDemux.h"

#include "LabelDemuxImpl.h"
#include <string.h>

namespace ThetaStream
{
	LabelDemux::LabelDemux()
		:_pimpl(new LabelDemuxImpl)
		, _label(nullptr)
	{

	}

	LabelDemux::~LabelDemux()
	{
		delete _pimpl;
		delete[] _label;
	}

	void LabelDemux::parse(const BYTE* stream, unsigned long len)
	{
		delete[] _label;
		_label = nullptr;
		_pimpl->parse(stream, len);

		std::string temp = _pimpl->label();
		if (!temp.empty())
		{
			_label = new char[temp.length() + 1];
			strcpy(_label, temp.c_str());
		}
	}

	bool LabelDemux::hasLabelStream() const
	{
		return _pimpl->hasLabel();
	}

	const char* LabelDemux::label() const
	{
		return _label;
	}
}
