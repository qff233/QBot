#pragma once

#include <CQSDK.h>
#include "../command.h"

namespace qbot
{
namespace command 
{

class Reload : public qff233:: Command
{
public:
	typedef std::shared_ptr<Reload> ptr;
	Reload();
	void handle(MsgEvent& e) override;
};

}
}