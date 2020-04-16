#pragma once

#include <CQSDK.h>
#include <memory>

#include "../command.h"

namespace qbot {
namespace command {

class Ping : public qff233::Command{
public:
	typedef std::shared_ptr<Ping> ptr;
	Ping();
	void handle(MsgEvent& e) override;
};

}
}