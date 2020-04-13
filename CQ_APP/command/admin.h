#pragma once

#include "../command.h"

namespace qff233 {
namespace command
{


class AddAdmin : public Command {
public:
	typedef std::shared_ptr<AddAdmin> ptr;
	AddAdmin();
	void handle(MsgEvent& e) override;
};
class DelAdmin : public Command {
public:
	typedef std::shared_ptr<DelAdmin> ptr;
	DelAdmin();
	void handle(MsgEvent& e) override;
};

}
}