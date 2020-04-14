#include "address.h"

#include <iostream>
#include <sstream>
#include <algorithm>

namespace qff233
{

	static int err = 0;

	namespace
	{
		struct __InitWSA {
			__InitWSA()
			{
				WSAData wsaData;
				err = WSAStartup(MAKEWORD(2, 2), &wsaData);
			}
		};

		static __InitWSA InitWsa__;
	}

	int get_error() {
		return err;
	}

	template<class T>
	static T
		CreateMask(uint32_t bits)
	{
		return (1 << (sizeof(T) * 8 - bits)) - 1;
	}

	IPAddress::ptr
		IPAddress::Create(const sockaddr* addr, socklen_t addrlen)
	{
		if (addr == nullptr)
			return nullptr;

		IPAddress::ptr result;
		switch (addr->sa_family)
		{
		case AF_INET:
			result.reset(new IPv4Address(*(const sockaddr_in*)addr));
			break;
		case AF_INET6:
			result.reset(new IPv6Address(*(const sockaddr_in6*)addr));
			break;
		default:
			break;
		}
		return result;
	}

	IPAddress::ptr
		IPAddress::Create(const char* address, socklen_t port)
	{
		addrinfo hint, * results;
		memset(&hint, 0, sizeof(hint));

		hint.ai_flags = AI_NUMERICHOST;
		hint.ai_family = AF_UNSPEC;

		int error = getaddrinfo(address, NULL, &hint, &results);
		if (error) {
			//TODO error log
			return nullptr;
		}

		try {
			IPAddress::ptr result = Create(results->ai_addr, (socklen_t)results->ai_addrlen);
			if (result) {
				result->setPort(port);
			}
			freeaddrinfo(results);
			return result;
		}
		catch (...) {
			freeaddrinfo(results);
			return nullptr;
		}
	}

	bool
		IPAddress::Lookup(std::vector<IPAddress::ptr>& result, const std::string& host,
			int family, int type, int protocol)
	{
		if (err) {
			//TODO err log
			std::cout << "WSAStartup error" << std::endl;
			return false;
		}
		addrinfo hints, * results, * next;
		hints.ai_flags = 0;
		hints.ai_family = family;
		hints.ai_socktype = type;
		hints.ai_protocol = protocol;
		hints.ai_addrlen = 0;
		hints.ai_canonname = NULL;
		hints.ai_addr = NULL;
		hints.ai_next = NULL;

		std::string node;
		std::string service;
		//ipv6
		if (!host.empty() && host[0] == '[') {
			size_t right = host.find(']');
			if (right == std::string::npos) {
				//TODO error log
				std::cout << "inval host" << std::endl;
				return false;
			}
			node = host.substr(1, right - 1);
			if (host.find(':', right) != std::string::npos) {
				service = host.substr(right + 2, host.size() - right - 2);
			}
		}
		//ipv4
		if (node.empty()) {
			//127.0.0.1:xx
			size_t colon = host.find(':');
			if (colon != std::string::npos) {
				if (host.find(':', colon + 1) == std::string::npos) {
					node = host.substr(0, colon);
					service = host.substr(colon + 1, host.size() - colon - 1);
				}
				else {
					//TODO erro log
					std::cout << "inval host" << std::endl;
				}
			}
		}

		if (node.empty()) {
			node = host;
		}

		int error = getaddrinfo(node.c_str(), service.c_str(), &hints, &results);
		if (error) {
			//TODO error log
			std::cout << "getaddrinfo error:" << error << "   " << strerror(error) << std::endl;
			return false;
		}

		next = results;
		while (next) {
			result.push_back(Create(next->ai_addr, (socklen_t)next->ai_addrlen));
			next = next->ai_next;
		}

		freeaddrinfo(results);
		return !result.empty();
	}

	IPAddress::ptr
		IPAddress::LookupAny(const std::string& host,
			int family, int type, int protocol)
	{
		std::vector<IPAddress::ptr> result;
		if (Lookup(result, host, family, type, protocol)) {
			return result[0];
		}
		return nullptr;
	}


