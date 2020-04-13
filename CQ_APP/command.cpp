#include "command.h"
#include <fstream>

#include "util/time_manager.h"

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
	if (!qff233::TimeMgr::GetInstance()->isActive(e)) {
		e.message_block();
		return 0;
	}
	if (e.message.substr(0, 2) != ".q") {
		return 0;
	}
	qff233::TimeMgr::GetInstance()->upTime(e);
	auto cmd = getMatchedCommand(e.message);
	if(cmd) {
		cmd->handle(e);
		return true; 
	}
	return false;
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
	auto mit = m_datas.find(name);
	if (mit != m_datas.end()) {
		return mit->second;
	}
	for (auto it = m_globs.begin(); it != m_globs.end(); ++it)
	{
		if (name.substr(0, it->first.size()) == it->first) {
			return it->second;
		}
	}
	return m_default;
}

NotFoundCommand::NotFoundCommand(const std::string& name)
	:Command("NotFoundCommand"),
	m_name(name)
{ 
	std::ifstream ss;
	ss.open("help.txt");
	if (!ss.is_open()) {
		m_isError = true;
		return;
	}
	ss >> m_help;
	ss.close();
}

void
NotFoundCommand::handle(CQ::MsgEvent& e)
{
	if (m_isError) {
		if (!m_isSend) {
			e.sendMsg("°ïÖúÎÄµµ³õÊ¼»¯Ê§°Ü");
			m_isSend = true;
		}
		return;
	}
	e.sendMsg(m_help);
}

void 
NotFoundCommand::reload()
{
	std::ifstream ss;
	ss.open("help.txt");
	if (!ss.is_open()) {
		m_isError = true;
		return;
	}
	ss >> m_help;
	ss.close();
}

}