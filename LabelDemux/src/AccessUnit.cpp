#include <LabelDemux/pch.h>
#include "AccessUnit.h"

#include <iterator>

/////////////////////////////////////////////////////////////////////////////
// AccessUnit
AccessUnit::AccessUnit()
{

}

AccessUnit::AccessUnit(char* sodb, unsigned int len)
{
	std::copy(sodb, sodb + len, std::back_inserter(_sodb));
}

AccessUnit::AccessUnit(const AccessUnit& src)
{
	_sodb.clear();
	std::copy(src.begin(), src.end(), std::back_inserter(_sodb));
}

AccessUnit& AccessUnit::operator=(const AccessUnit& rhs)
{
	AccessUnit temp(rhs);
	swap(temp);
	return *this;
}

AccessUnit::~AccessUnit()
{

}

void AccessUnit::insert(char* sodb, unsigned int len)
{
	std::copy(sodb, sodb + len, std::back_inserter(_sodb));
}

void AccessUnit::swap(AccessUnit& src)
{
	_sodb.swap(src._sodb);
}
