#include "http.h"

#include <iostream>
#include <sstream>

namespace qff233 {
namespace http
{

HttpMethod
String2HttpMethod(const std::string& m)
{
#define XX(num, name, string) \
    if(strcmp(#string, m.c_str()) == 0) { \
        return HttpMethod::name; \
    }
    HTTP_METHOD_MAP(XX);
#undef XX
    return HttpMethod::INVALID_METHOD;
}

HttpMethod
String2HttpMethod(const char* m)
{
#define XX(num, name, string) \
    if(strncmp(#string, m, strlen(#string)) == 0) { \
        return HttpMethod::name; \
    }
    HTTP_METHOD_MAP(XX);
#undef XX
    return HttpMethod::INVALID_METHOD;
}

static const char* s_method_string[] = {
#define XX(num, name, string) #string,
    HTTP_METHOD_MAP(XX)
#undef XX
};

const char*
HttpMethod2String(const HttpMethod& m)
{
    uint32_t idx = (uint32_t)m;
    if (idx >= (sizeof(s_method_string) / sizeof(s_method_string[0]))) {
        return "<unknow>";
    }
    return s_method_string[idx];
}

const char*
HttpStatus2String(const HttpStatus& s)
{
    switch (s) {
#define XX(code, name, msg) \
    case HttpStatus::name: \
        return #msg;
        HTTP_STATUS_MAP(XX);
#undef XX
    default:
        return "<unknow>";
    }
}

bool
CaseInsensitiveLess::operator()(const std::string& lhs, const std::string& rhs) const
{
    return _stricmp(lhs.c_str(), rhs.c_str()) < 0;
}

HttpRequest::HttpRequest(uint8_t version, bool close)
    :m_version(version),
     m_method(HttpMethod::GET),
     m_path("/"),
     m_websocket(false),
     m_close(close)
{ }

std::string
HttpRequest::getHeader(const std::string& key, const std::string& def) const
{
    auto it = m_headers.find(key);
    return it == m_headers.end() ? def : it->second;
}

std::string 
HttpRequest::getParam(const std::string& key, const std::string& def) const
{
    auto it = m_params.find(key);
    return it == m_params.end() ? def : it->second;
}

std::string 
HttpRequest::getCookie(const std::string& key, const std::string& def) const
{
    auto it = m_cookies.find(key);
    return it == m_headers.end() ? def : it->second;
}

void 
HttpRequest::setHeader(const std::string& key, const std::string& val)
{
    m_headers[key] = val;
}

void
HttpRequest::setParam(const std::string& key, const std::string& val)
{
    m_params[key] = val;
}

void 
HttpRequest::setCookie(const std::string& key, const std::string& val)
{
    m_cookies[key] = val;
}

void 
HttpRequest::delHeader(const std::string& key)
{
    m_headers.erase(key);
}

void
HttpRequest::delParam(const std::string& key)
{
    m_params.erase(key);
}

void
HttpRequest::delCookie(const std::string& key)
{
    m_cookies.erase(key);
}

std::ostream& 
HttpRequest::dump(std::ostream& os) const
{
    //GET / HTTP/1.1
    //Host: qff233.cn
    os << HttpMethod2String(m_method) << " "
        << m_path
        << (m_query.empty() ? "" : "?")
        << m_query
        << " HTTP/"
        << ((uint32_t)(m_version >> 4))
        << "."
        << ((uint32_t)(m_version & 0x0F))
        << "\r\n";
    if (!m_websocket) {
        os << "connection: " << (m_close ? "close" : "keep-alive") << "\r\n";
    }
    for (auto& i : m_headers) {
        if (!m_websocket && _stricmp(i.first.c_str(), "connection") == 0) {
            continue;
        }
        os << i.first << ": " << i.second << "\r\n";
    }

    if (!m_body.empty()) {
        os << "content-length: " << m_body.size() << "\r\n\r\n"
            << m_body;
    }
    else {
        os << "\r\n";
    }
    return os;
}

std::string
HttpRequest::toString() const
{
    std::stringstream ss;
    this->dump(ss);
    return ss.str();
}

HttpResponse::HttpResponse(uint8_t version, bool close)
    :m_version(version),
     m_status(HttpStatus::OK),
     m_websocket(false),
     m_close(close)
{ }
    
std::string 
HttpResponse::getHeader(const std::string& key, const std::string& def) const
{
    auto it = m_headers.find(key);
    return it == m_headers.end() ? def : it->second;
}

void
HttpResponse::setHeader(const std::string& key, const std::string& val)
{
    m_headers[key] = val;
}

void 
HttpResponse::delHeader(const std::string& key)
{
    m_headers.erase(key);
}

std::ostream&
HttpResponse::dump(std::ostream& os) const
{
    // HTTP/1.1 200 OK
    os << "HTTP/"
        << ((uint32_t)(m_version >> 4))
        << "."
        << ((uint32_t)(m_version & 0x0F))
        << " "
        << (uint32_t)m_status
        << " "
        << (m_reason.empty() ? HttpStatus2String(m_status) : m_reason)
        << "\r\n";

    for (auto& i : m_headers) {
        if (!m_websocket && stricmp(i.first.c_str(), "connection") == 0) {
            continue;
        }
        os << i.first << ": " << i.second << "\r\n";
    }
    for (auto& i : m_cookies) {
        os << "Set-Cookie: " << i << "\r\n";
    }
    if (!m_websocket) {
        os << "connection: " << (m_close ? "close" : "keep-alive") << "\r\n";
    }
    if (!m_body.empty()) {
        os << "content-length: " << m_body.size() << "\r\n\r\n";
    }
    else {
        os << "\r\n";
    }
    return os;
}

std::string 
HttpResponse::toString() const
{
    std::stringstream ss;
    this->dump(ss);
    return ss.str();
}

void
HttpResponse::setRedirect(const std::string& uri)
{
    m_status = HttpStatus::FOUND;
    this->setHeader("Location", uri);
}


}
}