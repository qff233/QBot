#include "app.h"
#include "file_store.h"
#include "admin_list.h"
#include "handle_event/repeat.h"
#include "command/ping.h"
#include "command/reload.h"
#include "command/admin.h"
#include "command/block.h"
#include "command/test.h"

namespace qff233 {

void init()
{
#define ADDFILESTORE(type, name, file_name) \
	qff233::FileStore::AddFile<qff233::type>(name, file_name)

	ADDFILESTORE(FileStoreString , "NotFoundCommand", "help.txt");
	ADDFILESTORE(FileStoreInt64, "AdminList", "adminlist.txt");
	ADDFILESTORE(FileStoreInt64, "BlockList", "blocklist.txt");
	ADDFILESTORE(FileStoreString, "AdminHelp", "adminhelp.txt");

	qff233::FileStore::AddCallBack<FileStoreInt64>("AdminList", [](int32_t v) {
		if (v == 2) {
			qff233::GetLogger()->Warning("AdminList 文件IO有问题！！");
		}
		if (v == 0) {
			qff233::GetLogger()->Info("AdminList操作成功");
		}
		});

	qff233::FileStore::AddCallBack<FileStoreString>("AdminHelp", [](int32_t v) {
		if (v == 2) {
			qff233::GetLogger()->Error("AdminHelp 文件IO出问题");
		}
		if (v == 0) {
			qff233::GetLogger()->Info("AdminHelp操作成功！！");
		}
		});
#undef ADDFILESTORE

	qff233::Config::LoadConfigFromFile("config.ini");
	qff233::FileStore::Load();

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

	ADDCMD(".q block list", BlockList);
	ADDGCMD(".q block add", AddBlock);
	ADDGCMD(".q block del", DelBlock);

	ADDCMD(".q ping", Ping);
	ADDCMD(".q test", Test);

#undef ADDCMD
#undef ADDGCMD
	GetLogger()->Info("init done");
}

void reload()
{
	qff233::Config::LoadConfigFromFile("config.ini");
	qff233::FileStore::Load();
	//qbot::AdminListMgr::GetInstance()->reload();
	//auto help_cmd = qff233::CommandMgr::GetInstance()->getDefault();
	//std::dynamic_pointer_cast<qff233::NotFoundCommand>(help_cmd)->reload();
	//auto admin_help_cmd = qff233::CommandMgr::GetInstance()->getCommand(".q admin help");
	//std::dynamic_pointer_cast<qbot::command::AdminHelp>(admin_help_cmd)->reload();
}

void handle(MsgEvent& e)
{
	qff233::handle_event::Repeat::run(&e);
}

}