#include "block.h"

#include <sstream>

#include "../admin_list.h"
#include "../block_list.h"

namespace qbot {
namespace command 
{

AddBlock::AddBlock()
    :Command("AddBlock") 
{ }

void 
AddBlock::handle(MsgEvent& e)
{
	if (e.message.size() < 15) {
		e.sendMsg("无效的账号");
		e.message_block();
		return;
	}
	if (e.fromAccount != *AdminListMgr::GetInstance()) {
		e.sendMsg("哦豁 你不是管理员！");
		e.message_block();
		return;
	}
	std::string str = e.message.substr(13, e.message.size() - 13);
	if (str[0] == '[') {
		str = str.substr(10, str.size() - 11);
	}
	try {
		int64_t account = std::stoll(str);
		if (account == *BlockListMgr::GetInstance()) {
			e.sendMsg("已经有该账号了，不要重复添加！");
			e.message_block();
			return;
		}
		if (!BlockListMgr::GetInstance()->add(account)) {
			e.sendMsg("文件操作有问题!!!");
			e.message_block();
		}
		e.sendMsg("添加成功~");
		e.message_block();
	}
	catch (const std::exception & ex) {
		e.sendMsg(std::string("无效的账号  ") + ex.what());
		e.message_block();
	}
}

DelBlock::DelBlock()
    :Command("DelBlock") 
{ }

void 
DelBlock::handle(MsgEvent& e)
{
	if (e.message.size() < 15) {
		e.sendMsg("无效的账号");
		e.message_block();
		return;
	}
	if (e.fromAccount != *AdminListMgr::GetInstance()) {
		e.sendMsg("哦豁 你不是管理员！");
		e.message_block();
		return;
	}
	std::string str = e.message.substr(13, e.message.size() - 13);
	if (str[0] == '[') {
		str = str.substr(10, str.size() - 11);
	}
	try {
		if (std::stoll(str) != *BlockListMgr::GetInstance()) {
			e.sendMsg("这个人在黑名单中");
			e.message_block();
			return;
		}
		if (!BlockListMgr::GetInstance()->del(std::stoll(str))) {
			e.sendMsg("文件操作有问题!!!");
			e.message_block();
		}
		e.sendMsg("删除成功~");
		e.message_block();
	}
	catch (const std::exception & ex) {
		e.sendMsg(std::string("无效的账号  ") + ex.what());
		e.message_block();
	}
}

BlockList::BlockList()
    :Command("BlockList") 
{ }

void
BlockList::handle(MsgEvent& e)
{
	if (e.fromAccount != *AdminListMgr::GetInstance()) {
		e.sendMsg("不是管理员。查什么查？");
		e.message_block();
		return;
	}
	const auto& vecs = BlockListMgr::GetInstance()->get();
	std::stringstream ss;
	for (const auto& i : vecs) {
		ss << i << std::endl;
	}
	e.sendMsg(ss.str());
	e.message_block();
}

}
}