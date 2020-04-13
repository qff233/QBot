#pragma once

#include <CQSDK.h>
#include "../command.h"

namespace qff233 
{
namespace command 
{

class Reload : public Command
{
public:
	typedef std::shared_ptr<Reload> ptr;
	Reload();
	void handle(MsgEvent& e) override;
};

}
}