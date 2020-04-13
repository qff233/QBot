#include "reload.h"

#include <ctime>
#include "../app.h"
#include "../admin_list.h"
#include "../util/common.h"

namespace qff233 {
namespace command {

Reload::Reload()
	:Command("Reload"){ }

void Reload::handle(MsgEvent& e)
{
	if (e.message != ".q reload") {
		return;
	}
	if (e.fromAccount != *AdminListMgr::GetInstance()) {
		e.sendMsg("哦豁 你不是管理员！");
		return;
	}
		
	clock_t start = clock();
	qff233::reload();
	clock_t stop = clock();
	e.sendMsg(std::string("[QBot] Reloaded\nUsed ") + std::to_string(1000 * (stop - start) / CLOCKS_PER_SEC) + " ms");
	e.message_block();
	return;
}

}
}