	int
		IPAddress::getFamily() const
	{
		return this->getAddr()->sa_family;
	}

	std::string
		IPAddress::toString() const
	{
		std::stringstream ss;
		this->insert(ss);
		return ss.str();
	}

	bool
		IPAddress::operator<(const IPAddress& rhs) const
	{
		socklen_t minlen = std::min(getAddrLen(), rhs.getAddrLen());
		int result = memcmp(getAddr(), rhs.getAddr(), minlen);
		if (result < 0) {
			return true;
		}
		else if (result > 0) {
			return false;
		}
		else if (getAddrLen() < rhs.getAddrLen()) {
			return true;
		}
		return false;
	}

	bool
		IPAddress::operator==(const IPAddress& rhs) const
	{
		return getAddrLen() == rhs.getAddrLen()
			&& memcmp(getAddr(), rhs.getAddr(), getAddrLen());
	}

	bool
		IPAddress::operator!=(const IPAddress& rhs) const
	{
		return !(*this == rhs);
	}

	IPv4Address::ptr
		IPv4Address::Create(const char* address, uint16_t port)
	{
		IPv4Address::ptr rt(new IPv4Address);
		rt->setPort(port);
		int result = inet_pton(AF_INET, address, &rt->m_addr.sin_addr.S_un.S_addr);
		if (result <= 0) {
			//TODO erro log
			std::cout << "Pv4Address::Create error " << result << std::endl;
			return nullptr;
		}
		return rt;
	}

	IPv4Address::IPv4Address(const sockaddr_in& address)
		:m_addr(address)
	{ }

	IPv4Address::IPv4Address(uint32_t address, uint16_t port)
	{
		memset(&m_addr, 0, sizeof(m_addr));
		m_addr.sin_addr.S_un.S_addr = htonl(address);
		this->setPort(port);
	}

	const sockaddr*
		IPv4Address::getAddr() const
	{
		return (sockaddr*)&m_addr;
	}

	sockaddr*
		IPv4Address::getAddr()
	{
		return (sockaddr*)&m_addr;
	}

	socklen_t
		IPv4Address::getAddrLen() const
	{
		return sizeof(m_addr);
	}

	std::ostream&
		IPv4Address::insert(std::ostream& os) const
	{
		uint32_t addr = ntohl(m_addr.sin_addr.S_un.S_addr);
		os << ((addr >> 24) & 0xff) << "."
			<< ((addr >> 16) & 0xff) << "."
			<< ((addr >> 8) & 0xff) << "."
			<< (addr & 0xff);
		os << ":" << this->getPort();
		return os;
	}

	IPAddress::ptr
		IPv4Address::broadcastAddress(uint32_t prefix_len)
	{
		if (prefix_len > 32)
			return nullptr;

		sockaddr_in baddr(m_addr);
		baddr.sin_addr.S_un.S_addr
			|= htonl(CreateMask<uint32_t>(prefix_len));
		return IPv4Address::ptr(new IPv4Address(baddr));
	}

	IPAddress::ptr
		IPv4Address::networdAddress(uint32_t prefix_len)
	{
		if (prefix_len > 32)
			return nullptr;

		sockaddr_in baddr(m_addr);
		baddr.sin_addr.S_un.S_addr
			&= htonl(CreateMask<uint32_t>(prefix_len));
		return IPv4Address::ptr(new IPv4Address(baddr));
	}

	IPAddress::ptr
		IPv4Address::subnetMask(uint32_t prefix_len)
	{
		if (prefix_len > 32)
			return nullptr;

		sockaddr_in subnet;
		memset(&subnet, 0, sizeof(subnet));
		subnet.sin_family = AF_INET;
		subnet.sin_addr.S_un.S_addr
			= ~htonl(CreateMask<uint32_t>(prefix_len));
		return IPv4Address::ptr(new IPv4Address(subnet));
	}

	uint32_t
		IPv4Address::getPort() const
	{
		return ntohs(m_addr.sin_port);
	}

	void
		IPv4Address::setPort(uint16_t v)
	{
		m_addr.sin_port = htons(v);
	}

