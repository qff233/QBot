#include "time_manager.h"

namespace qff233 {

	static ConfigVar<int>::ptr g_int_anti_often_use_time
		= Config::Lookup("time.anti_often_use", (int)6, "send message time out");

	void TimeManager::upTime(MsgEvent& e) {
		m_datas[e.fromAccount] = time(0);
	}

	bool TimeManager::isActive(MsgEvent& e)
	{
		auto it = m_datas.find(e.fromAccount);
		if (it == m_datas.end()) {
			return true;
		}
		time_t temp = time(0) - it->second;
		if (temp >= g_int_anti_often_use_time->getValue()) {
			return true;
		}
		else {
			return false;
		}
	}

}