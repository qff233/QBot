#pragma once

#include <vector>

#include "util/singleton.h"

namespace qbot 
{

class AdminList 
{
public:
	AdminList();
	
	bool addAdmin(int64_t v);
	bool delAdmin(int64_t v);
	
	const std::vector<int64_t>& getList() const;

	bool operator==(int64_t rhs) const;
	bool operator!=(int64_t rhs) const;
};

bool operator!=(const int64_t al, const AdminList& v);
bool operator==(const int64_t al, const AdminList& v);

typedef qff233::Singleton<AdminList> AdminListMgr;

}