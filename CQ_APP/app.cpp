#include "app.h"
#include "admin_list.h"
#include "handle_event/repeat.h"
#include "command/ping.h"
#include "command/reload.h"
#include "command//admin.h"

namespace qff233 {

void init()
{
	qff233::Config::LoadConfigFromFile("config.ini");
	auto cmdMgr = qff233::CommandMgr::GetInstance();

#define ADDCMD(name, clazz) \
	cmdMgr->addCommand(name, clazz::ptr(new clazz))

#define ADDGCMD(name, clazz) \
	cmdMgr->addGlobCommand(name, clazz::ptr(new clazz))

	ADDCMD(".q reload", command::Reload);
	ADDCMD(".q admin help", command::AdminHelp);
	ADDCMD(".q admin list", command::AdminList);
	ADDGCMD(".q admin add", command::AddAdmin);
	ADDGCMD(".q admin del", command::DelAdmin);
	ADDCMD(".q ping", command::Ping);

#undef ADDCMD
#undef ADDGCMD
}

void reload()
{
	qff233::Config::LoadConfigFromFile("config.ini");
	AdminListMgr::GetInstance()->reload();

	auto help_cmd = qff233::CommandMgr::GetInstance()->getDefault();
	std::dynamic_pointer_cast<qff233::NotFoundCommand>(help_cmd)->reload();

	auto admin_help_cmd = qff233::CommandMgr::GetInstance()->getCommand(".q admin help");
	std::dynamic_pointer_cast<qff233::command::AdminHelp>(admin_help_cmd)->reload();
}

void handle(MsgEvent& e)
{
	handle_event::Repeat::run(&e);
}

}