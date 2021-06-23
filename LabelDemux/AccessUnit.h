#pragma once

#include <vector>

/////////////////////////////////////////////////////////////////////////////
// AccessUnit
class AccessUnit
{
public:
	typedef std::vector<char> sodb_type; // string of data bits collection type
	typedef sodb_type::iterator iterator;
	typedef sodb_type::const_iterator const_iterator;
public:
	AccessUnit();
	AccessUnit(char* sodb, unsigned int len);
	AccessUnit(const AccessUnit& src);
	AccessUnit& operator=(const AccessUnit& rhs);
	~AccessUnit();

	void insert(char* sodb, unsigned int len);

	void clear() { _sodb.clear(); }

	unsigned int length() const { return (unsigned int) _sodb.size(); }

	iterator begin() { return _sodb.begin(); }
	iterator end() { return _sodb.end(); }

	const_iterator begin() const { return _sodb.begin(); }
	const_iterator end() const { return _sodb.end(); }

	char* data() { return _sodb.data(); }

private:
	void swap(AccessUnit& src);

private:
	sodb_type _sodb;  //string of data bits
};