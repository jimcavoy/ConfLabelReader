#include "pch.h"
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

AccessUnit::~AccessUnit()
{

}

void AccessUnit::insert(char* sodb, unsigned int len)
{
	std::copy(sodb, sodb + len, std::back_inserter(_sodb));
}