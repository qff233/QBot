#include "reload.h"

#include <ctime>

#include "common.h"

namespace qff233 {
namespace command {
	bool Reload::handle(MsgEvent& e) 
	{
		if (e.message != ".q reload") {
			return false;
		}
		if (e.fromAccount != 347633553) {
			e.sendMsg("哦豁 你不是管理员！");
			return true;
		}
		
		clock_t start = clock();
		Config::LoadConfigFromFile("config.ini");
		clock_t stop = clock();
		e.sendMsg(std::string("[QBot] reload is successful\nspend ") + std::to_string(1000*(stop - start)/CLOCKS_PER_SEC) + " ms");
		e.message_block();
		return true;
	}

}
}