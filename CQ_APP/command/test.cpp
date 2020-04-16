#include "test.h"

#include "../file_store.h"

namespace qbot {
namespace command
{

Test::Test()
	:Command("Test") 
{ }

void 
Test::handle(MsgEvent& e)
{
	std::string a = qff233::FileStoreMgr::GetInstance()->print();
	e.sendMsg(a);
}

}
}