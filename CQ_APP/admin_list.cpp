#include "admin_list.h"

#include <fstream>

#include "util/common.h"

namespace qff233
{

AdminList::AdminList()
{
	std::ifstream ss;
	std::string str;
	ss.open("adminlist.txt");
	if (!ss.is_open()) {
		GetLogger()->Error("open adminlist.txt error");
		return;
	}
	while (std::getline(ss, str))
	{
		try {
			m_list.push_back(std::stoll(str));
		} catch (const std::exception& e) {
			GetLogger()->Error(std::string("Adminlist::Adminlist error  ") + e.what());
		}
	}
	ss.close();
}

bool
AdminList::addAdmin(int64_t v)
{
	std::ofstream ss;
	ss.open("adminlist.txt", std::ios::app);
	if (!ss.is_open()) {
		GetLogger()->Error("open adminlist.txt error");
		return false;
	}
	ss << v << std::endl;
	ss.close();

	m_list.push_back(v);
	return true;
}

bool
AdminList::delAdmin(int64_t v)
{
	std::string str;
	std::fstream ss;
	ss.open("adminlist.txt");
	if (!ss.is_open()) {
		GetLogger()->Error("open adminlist.txt error");
		return false;
	}
	ss >> str;
	std::string vv = std::to_string(v);
	size_t begin = str.find(vv);
	str.erase(begin, vv.size());
	ss << str;
	ss.close();

	for (auto it = m_list.begin(); it != m_list.end(); ++it) {
		if (*it == v) {
			m_list.erase(it);
		}
	}
	return true;
}

void
AdminList::reload()
{
	std::ifstream ss;
	std::string str;
	ss.open("adminlist.txt");
	if (!ss.is_open()) {
		GetLogger()->Error("open adminlist.txt error");
		return;
	}
	while (std::getline(ss, str))
	{
		try {
			m_list.push_back(std::stoll(str));
		}
		catch (const std::exception & e) {
			GetLogger()->Error(std::string("Adminlist::Adminlist error  ") + e.what());
		}
	}
	ss.close();
}

bool AdminList::operator==(int64_t rhs) const
{
	for (const auto& i : m_list) {
		if (rhs == i) {
			return true;
		}
	}
	return false;
}

bool 
AdminList::operator!=(int64_t rhs) const
{
	return !(*this == rhs);
}

bool
operator!=(const int64_t al, const AdminList& v)
{
	return (v != al);
}

bool
operator==(const int64_t al, const AdminList& v)
{
	return (v == al);
}

}