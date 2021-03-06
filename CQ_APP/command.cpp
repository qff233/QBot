#include "command.h"
#include <fstream>

#include "util/time_manager.h"
#include "util/common.h"
#include "file_store.h"
#include "block_list.h"


namespace qff233 
{

FunctionCommand::FunctionCommand(callback cb)
	:Command("FunctionCommand"),
	m_cb(cb)
{ }

void
FunctionCommand::handle(CQ::MsgEvent& e)
{
	 m_cb(e);
}

CommandDispatch::CommandDispatch()

{
	m_default.reset(new NotFoundCommand("command help"));
}

bool
CommandDispatch::handle(CQ::MsgEvent& e)
{
	if (e.fromAccount == *qbot::BlockListMgr::GetInstance()) {
		return true;
	}
	if (!qff233::TimeMgr::GetInstance()->isActive(e)) {
		e.message_block();
		return true;
	}
	if (e.message.substr(0, 2) != ".q") {
		return false;
	}
	qff233::TimeMgr::GetInstance()->upTime(e);
	auto cmd = this->getMatchedCommand(e.message);
	cmd->handle(e);
	return true; 
}

void
CommandDispatch::addCommand(const std::string& name, Command::ptr cmd)
{
	m_datas[name] = cmd;
}

void
CommandDispatch::addCommand(const std::string& name, FunctionCommand::callback cb)
{
	m_datas[name] = std::make_shared<FunctionCommand>(cb);
}

void
CommandDispatch::addGlobCommand(const std::string& name, Command::ptr cmd)
{
	for (auto it = m_globs.begin(); it != m_globs.end(); ++it)
	{
		if (it->first == name) {
			m_globs.erase(it);
			break;
		}
	}
	m_globs.push_back(std::make_pair(name, cmd));
}

void
CommandDispatch::addGlobCommand(const std::string& name, FunctionCommand::callback cb)
{
	this->addGlobCommand(name, std::make_shared<FunctionCommand>(cb));
}

void
CommandDispatch::delCommand(const std::string& name)
{
	m_datas.erase(name);
}

void
CommandDispatch::delGlobCommand(const std::string& name)
{
	for (auto it = m_globs.begin(); it != m_globs.end(); ++it) {
		if (it->first == name) {
			m_globs.erase(it);
			break;
		}
	}
}

Command::ptr
CommandDispatch::getCommand(const std::string& name)
{
	auto it = m_datas.find(name);
	return it == m_datas.end() ? nullptr : it->second;
}

Command::ptr 
CommandDispatch::getGlobCommand(const std::string& name)
{
	for (auto it = m_globs.begin(); it != m_globs.end(); ++it) 
	{
		if (it->first == name) {
			return it->second;
		}
	}
	return nullptr;
}

Command::ptr
CommandDispatch::getMatchedCommand(const std::string& name)
{									
	auto mit = m_datas.find(name);				//.q xxx
	if (mit != m_datas.end()) {
		return mit->second;
	}
	for (auto it = m_globs.begin(); it != m_globs.end(); ++it)
	{
		if(std::string_view(name.c_str(), it->first.size()) == it->first) {
		//if (name.substr(0, it->first.size()) == it->first) {
			return it->second;
		}
	}
	return m_default;
}

NotFoundCommand::NotFoundCommand(const std::string& name)
	:Command("NotFoundCommand")
{
	//FileStoreMgr::GetInstance()->AddFile<qff233::FileStoreString>("NotFoundCommand", "help.txt");
}

void
NotFoundCommand::handle(CQ::MsgEvent& e)
{
	try {
		auto it = FileStore::GetFile<qff233::FileStoreString>("NotFoundCommand");
		const std::string& msg = it->getContent();
		e.sendMsg(msg);
	}
	catch(const std::exception& e){
		qff233::GetLogger()->Debug(e.what());
	}
}

}