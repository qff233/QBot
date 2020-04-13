#pragma once

#include <CQSDK.h>
#include <memory>

#include "../command.h"

namespace qff233 {
namespace command {

class Ping : public Command{
public:
	typedef std::shared_ptr<Ping> ptr;
	Ping();
	void handle(MsgEvent& e) override;
};

}
}