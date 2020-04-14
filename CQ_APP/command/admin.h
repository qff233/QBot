#pragma once

#include "../command.h"

namespace qff233 {
namespace command
{


class AddAdmin : public Command 
{
public:
	typedef std::shared_ptr<AddAdmin> ptr;
	AddAdmin();
	void handle(MsgEvent& e) override;
};

class DelAdmin : public Command 
{
public:
	typedef std::shared_ptr<DelAdmin> ptr;
	DelAdmin();
	void handle(MsgEvent& e) override;
};

class AdminHelp : public Command 
{
public:
	typedef std::shared_ptr<AdminHelp> ptr;
	AdminHelp();
	void reload();
	void handle(MsgEvent& e) override;
private:
	std::string m_help;
	bool m_isError = false;
};

class AdminList : public Command
{
public:
	typedef std::shared_ptr<AdminList> ptr;
	AdminList();
	void handle(MsgEvent& e) override;
private:
	std::string m_help;
	bool m_isError = false;
};

}
}