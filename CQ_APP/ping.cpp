#include "ping.h"

namespace qff233 {
namespace command {
	bool Ping::handle(MsgEvent& e) {
		if (e.message == ".q ping") {
			e.sendMsg("[QBot] ≈¿~");
			e.message_block();
			return true;
		}
		return false;
	}
}
}