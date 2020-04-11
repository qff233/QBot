#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <boost/lexical_cast.hpp>

#include "common.h"

namespace qff233 {

	class ConfigVarBase {
	public:
		typedef std::shared_ptr<ConfigVarBase> ptr;

		ConfigVarBase(const std::string& name, const std::string description);
		~ConfigVarBase() { }

		const std::string& getName() const { return m_name; }
		const std::string& getDescription() const { return m_description; }

		virtual void fromString(const std::string& str) = 0;
		virtual std::string toString() = 0;
	protected:
		std::string m_name;
		std::string m_description;
	};

	template<class T>
	class ConfigVar : public ConfigVarBase {
	public:
		typedef std::shared_ptr<ConfigVar> ptr;
		ConfigVar(const std::string& name, const T& value, const std::string& description)
			:ConfigVarBase(name, description)
			,m_val(value) { }

		void fromString(const std::string& str) override {
			try
			{
				setValue(boost::lexical_cast<T>(str));
			}
			catch (std::exception& e)
			{
				GetLogger()->Warning(std::string("ConfigVar::fromString() is error   ") + e.what());
			}
		}
		std::string toString() override {
			std::string str;
			try
			{
				str = boost::lexical_cast<std::string>(m_val);
			}
			catch (...)
			{
				GetLogger()->Warning("ConfigVar::toString() is error");
			}
			return str;
		}

		void setValue(const T& v) {
			if (v == m_val) {
				return;
			}
			m_val = v;
		}
		const T& getValue() {
			return m_val;
		}
	private:
		T m_val;
	};

	class Config {
	public:
		typedef std::unordered_map<std::string, ConfigVarBase::ptr> DataType;
		
		template<class T>
		static typename ConfigVar<T>::ptr Lookup(const std::string& name,
										const T& value, 
										const std::string& description) {
			auto it = GetDatas().find(name);
			if (it != GetDatas().end()) {
				auto tmp = std::dynamic_pointer_cast<ConfigVar<T> >(it->second);
				if (tmp) {
					return tmp;
				} else {
					//MessageBox(NULL, TEXT("config error type"), TEXT("qff233"), MB_OK);
					GetLogger()->Error("[Config::Lookup] error type");
					return nullptr;
				}
			}
			std::string temp = name;
			std::transform(temp.begin(), temp.end(), temp.begin(), ::tolower);
			if (temp.find_first_not_of("abcdefghijklmnopqrstuvwxyz._123456789") != std::string::npos){
				//MessageBox(NULL, TEXT("config inval"), TEXT("qff233"), MB_OK);
				GetLogger()->Error("[Config::Lookup] inval config name");
				return nullptr;
			}
			
			ConfigVar<T>::ptr tmp(new ConfigVar<T>(temp, value, description));
			GetDatas()[temp] = tmp;
			//GetLogger()->Info(tmp->toString());
			//MessageBox(NULL, TEXT("config end"), TEXT("qff233"), MB_OK);
			return tmp;
		}

		template<class T>
		static typename ConfigVar<T>::ptr Lookup(const std::string& name) {
			auto it = GetDatas().find(name);
			if (it == GetDatas().end()) {
				return nullptr;
			}
			return std::dynamic_pointer_cast<ConfigVar<T>>(it->second);
		}

		static ConfigVarBase::ptr LookupBase(const std::string& name) {
			auto it = GetDatas().find(name);
			return it == GetDatas().end() ? nullptr : it->second;
		}
		static void LoadConfigFromFile(const std::string& file_name);
	private:
		static DataType& GetDatas() {
			static DataType v;
			return v;
		}
	};

}