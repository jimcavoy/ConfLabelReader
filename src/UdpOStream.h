#pragma once

#include <memory>
#include <ostream>
#include <string>

class UdpOStream;

class UdpStreamBuf : public std::streambuf 
{
public:
    virtual ~UdpStreamBuf();

protected:
    int overflow(int c) override;

public:
    class Impl;

private:
    friend class UdpOStream;
    std::unique_ptr<Impl> _pimpl;
};

class UdpOStream : public std::ostream 
{

public:
    UdpOStream(std::string url);
    ~UdpOStream();

private:
    UdpStreamBuf _udpStreamBuf;
};



