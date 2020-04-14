#include "socket.h"

#include <iostream>
#include <sstream>

namespace qff233
{

    Socket::ptr
        Socket::CreateTCP(qff233::IPAddress::ptr address)
    {
        Socket::ptr sock(new Socket(address->getFamily(), TCP, 0));
        return sock;
    }

    Socket::ptr
        Socket::CreateUDP(qff233::IPAddress::ptr address)
    {
        Socket::ptr sock(new Socket(address->getFamily(), UDP, 0));
        sock->newSock();
        sock->m_isConnected = true;
        return sock;
    }

    Socket::ptr
        Socket::CreateTCPSocket()
    {
        Socket::ptr sock(new Socket(IPv4, TCP, 0));
        return sock;
    }

    Socket::ptr
        Socket::CreateUDPSocket()
    {
        Socket::ptr sock(new Socket(IPv4, UDP, 0));
        sock->newSock();
        sock->m_isConnected = true;
        return sock;
    }

    Socket::ptr
        Socket::CreateTCPSocket6()
    {
        Socket::ptr sock(new Socket(IPv6, TCP, 0));
        return sock;
    }

    Socket::ptr
        Socket::CreateUDPSocket6()
    {
        Socket::ptr sock(new Socket(IPv6, UDP, 0));
        sock->newSock();
        sock->m_isConnected = true;
        return sock;
    }

    Socket::Socket(int family, int type, int protocol)
        :m_sock(-1),
        m_family(family),
        m_type(type),
        m_protocol(protocol),
        m_isConnected(false)
    { }

    Socket::~Socket()
    {
        this->close();
    }

    int64_t
        Socket::getSendTimeout()
    {
        int time_ms;
        this->getOption(SOL_SOCKET, SO_SNDTIMEO, time_ms);
        return  time_ms;
    }

    void
        Socket::setSendTimeout(int64_t v)
    {
        this->setOption(SOL_SOCKET, SO_SNDTIMEO, v);
    }

    int64_t
        Socket::getRecvTimeout()
    {
        int time_ms;
        this->getOption(SOL_SOCKET, SO_RCVTIMEO, time_ms);
        return time_ms;
    }

    void
        Socket::setRecvTimeout(int64_t v)
    {
        this->setOption(SOL_SOCKET, SO_RCVTIMEO, v);
    }

    bool
        Socket::getOption(int level, int option,
            void* result, socklen_t* len)
    {
        int rt = ::getsockopt(m_sock, level, option, (char*)result, len);
        if (rt) {
            //TODO error log
            std::cout << "getOption is error" << std::endl;
            return false;
        }
        return true;
    }

    bool
        Socket::setOption(int level, int option, const void* result, socklen_t len)
    {
        int rt = ::setsockopt(m_sock, level, option, (const char*)result, len);
        if (rt) {
            //TODO error log
            std::cout << "setOption is error" << std::endl;
            return false;
        }
        return true;
    }

    void
        Socket::initSock()
    {
        int val = 1;
        this->setOption(SOL_SOCKET, SO_REUSEADDR, val);
        if (m_type == SOCK_STREAM) {
            this->setOption(IPPROTO_TCP, TCP_NODELAY, val);
        }
    }

    void
        Socket::newSock()
    {
        m_sock = ::socket(m_family, m_type, m_protocol);
        if (m_sock != -1) {
            this->initSock();
        }
        else {
            //TODO err log
            std::cout << "socket(" << m_family
                << ", " << m_type << ", " << m_protocol << ") errno="
                << errno << " errstr=" << strerror(errno);
        }
    }

    bool
        Socket::init(int sock)
    {
        m_sock = sock;
        m_isConnected = true;
        this->initSock();
        this->structLocalAddress();
        this->structRemoteAddress();
        return true;
    }


    bool
        Socket::bind(const IPAddress::ptr addr)
    {
        if (!this->isValid()) {
            this->newSock();
            if (!this->isValid()) {
                return false;
            }
        }

        if (addr->getFamily() != m_family) {
            //TODO error log
            std::cout << "bind sock.family("
                << m_family << ") addr.family(" << addr->getFamily()
                << ") not equal, addr=" << addr->toString() << std::endl;
            return false;
        }

        if (::bind(m_sock, addr->getAddr(), addr->getAddrLen())) {
            //TODO error log
            std::cout << "bind error errrno=" << errno
                << " errstr=" << strerror(errno) << std::endl;
            return false;
        }
        this->structLocalAddress();
        return true;
    }

    bool
        Socket::listen(int backlog)
    {
        if (!this->isValid()) {
            //TODO error log
            std::cout << "listen error sock=-1" << std::endl;
            return false;
        }
        if (::listen(m_sock, backlog)) {
            std::cout << "list error errno=" << errno
                << " errstr" << strerror(errno) << std::endl;
            return false;
        }
        return true;
    }

    Socket::ptr
        Socket::accept()
    {
        int newsock = ::accept(m_sock, nullptr, nullptr);
        if (newsock == -1) {
            //TODO error log
            std::cout << "accept(" << m_sock << ") errno="
                << errno << " errstr=" << strerror(errno) << std::endl;
            return nullptr;
        }
        Socket::ptr sock(new Socket(m_family, m_type, m_protocol));
        if (sock->init(newsock)) {
            return sock;
        }
        return nullptr;
    }

