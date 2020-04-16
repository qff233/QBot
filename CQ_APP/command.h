#pragma once

#include <CQSDK.h>
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <functional>
#include <string_view>

#include "util/singleton.h"

namespace qff233 
{

class Command 
{
public:
	typedef std::shared_ptr<Command> ptr;

	Command(const std::string& name)
		:m_name(name) { }
	virtual ~Command() { }

	virtual void handle(CQ::MsgEvent& e) = 0;

	std::string getName() const { return m_name; }
private:
	std::string m_name;
};

class FunctionCommand : public Command 
{
public:
	typedef std::shared_ptr<FunctionCommand> ptr;
	typedef std::function<bool(CQ::MsgEvent&)> callback;

	FunctionCommand(callback cb);
	void handle(CQ::MsgEvent& e) override;
private:
	callback m_cb;
};

class CommandDispatch 
{
public:
	typedef std::shared_ptr<CommandDispatch> ptr;

	CommandDispatch();
	bool handle(CQ::MsgEvent& e);

	void addCommand(const std::string& name, Command::ptr cmd);
	void addCommand(const std::string& name, FunctionCommand::callback cb);
	void addGlobCommand(const std::string& name, Command::ptr cmd);
	void addGlobCommand(const std::string& name, FunctionCommand::callback cb);

	void delCommand(const std::string& name);
	void delGlobCommand(const std::string& name);

	void setDefault(Command::ptr v) { m_default = v; }
	Command::ptr getDefault() const { return m_default; }

	Command::ptr getCommand(const std::string& name);
	Command::ptr getGlobCommand(const std::string& name);
	Command::ptr getMatchedCommand(const std::string& name);
private:
	std::unordered_map<std::string, Command::ptr> m_datas;
	std::vector<std::pair<std::string, Command::ptr> >m_globs;
	Command::ptr m_default;
};

class NotFoundCommand : public Command 
{
public:
	typedef std::shared_ptr<NotFoundCommand> ptr;

	NotFoundCommand(const std::string& name);
	void handle(CQ::MsgEvent& e) override;
private:
	bool m_isSend = false;
};

typedef Singleton<CommandDispatch> CommandMgr;

}