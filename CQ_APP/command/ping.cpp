#include "ping.h"

namespace qff233 {
namespace command {

Ping::Ping()
	:Command("Ping") { }

void Ping::handle(MsgEvent& e) {
	e.sendMsg("[QBot] ≈¿~");
	e.message_block();
}


}
}