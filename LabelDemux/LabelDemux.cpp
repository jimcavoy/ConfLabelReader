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
		, _length(0)
	{

	}

	LabelDemux::~LabelDemux()
	{
		delete _pimpl;
		delete[] _label;
	}

	void LabelDemux::parse(const BYTE* stream, UINT32 len)
	{
		delete[] _label;
		_label = nullptr;
		_length = 0;
		_pimpl->parse(stream, len);

		const AccessUnit& au = _pimpl->label();
		if (au.length() > 0)
		{
			_label = new BYTE[au.length()];
			_length = au.length();
			std::copy(au.begin(), au.end(), _label);
		}
	}

	bool LabelDemux::hasLabelStream() const
	{
		return _pimpl->hasLabel();
	}

	const BYTE* LabelDemux::label() const
	{
		return _label;
	}
	UINT32 LabelDemux::labelSize() const
	{
		return _length;
	}
}
