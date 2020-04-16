#include "ping.h"

namespace qbot {
namespace command {

Ping::Ping()
	:Command("Ping") { }

void Ping::handle(MsgEvent& e) {
	e.sendMsg("[QBot] ≈¿~");
	e.message_block();
}


}
}