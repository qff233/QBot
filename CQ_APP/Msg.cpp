#include <CQSDK.h>
#include <string>

#include "app.h"
#include "command.h"

EVE_GroupMsg_EX(GroupMsg_EX)
{
	if(qff233::CommandMgr::GetInstance()->handle(e)) return;  //处理指令
	qff233::handle(e);							   //处理or记录 消息
}
EVE_Request_AddGroup_EX(Request_AddGroupMsg_EX)
{
	if (e.subType == 2) {
		e.pass();				//自动同意进群邀请
	}
}

EVE_PrivateMsg_EX(PrivateMsg_EX)
{
}
EVE_DiscussMsg_EX(DiscussMsg_EX) //讨论组消息
{
}