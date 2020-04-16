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
	cmdMgr->addCommand(name, ::qbot::command::clazz::ptr(new ::qbot::command::clazz))

#define ADDGCMD(name, clazz) \
	cmdMgr->addGlobCommand(name, ::qbot::command::clazz::ptr(new ::qbot::command::clazz))

	ADDCMD(".q reload", Reload);
	ADDCMD(".q admin help", AdminHelp);
	ADDCMD(".q admin list", AdminList);
	ADDGCMD(".q admin add", AddAdmin);
	ADDGCMD(".q admin del", DelAdmin);
	ADDCMD(".q ping", Ping);

#undef ADDCMD
#undef ADDGCMD
}

void reload()
{
	qff233::Config::LoadConfigFromFile("config.ini");
	qbot::AdminListMgr::GetInstance()->reload();

	auto help_cmd = qff233::CommandMgr::GetInstance()->getDefault();
	std::dynamic_pointer_cast<qff233::NotFoundCommand>(help_cmd)->reload();

	auto admin_help_cmd = qff233::CommandMgr::GetInstance()->getCommand(".q admin help");
	std::dynamic_pointer_cast<qbot::command::AdminHelp>(admin_help_cmd)->reload();
}

void handle(MsgEvent& e)
{
	qff233::handle_event::Repeat::run(&e);
}

}