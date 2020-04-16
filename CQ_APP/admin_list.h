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
	
	std::vector<int64_t> getList() { return m_list; }

	void reload();

	bool operator==(int64_t rhs) const;
	bool operator!=(int64_t rhs) const;
private:
	std::vector<int64_t> m_list;
};

typedef qff233::Singleton<AdminList> AdminListMgr;

bool operator!=(const int64_t al, const AdminList& v);
bool operator==(const int64_t al, const AdminList& v);

}