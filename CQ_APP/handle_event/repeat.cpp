#include "repeat.h"
#include "../util/common.h"

namespace qff233 {
namespace handle_event {
	ConfigVar<double>::ptr g_int_probability
		= Config::Lookup("repeat.probability", 90.0, "repeat trigger probability");
	ConfigVar<double>::ptr g_int_random_probability
		= Config::Lookup("repeat.random_probability", 0.5, "random repeat trigger probability");

	void Repeat::run(MsgEvent* re)
	{	//1~10000
		auto e = dynamic_cast<GroupMsgEvent*>(re);
		if(!e) {
			CQ::sendPrivateMsg(347633553, "¸´¶Árun×ª»»³ö´í");
			return;
		}
		if ((1.0*(rand() % 10000) + 1) <= g_int_random_probability->getValue()*100) {
			e->sendMsg(e->message);
			e->message_block();
			return;
		}

		auto it = GetDatas().find(e->fromGroup);
		if (it == GetDatas().end()) {
			GetDatas()[e->fromGroup] = std::move(status(e->message, { e->fromAccount }, false));
			return;
		}

		auto& temp = it->second;

		if (temp.message != e->message) {
			temp.hasRepeat = false;
			temp.message = e->message;
			temp.accounts.clear();
			temp.accounts.insert(e->fromAccount);
			return;
		}

		if (temp.hasRepeat) {
			return;
		}

		if (!temp.accounts.insert(e->fromAccount).second) {
			return;
		}

		if (!(temp.accounts.size() >= 3)) {
			return;
		}

		if (!((1.0*(rand() % 10000) + 1) <= g_int_probability->getValue()*100))
		{
			return;
		}

		e->sendMsg(temp.message);
		temp.hasRepeat = true;
		e->message_block();
		return;
	}
}
}