    bool
        Socket::connect(const IPAddress::ptr addr, int64_t timeout_ms)
    {
        m_remoteAddress = addr;
        if (!this->isValid()) {
            this->newSock();
            if (!this->isValid()) {
                return false;
            }
        }

        if (addr->getFamily() != m_family) {
            //TODO err log
            std::cout << "connect sock.family("
                << m_family << ") addr.family(" << addr->getFamily()
                << ") not equal, addr=" << addr->toString();
            return false;
        }

        if (timeout_ms == -1) {
            if (::connect(m_sock, addr->getAddr(), addr->getAddrLen())) {
                //TODO err log
                std::cout << "sock=" << m_sock << " connect(" << addr->toString()
                    << ") error errno=" << errno << "  errstr="
                    << strerror(errno) << std::endl;
                this->close();
                return false;
            }
            m_isConnected = true;
            this->structRemoteAddress();
            this->structLocalAddress();
            return true;
        }
        else {
            struct timeval timeout;
            bool ret = false;
            fd_set r;
            unsigned long ul = 1;
            ioctlsocket(m_sock, FIONBIO, &ul);
            if (::connect(m_sock, addr->getAddr(), addr->getAddrLen()) == -1) {
                int error = -1;
                FD_ZERO(&r);
                FD_SET(m_sock, &r);
                timeout.tv_sec = (long)(timeout_ms / 1000);
                timeout.tv_usec = (long)(timeout_ms % 1000 * 1000);
                if (select(0, NULL, &r, NULL, &timeout) > 0) {
                    getOption(SOL_SOCKET, SO_ERROR, error);
                    if (error == 0)
                        ret = true;
                    else
                        ret = false;
                }
                else
                    ret = false;
            }
            else
                ret = true;
            ul = 0;
            ioctlsocket(m_sock, FIONBIO, &ul);
            if (ret) {
                m_isConnected = true;
                this->structRemoteAddress();
                this->structLocalAddress();
                return true;
            }
            else {
                return false;
            }
        }
        m_isConnected = true;
        this->structRemoteAddress();
        this->structLocalAddress();
        return true;
    }

    bool
        Socket::reconnect(uint64_t timeout_ms)
    {
        if (!m_remoteAddress) {
            //TODO error log
            std::cout << "reconnect m_remoteAddress is null" << std::endl;
            return false;
        }
        m_localAddress.reset();
        return this->connect(m_remoteAddress, timeout_ms);
    }

    bool
        Socket::close()
    {
        if (!m_isConnected && m_sock == -1) {
            return false;
        }
        m_isConnected = false;
        if (m_sock != -1) {
            ::closesocket(m_sock);
            m_sock = -1;
        }
        return true;
    }

    int
        Socket::send(const void* buffer, size_t length, int flags)
    {
        if (this->isConnected()) {
            return ::send(m_sock, (const char*)buffer, length, flags);
        }
        return -1;
    }

    int
        Socket::sendTo(const void* buffer, size_t length,
            const IPAddress::ptr to, int flags)
    {
        if (this->isConnected()) {
            return ::sendto(m_sock, (const char*)buffer, length,
                flags, to->getAddr(), to->getAddrLen());
        }
        return -1;
    }

    int
        Socket::recv(void* buffer, size_t length, int flags)
    {
        if (this->isConnected()) {
            return ::recv(m_sock, (char*)buffer, length, flags);
        }
        return -1;
    }

    int
        Socket::recvFrom(void* buffer, size_t length,
            const IPAddress::ptr from, int flags)
    {
        if (this->isConnected()) {
            socklen_t len = from->getAddrLen();
            return ::recvfrom(m_sock, (char*)buffer, length,
                flags, from->getAddr(), &len);
        }
        return -1;
    }


    IPAddress::ptr
        Socket::getLocalAddress()
    {
        if (m_localAddress) {
            return m_localAddress;
        }
        return nullptr;
    }

    IPAddress::ptr
        Socket::getRemoteAddress()
    {
        if (m_remoteAddress) {
            return m_remoteAddress;
        }
        return nullptr;
    }

    void
        Socket::structLocalAddress()
    {
        IPAddress::ptr result;
        switch (m_family)
        {
        case AF_INET:
            result.reset(new IPv4Address);
            break;
        case AF_INET6:
            result.reset(new IPv6Address);
            break;
        default:
            break;
        }
        socklen_t addrlen = result->getAddrLen();
        if (::getsockname(m_sock, result->getAddr(), &addrlen)) {
            //TODO err log
            std::cout << "getsockname error sock=" << m_sock
                << " errno=" << errno << " errstr=" << strerror(errno);
            return;
        }
        m_localAddress = result;
    }

    void
        Socket::structRemoteAddress()
    {
        IPAddress::ptr result;
        switch (m_family)
        {
        case AF_INET:
            result.reset(new IPv4Address);
            break;
        case AF_INET6:
            result.reset(new IPv6Address);
            break;
        default:
            break;
        }
        socklen_t addrlen = result->getAddrLen();
        if (getpeername(m_sock, result->getAddr(), &addrlen)) {
            //TODO error log
            return;
        }
        m_remoteAddress = result;
    }

    bool
        Socket::isValid() const
    {
        return m_sock != -1;
    }

    int
        Socket::getError()
    {
        int error = 0;
        socklen_t len = sizeof(error);
        error = WSAGetLastError();
        if (error) {
            return error;
        }
        if (this->getOption(SOL_SOCKET, SO_ERROR, error)) {
            return error;
        }
    }

    std::ostream&
        Socket::dump(std::ostream& os) const
    {
        os << "[Socket sock=" << m_sock
            << " is_connected=" << m_isConnected
            << " family=" << m_family
            << " type=" << m_type
            << " protocol=" << m_protocol;
        if (m_localAddress) {
            os << " local_address=" << m_localAddress->toString();
        }
        if (m_remoteAddress) {
            os << " remote_address=" << m_remoteAddress->toString();
        }
        os << "]";
        return os;
    }

    std::string
        Socket::toString() const
    {
        std::stringstream ss;
        this->dump(ss);
        return ss.str();
    }

}