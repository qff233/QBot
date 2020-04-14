#pragma once

#include <memory>
#include <string>
#include <vector>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment (lib, "Ws2_32.lib")

namespace qff233
{

	class IPAddress
	{
	public:
		typedef std::shared_ptr<IPAddress> ptr;

		static IPAddress::ptr Create(const char* address, socklen_t port = 0);
		static bool Lookup(std::vector<IPAddress::ptr>& result, const std::string& host,
			int family = AF_INET, int type = 0, int protocol = 0);
		static IPAddress::ptr LookupAny(const std::string& host,
			int family = AF_INET, int type = 0, int protocol = 0);


		virtual ~IPAddress() { }

		int getFamily() const;
		virtual const sockaddr* getAddr() const = 0;
		virtual sockaddr* getAddr() = 0;
		virtual socklen_t getAddrLen() const = 0;
		virtual std::ostream& insert(std::ostream& os) const = 0;
		std::string toString() const;

		virtual IPAddress::ptr broadcastAddress(uint32_t prefix_len) = 0;
		virtual IPAddress::ptr networdAddress(uint32_t prefix_len) = 0;
		virtual IPAddress::ptr subnetMask(uint32_t prefix_len) = 0;

		virtual uint32_t getPort() const = 0;
		virtual void setPort(uint16_t v) = 0;

		bool operator<(const IPAddress& rhs) const;
		bool operator==(const IPAddress& rhs) const;
		bool operator!=(const IPAddress& rhs) const;
	private:
		static IPAddress::ptr Create(const sockaddr* addr, socklen_t addrlen);
	};

	class IPv4Address :public IPAddress
	{
	public:
		typedef std::shared_ptr<IPv4Address> ptr;

		static IPv4Address::ptr Create(const char* address, uint16_t port = 0);

		IPv4Address(const sockaddr_in& address);
		IPv4Address(uint32_t address = INADDR_ANY, uint16_t port = 0);

		const sockaddr* getAddr() const override;
		sockaddr* getAddr() override;
		socklen_t getAddrLen() const override;
		std::ostream& insert(std::ostream& os) const override;

		IPAddress::ptr broadcastAddress(uint32_t prefix_len) override;
		IPAddress::ptr networdAddress(uint32_t prefix_len) override;
		IPAddress::ptr subnetMask(uint32_t prefix_len) override;

		uint32_t getPort() const override;
		void setPort(uint16_t v) override;
	private:
		sockaddr_in m_addr;
	};

	class IPv6Address : public IPAddress
	{
	public:
		typedef std::shared_ptr<IPv6Address> ptr;

		static IPv6Address::ptr Create(const char* address, uint16_t port = 0);

		IPv6Address();
		IPv6Address(const sockaddr_in6& address);
		IPv6Address(const uint8_t address[16], uint16_t port = 0);

		const sockaddr* getAddr() const override;
		sockaddr* getAddr() override;
		socklen_t getAddrLen() const override;
		std::ostream& insert(std::ostream& os) const override;

		IPAddress::ptr broadcastAddress(uint32_t prefix_len) override;
		IPAddress::ptr networdAddress(uint32_t prefix_len) override;
		IPAddress::ptr subnetMask(uint32_t prefix_len) override;

		uint32_t getPort() const override;
		void setPort(uint16_t v) override;
	private:
		sockaddr_in6 m_addr;
	};

	int get_error();

}