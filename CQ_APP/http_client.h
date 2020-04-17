#pragma once

#include <memory>
#include <string>

#include "http/http.h"

namespace qff233 
{


class HttpClient 
{
public:
	typedef std::shared_ptr<HttpClient> ptr;
	HttpClient();

	void setMethod(http::HttpMethod v);
	void setPath(const std::string& v);
	void setQuery(const std::string& v);
	void setBody(const std::string& v);
	void setHeader(const std::string& key, const std::string& val);
	void setCookie(const std::string& val) { m_cookies = val; }

	int send(const std::string& hostname);

	std::string getBody();
	std::string getHeader(const std::string& name);
	const http::HttpResponse::MapType& getHeader();

	std::string getRequestString() { return m_request->toString(); }
private:
	http::HttpRequest::ptr m_request;
	http::HttpResponse::ptr m_response;
	std::string m_cookies;
};

}