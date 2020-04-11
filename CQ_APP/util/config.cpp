#include "config.h"

#include <fstream>

namespace qff233 {
	ConfigVarBase::ConfigVarBase(const std::string& name, const std::string description) 
		:m_name(name)
		,m_description(description) {
	}

	void Config::LoadConfigFromFile(const std::string& file_name) {
		std::ifstream ss;
		ss.open(file_name);
		std::string key;
		std::string value;
		std::string temp;
		std::string prefix;
		while (std::getline(ss, temp)) {
			//GetLogger()->Info(temp);
			if (temp.empty() || temp[0] == ';') {
				continue;
			}
			//[xxxx]
			//xx=xx
			if (temp[0] == '[') {
				auto right = temp.find(']');
				if (right == std::string::npos) {
					GetLogger()->Error("config file is inval");
					return;
				}
				prefix = temp.substr(1, right - 1);
				continue;
			}

			for (auto it = temp.begin(); it != temp.end(); ++it) {
				if (*it == ' ') {
					temp.erase(it);
				}
			}

			auto sign = temp.find('=');
			if (sign == std::string::npos) {
				GetLogger()->Error("config file is inval");
				return;
			}

			key = prefix + "." + temp.substr(0, sign);
			auto config_base = LookupBase(key);
			if (!config_base) {
				GetLogger()->Warning("not find config. key= " + key);
				continue;
			}
			value = temp.substr(sign + 1, temp.size() - sign - 1);
			GetLogger()->Info(key + " : " + value);
			config_base->fromString(value);
			key.clear();
			value.clear();
			temp.clear();
		}
		ss.close();
	}
}