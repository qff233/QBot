#include "admin_list.h"

#include <fstream>

#include "util/common.h"
#include "file_store.h"

namespace qbot
{

bool
AdminList::add(int64_t v)
{
	//std::ofstream ss;
	//ss.open("adminlist.txt", std::ios::app);
	//if (!ss.is_open()) {
	//	qff233::GetLogger()->Error("open adminlist.txt error");
	//	return false;
	//}
	//ss << v << '\n';
	//ss.close();

	//m_list.push_back(v);
	auto it = qff233::FileStore::GetFile<qff233::FileStoreInt64>("AdminList");
	if (!it)
	{
		return false;
	}
	it->addLine(v);
	return true;
}

bool
AdminList::del(int64_t v)
{
	auto it = qff233::FileStore::GetFile<qff233::FileStoreInt64>("AdminList");
	if (!it)
	{
		return false;
	}
	it->delLine(v);
	return true;
	//std::string str;
	//std::ifstream ss;
	//ss.open("adminlist.txt", std::ios::in);
	//if (!ss.is_open()) {
	//	qff233::GetLogger()->Error("open adminlist.txt error");
	//	return false;
	//}
	//std::string tmp;
	//while (std::getline(ss, tmp)) {
	//	str += (tmp + '\n');
	//}
	//ss.close();
	////*****
	////qff233::GetLogger()->Debug(str);
	//std::string vv = std::to_string(v);
	//size_t begin = str.find(vv);
	//if (begin == std::string::npos) {
	//	throw std::invalid_argument("文件中找不到此账号");
	//	return false;
	//}
	//str.erase(begin, vv.size() + 1);
	////*****
	////qff233::GetLogger()->Debug(str + "    处理完");
	//std::ofstream ss1;
	//ss1.open("adminlist.txt", std::ios::out | std::ios::trunc);
	//if (!ss1.is_open()) {
	//	throw std::invalid_argument("打开adminlist失败！");
	//	return false;
	//}
	//ss1 << str;
	//ss1.close();

	//auto m_list = this->getList();
	//for (auto it = m_list.begin(); it != m_list.end(); ++it) {
	//	if (*it == v) {
	//		m_list.erase(it);
	//		return true;
	//	}
	//}
	//return false;
}

const std::vector<int64_t>& 
AdminList::get() const
{
	auto fsi = qff233::FileStore::GetFile<qff233::FileStoreInt64>("AdminList");
	if (fsi) {
		
		return fsi->getContent();
	}
	return {};
}

bool AdminList::operator==(int64_t rhs) const
{
	for (const auto& i : this->get()) {
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