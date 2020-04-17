#include "http_client.h"

#include "socket/socket.h"
#include "util/common.h"

#include <fstream>

namespace qff233
{

HttpClient::HttpClient()
	:m_request(new http::HttpRequest),
	m_response(new http::HttpResponse)
{ }

void 
HttpClient::setMethod(http::HttpMethod v)
{
	m_request->setMethod(v);
}

void
HttpClient::setPath(const std::string& v)
{
	m_request->setPath(v);
}

void
HttpClient::setQuery(const std::string& v)
{
	m_request->setQuery(v);
}

void
HttpClient::setBody(const std::string& v)
{
	m_request->setBody(v);
}

void
HttpClient::setHeader(const std::string& key, const std::string& val)
{
	m_request->setHeader(key, val);
}

int 
HttpClient::send(const std::string& hostname)
{
	auto addr = IPv4Address::LookupAny(hostname);
	auto socketClient = Socket::CreateTCPSocket();

	size_t host_name_port = hostname.find(":");
	if (host_name_port != std::string::npos) {
		this->setHeader("Host", hostname.substr(0, host_name_port));
	}
	else {
		this->setHeader("Host", hostname);
	}
	if (!m_cookies.empty())
	{
		this->setHeader("Cookie", m_cookies);
	}
	this->setHeader("Accept-Encoding", "gzip");

	std::string buffer = gbk2utf8(m_request->toString().c_str());
	if (!socketClient->connect(addr, 10000)) {
		GetLogger()->Error("HttpClient连接失败");
		return -1;
	}

	int rt = socketClient->send(buffer.c_str(), buffer.size());
	if (rt < 0) {
		GetLogger()->Error("HttpClient发送失败");
		return -1;
	}

	buffer.clear();
	buffer.resize(1024 * 128);
	rt = socketClient->recv(&buffer[0], 8192);
	if (rt <= 0) {
		GetLogger()->Error("HttpClient接受失败");
		return -1;
	}


	//std::ofstream fs;
	//fs.open("htm1.txt");
	//fs << buffer;
	//fs.close();

	int8_t v = buffer[7] - '0';
	v |= ((buffer[5] - '0') << 4);
	m_response->setVersion(v);
	size_t idx = buffer.find("\r\n");
	//  HTTP/1.1 200 OK/r/n
	buffer.erase(0, idx + 2);
	bool finish = false;
	while (true) {
		idx = buffer.find("\r\n");
		if (buffer.substr(idx, 4) == "\r\n\r\n") {
			finish = true;
		}
		std::string substr = buffer.substr(0, idx);
		size_t port = substr.find(":");
		m_response->setHeader(substr.substr(0, port), substr.substr(port + 2, substr.size() - port - 2));
		if (!finish) {
			buffer.erase(0, idx + 2);
			continue;
		}
		buffer.erase(0, idx + 4);
		break;
	}
	m_response->setBody(buffer);
}

std::string 
HttpClient::getBody()
{
	return m_response->getBody();
}

std::string 
HttpClient::getHeader(const std::string& name)
{
	return m_response->getHeader(name);
}

const http::HttpResponse::MapType& 
HttpClient::getHeader()
{
	return m_response->getHeaders();
}

}