#pragma once

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <functional>
#include <CQSDK.h>

#include "util/singleton.h"
#include "util/common.h"

namespace qff233 
{

class FileStoreManager;

class FileStoreVar
{
public:
	friend FileStoreManager;
	typedef std::shared_ptr<FileStoreVar> ptr;
	// 1:删除对应未存在;添加对应已存在    2:文件IO有问题     0:成功
	typedef std::function<void(int32_t)> CallBackType;
	
	FileStoreVar(const std::string& file_name);
	virtual ~FileStoreVar() { }

	virtual void load() = 0;

	void addCallBack(const CallBackType& cb);
	void clearCallBack(const CallBackType& cb);
protected:
	std::string m_fileName;
	std::vector<CallBackType> m_cbs;
};

class FileStoreString : public FileStoreVar
{
public:
	typedef std::shared_ptr<FileStoreString> ptr;
	FileStoreString(const std::string file_name);

	const std::string& getContent() { return m_content; }

	void addLine(const std::string& value);
	void delLine(const std::string& value);
	void load() override;
private:
	std::string m_content;
};

class FileStoreInt64 : public FileStoreVar
{
public:
	typedef std::shared_ptr<FileStoreInt64> ptr;
	FileStoreInt64(const std::string file_name);

	const std::vector<int64_t>& getContent() const { return m_content; }

	void addLine(int64_t value);
	void delLine(int64_t value);
	void load() override;
private:
	std::vector<int64_t> m_content;
};

class FileStore
{
public:
	typedef std::unordered_map<std::string, FileStoreVar::ptr> DataType;
	
	static void Load();

	static void AddFile(const std::string& name, FileStoreVar::ptr fs) 
	{
		GetDatas()[name] = fs;
		//GetLogger()->Debug("AddFile");
	}

	template<class T>
	static void AddFile(const std::string& name, const std::string& file_name) 
	{
		auto it = GetDatas().find(name);
		if (it != GetDatas().end()) {
			return;
		}
		//GetLogger()->Debug("AddFile");
		T::ptr tmp(new T(file_name));
		GetDatas()[name] = tmp;
	}

	template<class T>
	static typename T::ptr GetFile(const std::string& name)
	{
		auto it = GetDatas().find(name);
		return it == GetDatas().end() ? nullptr : std::dynamic_pointer_cast<T>(it->second);
	}

	static std::string print() {
		std::string a;
		for (auto it = GetDatas().begin(); it != GetDatas().end(); ++it) {
			a += (it->first) + '\n';
		}
		return a;
	}

	template<class T>
	static void AddCallBack(const std::string& name, const FileStoreVar::CallBackType& cb)
	{
		auto it = GetFile<T>(name);
		it->addCallBack(cb);
	}
private:
	static DataType& GetDatas() {
		static DataType v;
		return v;
	}
};

}