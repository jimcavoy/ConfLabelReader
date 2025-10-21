#pragma once
#include <ostream>
#include <string>

class UdpOStream;

class UdpStreamBuf : public std::streambuf 
{
public:
    virtual ~UdpStreamBuf();

protected:
    int overflow(int c) override;

private:
    std::string _label;
    int _prev{ 0 };
    int _cur{ 0 };
};

class UdpOStream : public std::ostream 
{

public:
    UdpOStream(std::string url);
    ~UdpOStream();

private:
    UdpStreamBuf _udpStreamBuf;
};



