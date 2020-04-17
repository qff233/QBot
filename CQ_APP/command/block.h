#pragma once

#include "../command.h"

namespace qbot {
namespace command 
{

class AddBlock : public qff233::Command
{
public:
	typedef std::shared_ptr<AddBlock> ptr;
	AddBlock();
	void handle(MsgEvent& e) override;
};

class DelBlock : public qff233::Command
{
public:
	typedef std::shared_ptr<DelBlock> ptr;
	DelBlock();
	void handle(MsgEvent& e) override;
};

class BlockList : public qff233::Command
{
public:
	typedef std::shared_ptr<BlockList> ptr;
	BlockList();
	void handle(MsgEvent& e) override;
};

}
}