	IPv6Address::ptr
		IPv6Address::Create(const char* address, uint16_t port)
	{
		IPv6Address::ptr rt(new IPv6Address);
		rt->setPort(port);
		int result = inet_pton(AF_INET6, address, &rt->m_addr.sin6_addr.u.Byte);
		if (result <= 0) {
			//TODO err log
			std::cout << "IPv6Address::Create error:" << result << std::endl;
			return nullptr;
		}
		return rt;
	}

	IPv6Address::IPv6Address()
	{
		memset(&m_addr, 0, this->getAddrLen());
		m_addr.sin6_family = AF_INET6;
	}

	IPv6Address::IPv6Address(const sockaddr_in6& address)
		:m_addr(address)
	{ }

	IPv6Address::IPv6Address(const uint8_t address[16], uint16_t port)
	{
		memset(&m_addr, 0, sizeof(m_addr));
		m_addr.sin6_family = AF_INET6;
		m_addr.sin6_port = htons(port);
		memcpy(&m_addr.sin6_addr.u.Byte, address, 16);
	}

	const sockaddr*
		IPv6Address::getAddr() const
	{
		return (sockaddr*)&m_addr;
	}

	sockaddr*
		IPv6Address::getAddr()
	{
		return (sockaddr*)&m_addr;
	}

	socklen_t
		IPv6Address::getAddrLen() const
	{
		return sizeof(m_addr);
	}

	std::ostream&
		IPv6Address::insert(std::ostream& os) const
	{
		os << "[";
		uint16_t* addr = (uint16_t*)m_addr.sin6_addr.u.Byte;
		bool use_zeros = false;
		for (size_t i = 0; i < 8; ++i) {
			if (addr[i] == 0 && !use_zeros) {
				continue;
			}
			if (i && addr[i - 1] == 0 && !use_zeros) {
				os << ":";
				use_zeros = true;
			}
			if (i) {
				os << ":";
			}
			os << std::hex << (uint16_t)((int)ntohl(addr[i]) >> 16) << std::dec;
		}

		if (!use_zeros && addr[7] == 0) {
			os << "::";
		}

		os << "]:" << this->getPort();
		return os;
	}

	IPAddress::ptr
		IPv6Address::broadcastAddress(uint32_t prefix_len)
	{
		sockaddr_in6 baddr(m_addr);
		baddr.sin6_addr.u.Byte[prefix_len / 8]
			|= CreateMask<uint8_t>(prefix_len % 8);
		for (int i = prefix_len / 8 + 1; i < 16; ++i) {
			baddr.sin6_addr.u.Byte[i] = 0xff;
		}
		return IPv6Address::ptr(new IPv6Address(baddr));
	}

	IPAddress::ptr
		IPv6Address::networdAddress(uint32_t prefix_len)
	{
		sockaddr_in6 baddr(m_addr);
		baddr.sin6_addr.u.Byte[prefix_len / 8]
			&= CreateMask<uint8_t>(prefix_len % 8);
		for (int i = prefix_len / 8 + 1; i < 16; ++i) {
			baddr.sin6_addr.u.Byte[i] = 0x00;
		}
		return IPv6Address::ptr(new IPv6Address(baddr));
	}

	IPAddress::ptr
		IPv6Address::subnetMask(uint32_t prefix_len)
	{
		sockaddr_in6 subnet;
		memset(&subnet, 0, sizeof(subnet));
		subnet.sin6_family = AF_INET6;
		subnet.sin6_addr.u.Byte[prefix_len / 8]
			= ~CreateMask<uint8_t>(prefix_len % 8);

		for (uint32_t i = 0; i < prefix_len / 8; ++i) {
			subnet.sin6_addr.u.Byte[i] = 0xff;
		}
		return IPv6Address::ptr(new IPv6Address(subnet));
	}

	uint32_t
		IPv6Address::getPort() const
	{
		return ntohs(m_addr.sin6_port);
	}

	void
		IPv6Address::setPort(uint16_t v)
	{
		m_addr.sin6_port = htons(v);
	}


}