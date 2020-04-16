#include "admin_list.h"

#include <fstream>

#include "util/common.h"

namespace qbot
{

AdminList::AdminList()
{
	std::ifstream ss;
	std::string str;
	ss.open("adminlist.txt");
	if (!ss.is_open()) {
		qff233::GetLogger()->Error("open adminlist.txt error");
		return;
	}
	while (std::getline(ss, str))
	{
		try {
			m_list.push_back(std::stoll(str));
		} catch (const std::exception& e) {
			qff233::GetLogger()->Error(std::string("Adminlist::Adminlist error  ") + e.what());
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
		qff233::GetLogger()->Error("open adminlist.txt error");
		return false;
	}
	ss << v << '\n';
	ss.close();

	m_list.push_back(v);
	return true;
}

bool
AdminList::delAdmin(int64_t v)
{
	std::string str;
	std::ifstream ss;
	ss.open("adminlist.txt", std::ios::in);
	if (!ss.is_open()) {
		qff233::GetLogger()->Error("open adminlist.txt error");
		return false;
	}
	std::string tmp;
	while (std::getline(ss, tmp)) {
		str += (tmp + '\n');
	}
	ss.close();
	//*****
	qff233::GetLogger()->Debug(str);
	std::string vv = std::to_string(v);
	size_t begin = str.find(vv);
	if (begin == std::string::npos) {
		throw std::invalid_argument("文件中找不到此账号");
		return false;
	}
	str.erase(begin, vv.size() + 1);
	//*****
	qff233::GetLogger()->Debug(str + "    处理完");
	std::ofstream ss1;
	ss1.open("adminlist.txt", std::ios::out | std::ios::trunc);
	if (!ss1.is_open()) {
		throw std::invalid_argument("打开adminlist失败！");
		return false;
	}
	ss1 << str;
	ss1.close();

	for (auto it = m_list.begin(); it != m_list.end(); ++it) {
		if (*it == v) {
			m_list.erase(it);
			return true;
		}
	}
	return false;
}

void
AdminList::reload()
{
	std::ifstream ss;
	std::string str;
	ss.open("adminlist.txt");
	if (!ss.is_open()) {
		qff233::GetLogger()->Error("open adminlist.txt error");
		return;
	}
	m_list.clear();
	while (std::getline(ss, str))
	{
		try {
			m_list.push_back(std::stoll(str));
		}
		catch (const std::exception & e) {
			qff233::GetLogger()->Error(std::string("Adminlist::Adminlist error  ") + e.what());
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