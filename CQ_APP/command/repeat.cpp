#include "repeat.h"
#include "../util/common.h"

namespace qff233 {
namespace command {
	ConfigVar<double>::ptr g_int_probability
		= Config::Lookup("repeat.probability", 90.0, "repeat trigger probability");
	ConfigVar<double>::ptr g_int_random_probability
		= Config::Lookup("repeat.random_probability", 0.5, "random repeat trigger probability");

	bool Repeat::handle(GroupMsgEvent& e)
	{	//1~10000
		if ((1.0*(rand() % 10000) + 1) <= g_int_random_probability->getValue()*100) {
			e.sendMsg(e.message);
			e.message_block();
			return true;
		}

		auto it = GetDatas().find(e.fromGroup);
		if (it == GetDatas().end()) {
			GetDatas()[e.fromGroup] = std::move(status(e.message, { e.fromAccount }, false));
			return false;
		}

		auto& temp = it->second;

		if (temp.message != e.message) {
			temp.hasRepeat = false;
			temp.message = e.message;
			temp.accounts.clear();
			temp.accounts.insert(e.fromAccount);
			return false;
		}

		if (temp.hasRepeat) {
			return false;
		}

		if (!temp.accounts.insert(e.fromAccount).second) {
			return false;
		}

		if (!(temp.accounts.size() >= 3)) {
			return false;
		}

		if (!((1.0*(rand() % 10000) + 1) <= g_int_probability->getValue()*100))
		{
			return false;
		}

		e.sendMsg(temp.message);
		temp.hasRepeat = true;
		e.message_block();
		return true;
	}
}
}