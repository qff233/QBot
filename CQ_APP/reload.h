#pragma once

#include <CQSDK.h>

namespace qff233 {
namespace command {
	class Reload
	{
	public:
		static bool handle(MsgEvent& e);
	};

}
}