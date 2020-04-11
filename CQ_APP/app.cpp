#include "app.h"
#include "command/ping.h"
#include "command/repeat.h"
#include "command/reload.h"

namespace qff233 {

void GroupMsgEventHandle(GroupMsgEvent& e)
{
	if (command::Ping::handle(e)) return;
	if (command::Reload::handle(e)) return;
	if (command::Repeat::handle(e)) return;
}

}