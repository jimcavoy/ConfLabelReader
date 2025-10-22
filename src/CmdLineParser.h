#pragma once
#include <string>
#include <memory>


class CmdLineParser
{
public:
    CmdLineParser();
    ~CmdLineParser();

    CmdLineParser(const CmdLineParser& src);
    CmdLineParser& operator=(const CmdLineParser& rhs);

    CmdLineParser(CmdLineParser&& src) noexcept;
    CmdLineParser& operator=(CmdLineParser&& rhs) noexcept;


    int parse(int argc, char* argv[]);

    std::string input() const;
    std::string output() const;
    int number() const;

private:
    class Impl;
    std::shared_ptr<Impl> _pimpl;
};

