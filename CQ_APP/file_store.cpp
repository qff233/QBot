#include "file_store.h"
#include "util/common.h"

#include <fstream>

namespace qff233 
{
	

FileStore::FileStore(const std::string& file_name)
	:m_fileName(file_name)
{ }

void 
FileStore::addCallBack(const CallBackType& cb)
{
	m_cbs.push_back(cb);
}

void 
FileStore::clearCallBack(const CallBackType& cb)
{
	m_cbs.clear();
}

FileStoreString::FileStoreString(const std::string file_name)
	:FileStore(file_name) 
{ }

void 
FileStoreString::addLine(const std::string& value)
{
	size_t idx = m_content.find(value);
	if (idx != std::string::npos) {
		for (auto& i : m_cbs) {
			i(1);
		}
		return;
	}

	std::ofstream ofs;
	ofs.open(m_fileName, std::ios::app);
	if (!ofs.is_open()) {
		for (auto& i : m_cbs) {
			i(2);
		}
		return;
	}
	ofs << gbk2utf8(value.c_str()) << std::endl;
	ofs.close();
	for (auto& i : m_cbs) {
		i(0);
	}
}

void 
FileStoreString::delLine(const std::string& value)
{
	size_t idx = m_content.find(value);
	if (idx == std::string::npos) {
		for (auto& i : m_cbs) {
			i(1);
		}
		return;
	}

	m_content.erase(idx, value.size() + 1);
	std::ofstream ofs;
	ofs.open(m_fileName);
	if (!ofs.is_open()) {
		for (auto& i : m_cbs) {
			i(2);
		}
		return;
	}
	ofs << gbk2utf8(m_content.c_str());
	ofs.close();
	for (auto& i : m_cbs) {
		i(0);
	}
}

void
FileStoreString::load()
{
	m_content.clear();
	//GetLogger()->Debug("FileStoreString::load");
	std::ifstream ifs;
	ifs.open(m_fileName);
	if (!ifs.is_open()) {
		for (auto& i : m_cbs) {
			i(2);
		}
		return;
	}
	std::string str;
	while (std::getline(ifs, str)) {
		m_content += (str + '\n');
	}
	m_content = utf82gbk(m_content.c_str());
	ifs.close();
	for (auto& i : m_cbs) {
		i(0);
	}
}

FileStoreInt64::FileStoreInt64(const std::string file_name)
	:FileStore(file_name) 
{ }

void 
FileStoreInt64::addLine(int64_t value)
{
	for (auto i : m_content) {
		if (i == value) {
			for (auto& i : m_cbs) {
				i(1);
			}
			return;
		}
	}
	m_content.push_back(value);
	
	std::ofstream ofs;
	ofs.open(m_fileName, std::ios::app);
	if (!ofs.is_open()) {
		for (auto& i : m_cbs) {
			i(2);
		}
		return;
	}
	ofs << value << std::endl;;
	ofs.close();
	for (auto& i : m_cbs) {
		i(0);
	}
}

void 
FileStoreInt64::delLine(int64_t value)
{
	bool has_value = false;
	decltype(m_content.begin()) itx;
	for (auto it = m_content.begin(); it != m_content.end(); ++it) {
		if (*it == value) {
			itx = it;
			has_value = true;
		}
	}
	if (!has_value) {
		for (auto& i : m_cbs) {
			i(1);
		}
		return;
	}

	m_content.erase(itx);

	std::ofstream ofs;
	ofs.open(m_fileName);
	if (!ofs.is_open()) {
		for (auto& i : m_cbs) {
			i(2);
		}
		return;
	}
	for (auto i : m_content) {
		ofs << i << std::endl;
	}
	ofs.close();
	for (auto& i : m_cbs) {
		i(0);
	}
}

void
FileStoreInt64::load()
{
	//GetLogger()->Debug("FileStoreInt64::load");
	m_content.clear();
	std::ifstream ifs;
	ifs.open(m_fileName);
	if (!ifs.is_open()) {
		for (auto& i : m_cbs) {
			i(2);
		}
		return;
	}
	std::string str;
	while (std::getline(ifs, str)) {
		m_content.push_back(std::stoll(str));
	}
	ifs.close();
	for (auto& i : m_cbs) {
		i(0);
	}
}

void
FileStoreManager::Load()
{

	//GetLogger()->Debug("FileStoreManager::Load begin");
	try {
		for (auto& i : GetDatas()) {
			//GetLogger()->Debug("FileStoreManager::Load");
			i.second->load();
		}
	}
	catch (const std::exception & e) {
		GetLogger()->Error("Load File error. what: " + std::string(e.what()));
	}
}


}