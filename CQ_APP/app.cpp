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
	cmdMgr->addCommand(#name, command::clazz::ptr(new command::clazz))

#define ADDGCMD(name, clazz) \
	cmdMgr->addGlobCommand(#name, command::clazz::ptr(new command::clazz))

	ADDCMD(".q reload", Reload);
	ADDGCMD(".q add admin", AddAdmin);
	ADDGCMD(".q del admin", DelAdmin);
	ADDCMD(".q ping", Ping);

#undef ADDCMD
#undef ADDGCMD
}

void reload()
{
	qff233::Config::LoadConfigFromFile("config.ini");
	auto help_cmd = qff233::CommandMgr::GetInstance()->getDefault();
	std::dynamic_pointer_cast<qff233::NotFoundCommand>(help_cmd)->reload();
	AdminListMgr::GetInstance()->reload();
}

void handle(MsgEvent& e)
{
	handle_event::Repeat::run(&e);
}

}