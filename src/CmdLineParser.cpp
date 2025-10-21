#include "CmdLineParser.h"

#include <iostream>

#include <boost/program_options.hpp>

#ifdef _WIN32
#include <windows.h>
void enableVirtualTerminalProcessing() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    if (hOut != INVALID_HANDLE_VALUE && GetConsoleMode(hOut, &dwMode)) {
        SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    }
}
#endif


// ANSI escape codes for text colors
#define RED "\033[31m"
#define RESET "\033[0m"


class CmdLineParser::Impl
{
public:
    Impl() {}
    ~Impl() {}

    Impl(const Impl& src);

    std::string _input{ "file://-" };
    std::string _output{ "file://-" };
    int _number{ 0 };
};

const char* OTHERHELP = "";

CmdLineParser::Impl::Impl(const Impl& src)
    : _input(src._input)
    , _output(src._output)
    , _number(src._number)
{
}

CmdLineParser::CmdLineParser()
{
    _pimpl = std::make_shared<CmdLineParser::Impl>();
}


CmdLineParser::~CmdLineParser()
{
}

CmdLineParser::CmdLineParser(const CmdLineParser& src)
    : _pimpl(std::make_unique<CmdLineParser::Impl>(*src._pimpl))
{
}

CmdLineParser& CmdLineParser::operator=(const CmdLineParser& rhs)
{
    if (this != &rhs)
    {
        _pimpl.reset(new CmdLineParser::Impl(*rhs._pimpl));
    }
    return *this;
}

CmdLineParser::CmdLineParser(CmdLineParser&& src) noexcept
{
    *this = std::move(src);
}

CmdLineParser& CmdLineParser::operator=(CmdLineParser&& rhs) noexcept
{
    if (this == &rhs)
    {
        _pimpl = std::move(rhs._pimpl);
    }
    return *this;
}

int CmdLineParser::parse(int argc, char* argv[])
{
    using namespace boost;
    using namespace std;
    namespace po = boost::program_options;

    try
    {
        po::options_description desc("Allowed options");

        desc.add_options()
            ("help,?", "Produce help message.")
            ("input,i", po::value<string>(&_pimpl->_input), "Input URL for the Motion Imagery stream. (default: file://-).")
            ("numberOfReads,n", po::value<int>(&_pimpl->_number), "The minimum number of labels to read from the input Motion Imagery stream before exiting. Set to zero to read all. (default: 0).")
            ("output,o", po::value<string>(&_pimpl->_output), "Output URL for the Motion Imagery stream. (default: file://-).")
            ;

        po::command_line_parser parser{ argc, argv };
        parser.options(desc);
        po::parsed_options poptions = parser.run();

        po::variables_map vm;
        po::store(poptions, vm);
        po::notify(vm);

        if (vm.count("help"))
        {
            cerr << desc << endl;
            return -1;
        }

#ifdef _WIN32
        enableVirtualTerminalProcessing();
#endif

    }
    catch (const std::exception& ex)
    {
          cerr << ex.what() << endl;
        return -1;
    }

    return 0;
}

std::string CmdLineParser::input() const
{
    return _pimpl->_input;
}

std::string CmdLineParser::output() const
{
    return _pimpl->_output;
}

int CmdLineParser::number() const
{
    return _pimpl->_number;
}
