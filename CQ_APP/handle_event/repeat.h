#pragma once

#include <CQSDK.h>
#include <unordered_map>
#include <set>

namespace qff233 {
namespace handle_event {
	class Repeat {
	private:
		struct status;
	public:
		typedef std::unordered_map<long long, status> MapType;
		static void run(MsgEvent* e);
	private:
		struct status {
			status() = default;
			status(const std::string& message, const std::set<long long>& accounts, bool active)
				:message(message)
				,accounts(accounts)
				, hasRepeat(active){ }
			status(const status&) = default;
			status& operator=(const status&) = default;
			status& operator=(status&&) = default;

			std::string message;
			std::set<long long> accounts;
			bool hasRepeat = false;
		};


		static MapType& GetDatas() {
			static  MapType m_datas;
			return m_datas;
		}
	};

}
}