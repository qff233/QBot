#pragma once

#include "../command.h"

namespace qbot {
namespace command
{


class AddAdmin : public qff233::Command 
{
public:
	typedef std::shared_ptr<AddAdmin> ptr;
	AddAdmin();
	void handle(MsgEvent& e) override;
};

class DelAdmin : public qff233::Command 
{
public:
	typedef std::shared_ptr<DelAdmin> ptr;
	DelAdmin();
	void handle(MsgEvent& e) override;
};

class AdminHelp : public qff233::Command 
{
public:
	typedef std::shared_ptr<AdminHelp> ptr;
	AdminHelp();
	void handle(MsgEvent& e) override;
};

class AdminList : public qff233::Command
{
public:
	typedef std::shared_ptr<AdminList> ptr;
	AdminList();
	void handle(MsgEvent& e) override;
};

}
}