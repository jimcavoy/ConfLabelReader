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

	void LabelReader::parse(const BYTE* stream, size_t len)
	{
		delete[] _label;
		_label = nullptr;
		_pimpl->parse(stream, len);

		std::string temp = _pimpl->label();
		if (!temp.empty())
		{
			_label = new char[temp.length() + 1];
			strcpy_s(_label, temp.length() + 1, temp.c_str());
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
