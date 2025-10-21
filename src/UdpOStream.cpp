#include "UdpOStream.h"
#include <iostream>

UdpOStream::UdpOStream(std::string url)
    : std::ostream(&_udpStreamBuf)
{
}

UdpOStream::~UdpOStream()
{

}

UdpStreamBuf::~UdpStreamBuf()
{
    std::cout << _label;
}

int UdpStreamBuf::overflow(int c)
{
    if (c != EOF) 
    {
        _label.push_back(static_cast<char>(c));

        _prev = _cur;
        _cur = c;

        if (_prev == 60 && _cur == 63 && _label.size() > 2)
        {
            std::cout << _label.substr(0, _label.size() - 2);
            _label.clear();
            _label.push_back(static_cast<char>(_prev));
            _label.push_back(static_cast<char>(_cur));
        }
    }
    return c;
}
