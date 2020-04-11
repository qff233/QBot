#pragma once

#include <CQSDK.h>

namespace qff233 {
namespace command {
	class Ping {
	public:
		static bool handle(MsgEvent& e);
	};
}
}