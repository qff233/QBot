#include "app.h"
#include "ping.h"
#include "repeat.h"
#include "reload.h"

namespace qff233 {

void GroupMsgEventHandle(GroupMsgEvent& e)
{
	if (command::Ping::handle(e)) return;
	if (command::Reload::handle(e)) return;
	if (command::Repeat::handle(e)) return;
}

}