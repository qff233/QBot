#include "admin.h"

#include "../admin_list.h"

namespace qff233 {
namespace command 
{

AddAdmin::AddAdmin()
	:Command("AddAdmin") { }

void
AddAdmin::handle(MsgEvent& e)
{
	if (e.message.size() < 15) {
		e.sendMsg("无效的账号");
		return;
	}
	std::string str = e.message.substr(13, e.message.size() - 13);
	try {
		if (!AdminListMgr::GetInstance()->addAdmin(std::stoll(str))) {
			e.sendMsg("文件操作有问题!!!");
		}
	} catch (const std::exception & ex) {
		e.sendMsg(std::string("无效的账号  ") + ex.what());
	}
}

DelAdmin::DelAdmin()
	:Command("DelAdmin") { }

void
DelAdmin::handle(MsgEvent& e)
{
	if (e.message.size() < 15) {
		e.sendMsg("无效的账号");
		return;
	}
	std::string str = e.message.substr(13, e.message.size() - 13);
	try {
		if (!AdminListMgr::GetInstance()->delAdmin(std::stoll(str))) {
			e.sendMsg("文件操作有问题!!!");
		}
	}
	catch (const std::exception & ex) {
		e.sendMsg(std::string("无效的账号  ") + ex.what());
	}
}

}
}