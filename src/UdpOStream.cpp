#include "UdpOStream.h"
#include <iostream>
#include <boost/url.hpp>

#ifdef _WIN32
#include <io.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#endif

#ifdef _WIN32
#define sprintf sprintf_s
#else
#define sprintf sprintf
#define SOCKET int
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define PVOID void*
#define SD_SEND SHUT_WR
#define SOCKADDR sockaddr
#define IN_ADDR in_addr
#endif


using namespace std;
const int BUFLEN = 1000;


class UdpStreamBuf::Impl
{
public:
    Impl() {}
    Impl(const char* ipaddr, uint16_t port, unsigned char ttl, const char* iface_addr)
        : _port(port)
    {
        initSocket(ipaddr, port, ttl, iface_addr);
    }

    virtual ~Impl()
    {
        if (_socket != INVALID_SOCKET)
        {
            if (!_label.empty())
            {
                send(_label.c_str(), _label.size());
            }

            shutdown(_socket, SD_SEND);
#ifdef _WIN32
            closesocket(_socket);
#else
            close(_socket);
#endif
        }
    }

public:
    void send(const char* data, size_t length);
    void initSocket(const char* ipaddr, unsigned short port, unsigned char ttl, const char* iface_addr);

public:
    SOCKET _socket{ INVALID_SOCKET };
    bool _run{ true };
    sockaddr_in _recvAddr{};
    uint32_t _address{};
    uint16_t _port{};
    std::string _label;
    int _prev{ 0 };
    int _cur{ 0 };
};

void UdpStreamBuf::Impl::initSocket(const char* ipaddr, uint16_t port, unsigned char ttl, const char* iface_addr)
{
    char szErr[BUFSIZ]{};
    IN_ADDR inaddr;
#ifdef _WIN32
    WORD winsock_version, err;
    WSADATA winsock_data;
    winsock_version = MAKEWORD(2, 2);

    err = WSAStartup(winsock_version, &winsock_data);
    if (err != 0)
    {
        std::exception exp("Failed to initialize WinSock");
        throw exp;
    }
#endif

    //----------------------
    // Create a SOCKET for connecting to server
    _socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (_socket == INVALID_SOCKET) {
#ifdef _WIN32
        WSACleanup();
        sprintf(szErr, "Error at socket(): %d", WSAGetLastError());
#else
        sprintf(szErr, "Error at socket(): %d", errno);
#endif
        std::runtime_error exp(szErr);
        throw exp;
    }

    // Set time to live
    unsigned char optVal = ttl;
    int optLen = sizeof(optVal);

    if (setsockopt(_socket, IPPROTO_IP, IP_MULTICAST_TTL, (char*)&optVal, optLen) == SOCKET_ERROR)
    {
#ifdef _WIN32
        WSACleanup();
        sprintf(szErr, "Error setting socket option IP_MULTICAST_TTL: %d", WSAGetLastError());
#else
        sprintf(szErr, "Error setting socket option IP_MULICAST_TTL: %d", errno);
#endif
        std::runtime_error exp(szErr);
        throw exp;
    }

    if (strlen(iface_addr) > 0)
    {
        inet_pton(AF_INET, iface_addr, (PVOID)&inaddr);
        if (setsockopt(_socket, IPPROTO_IP, IP_MULTICAST_IF, (char*)&inaddr.s_addr, sizeof(inaddr.s_addr)) == SOCKET_ERROR)
        {
#ifdef _WIN32
            WSACleanup();
            sprintf(szErr, "KlvTextWriter Error setting socket option IP_MULTICAST_IF: %d", WSAGetLastError());
#else
            sprintf(szErr, "KlvTextWriter Error setting socket option IP_MULTICAST_IF: %d", errno);
#endif
            std::runtime_error exp(szErr);
            throw exp;
        }
    }

    if (inet_pton(AF_INET, ipaddr, (PVOID)&inaddr) != 1)
    {
#ifdef _WIN32
        WSACleanup();
        sprintf_s(szErr, "Error when calling inet_pton: %d", WSAGetLastError());
#else
        sprintf(szErr, "Error when calling inet_pton: %d", errno);
#endif
        std::runtime_error exp(szErr);
        throw exp;
    }
    //----------------------
    // The sockaddr_in structure specifies the address family,
    // IP address, and port for the socket that is being send to
    _recvAddr.sin_family = AF_INET;
    _recvAddr.sin_addr.s_addr = inaddr.s_addr;
    _recvAddr.sin_port = htons(port);

    _address = _recvAddr.sin_addr.s_addr;
}

void UdpStreamBuf::Impl::send(const char* data, size_t length)
{
    // Write the data to the standard console out
    if (_socket == INVALID_SOCKET)
    {
        return;
    }

    int bufsiz = BUFLEN;
    int nLeft = length;
    size_t i = 0;
    while (i < length)
    {
        if (nLeft < bufsiz)
        {
            bufsiz = nLeft;
        }

        const int status = sendto(_socket,
            data + i,
            bufsiz,
            0,
            (SOCKADDR*)&_recvAddr,
            sizeof(_recvAddr));

        if (status == SOCKET_ERROR)
        {
#ifdef _WIN32
            const int errCode = WSAGetLastError();
#else
            const int errCode = errno;
#endif
        }
        i += BUFLEN;
        nLeft -= BUFLEN;
    }
}


UdpOStream::UdpOStream(std::string strUrl)
    : std::ostream(&_udpStreamBuf)
{
    _udpStreamBuf._pimpl = std::make_unique<UdpStreamBuf::Impl>();

    using namespace boost;
    namespace url = boost::urls;

    system::result<url::url_view> res = url::parse_uri(strUrl);
    if (!res)
    {
        char szErr[BUFSIZ]{};
        sprintf(szErr, "Fail to parse URL: %s", strUrl.c_str());
        throw std::runtime_error(szErr);
    }

    url::url_view urlView = res.value();
    string ipaddr = urlView.host();
    uint16_t port = 50000;
    uint8_t ttl = 255;
    std::string ifaceaddr;

    if (urlView.has_port())
    {
        port = urlView.port_number();
    }

    if (urlView.has_query())
    {
        url::params_encoded_view params_ref = urlView.encoded_params();

        for (const auto& v : params_ref)
        {
            url::decode_view dk(v.key);
            url::decode_view dv(v.value);

            if (dk == "ttl")
            {
                std::string strTTL = std::string(dv.begin(), dv.end());
                ttl = (uint8_t)std::stoi(strTTL);
            }
            else if (dk == "localaddr")
            {
                std::string strVal(dv.begin(), dv.end());
                ifaceaddr = strVal;
            }
        }
    }

    _udpStreamBuf._pimpl->initSocket(ipaddr.c_str(), port, ttl, ifaceaddr.c_str());
}

UdpOStream::~UdpOStream()
{

}

UdpStreamBuf::~UdpStreamBuf()
{

}

int UdpStreamBuf::overflow(int c)
{
    if (c != EOF) 
    {
        _pimpl->_label.push_back(static_cast<char>(c));

        _pimpl->_prev = _pimpl->_cur;
        _pimpl->_cur = c;

        if (_pimpl->_prev == 60 && _pimpl->_cur == 63 && _pimpl->_label.size() > 2)
        {
            std::string xml = _pimpl->_label.substr(0, _pimpl->_label.size() - 2);
            _pimpl->send(xml.c_str(), xml.size());
            _pimpl->_label.clear();
            _pimpl->_label.push_back(static_cast<char>(_pimpl->_prev));
            _pimpl->_label.push_back(static_cast<char>(_pimpl->_cur));
        }
    }
    return c;
}
