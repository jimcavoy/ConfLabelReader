#ifdef WIN32
#define _CRT_SECURE_NO_WARNINGS
#endif
#include "pch.h"
#include "LabelReader.h"

#include "LabelReaderImpl.h"
#include <string.h>

namespace ThetaStream
{
	LabelReader::LabelReader()
		:_pimpl(new LabelReaderImpl)
		, _label(nullptr)
	{

	}

	LabelReader::~LabelReader()
	{
		delete _pimpl;
		delete[] _label;
	}

	void LabelReader::parse(const BYTE* stream, unsigned long len)
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

	bool LabelReader::hasLabelStream() const
	{
		return _pimpl->hasLabel();
	}

	const char* LabelReader::label() const
	{
		return _label;
	}
}
