#pragma once

#include <memory>
#include "address.h"

namespace qff233 {
	class Socket
	{
	public:
		typedef std::shared_ptr<Socket> ptr;
		typedef std::weak_ptr<Socket> weak_ptr;

		enum Type {
			TCP = SOCK_STREAM,
			UDP = SOCK_DGRAM
		};

		enum Family {
			IPv4 = AF_INET,
			IPv6 = AF_INET6
		};


		static Socket::ptr CreateTCP(qff233::IPAddress::ptr address);
		static Socket::ptr CreateUDP(qff233::IPAddress::ptr address);
		static Socket::ptr CreateTCPSocket();
		static Socket::ptr CreateUDPSocket();
		static Socket::ptr CreateTCPSocket6();
		static Socket::ptr CreateUDPSocket6();

		Socket(int family, int type, int protocol = 0);
		virtual ~Socket();

		int64_t getSendTimeout();
		void setSendTimeout(int64_t v);
		int64_t getRecvTimeout();
		void setRecvTimeout(int64_t v);

		bool getOption(int level, int option, void* result, socklen_t* len);
		template<class T>
		bool
			getOption(int level, int option, T& result) {
			socklen_t length = sizeof(T);
			return this->getOption(level, option, &result, &length);
		}

		bool setOption(int level, int option, const void* result, socklen_t len);
		template<class T>
		bool
			setOption(int level, int option, const T& result) {
			return this->setOption(level, option, &result, sizeof(T));
		}

		virtual bool bind(const IPAddress::ptr addr);
		virtual bool listen(int backlog = SOMAXCONN);
		virtual Socket::ptr accept();

		virtual bool connect(const IPAddress::ptr addr, int64_t timeout_ms = -1);
		virtual bool reconnect(uint64_t timeout_ms = -1);

		virtual bool close();

		virtual int send(const void* buffer, size_t length, int flags = 0);
		virtual int sendTo(const void* buffer, size_t length, const IPAddress::ptr to, int flags = 0);

		virtual int recv(void* buffer, size_t length, int flags = 0);
		virtual int recvFrom(void* buffer, size_t length, const IPAddress::ptr from, int flags = 0);

		IPAddress::ptr getLocalAddress();
		IPAddress::ptr getRemoteAddress();

		int getFamily() const { return m_family; }
		int getType() const { return m_type; }
		int getProtocol() const { return m_protocol; }
		bool isConnected() const { return m_isConnected; }
		SOCKET getSocket() const { return m_sock; }

		bool isValid() const;
		int getError();

		virtual std::ostream& dump(std::ostream& os) const;
		virtual std::string toString() const;
	protected:
		void initSock();
		void newSock();
		virtual bool init(int sock);

		void structLocalAddress();
		void structRemoteAddress();
	protected:
		SOCKET m_sock;
		int m_family;
		int m_type;
		int m_protocol;
		bool m_isConnected;
		IPAddress::ptr m_localAddress;
		IPAddress::ptr m_remoteAddress;
	};

}