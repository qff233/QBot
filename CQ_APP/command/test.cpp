#include "test.h"

#include "../file_store.h"
#include "../http_client.h"

#include <fstream>

namespace qbot {
namespace command
{

Test::Test()
	:Command("Test") 
{ }

void 
Test::handle(MsgEvent& e)
{
	//std::string a = qff233::FileStore::print();
	//e.sendMsg(a);

	std::string cookie = CQ::getCookies("qun.qq.com");
	e.sendMsg(cookie);

	qff233::HttpClient hc;
	hc.setPath("/interactive/honorlist");
	hc.setQuery("gc=478399804&type=1");
	hc.setCookie(cookie);

	hc.send("qun.qq.com:80");
	std::ofstream fs;
	fs.open("htm.txt");
	fs << hc.getBody();
	fs.close();
	e.sendMsg(hc.getRequestString());
	e.sendMsg("done");
}

}
}