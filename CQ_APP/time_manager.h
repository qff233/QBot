#pragma once

#include <CQSDK.h>
#include <unordered_map>
#include "common.h"

namespace qff233 {

	class TimeManager {
	public:
		void upTime(MsgEvent& e);
		bool isActive(MsgEvent& e);
	private:
		std::unordered_map<long long, time_t> m_datas;
	};

	typedef Singleton<TimeManager> TimeMgr;

}