// ConfLabelReader.cpp : Defines the entry point for the application.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#ifdef _WIN32
#include <io.h>
#endif
#include <fcntl.h>
#include <memory>
#include <string.h>

#include <LabelDemux/LabelDemux.h>
#include <exi2xml/XmlWriter.h>
#include <string.h>

// Forward declarations
void printExiLabel(std::ostream& ostrm, const BYTE* label, size_t len);
bool canStop(int num, int limit);
std::string getFilename(std::string& path);
std::shared_ptr<std::istream> createInput(std::string filepath);
void Banner();

// main
int main(int argc, char* argv[])
{
    using namespace std;
    const int tsPacketSize = 188;  // MPEG-2 TS packet size
    const int bufsize = tsPacketSize * 49; // Read multiple TS packets
    std::string ifile;
    std::string ofile;
    BYTE buffer[bufsize]{};
    ThetaStream::LabelDemux demux;
    int packetsRead = 0;
    int labelsRead = 0;
    char c;
    int limit = 0;

    Banner();

    const char* usage = "\nUsage: ConfLabelReader -i <MPEG_transport_stream_file> -n <Count> -o <Output_file>";
    const char* opts = "  -i\tInput MPEG transport stream file path.\n \
 -n\tThe minimum number of labels to read from the input file before exiting.\n \
   \tSet to zero to read all. (default: 0).\n \
 -o\tOptional output file name (default: console).\n \
 -?\tPrint this message.";

    while (--argc > 0 && (*++argv)[0] == '-')
    {
        c = *++argv[0];
        switch (c)
        {
        case 'i':
        {
            // Check for a space character between the option and value
            if (strlen(*argv + 1) == 0)
            {
                ifile = *++argv;
                --argc;
            }
            else // no space character between the option and value
            {
                ifile = *argv + 1;
            }
            break;
        }
        case 'o':
        {
            // Check for a space character between the option and value
            if (strlen(*argv + 1) == 0)
            {
                ofile = *++argv;
                --argc;
            }
            else // no space character between the option and value
            {
                ofile = *argv + 1;
            }
            break;
        }
        case 'n':
        {
            // Check for a space character between the option and value
            if (strlen(*argv + 1) == 0)
            {
                limit = std::stoi(*++argv);
                --argc;
            }
            else // no space character between the option and value
            {
                limit = std::stoi(*argv + 1);
            }
            break;
        }
        case '?':
            cout << usage << endl;
            cout << endl << "Options: " << endl;
            cout << opts << endl;
            return 0;
            break;
        default:
            cout << "ConfLabelReader: illegal option " << c << endl;
            return -1;
        }
    }

    try
    {
        shared_ptr<istream> input = createInput(ifile);
        ofstream oStream(ofile);

        while (input->good())
        {
            input->read((char*)buffer, bufsize);

            std::streamsize len = input->gcount();
            packetsRead += (int)len / tsPacketSize;

            demux.parse(buffer, (UINT32)len);

            demux.setLabelCallback([&](std::string encoding, const BYTE* label, size_t len)
                {
                    if (len == 0 || label == 0)
                        return;

                    if (encoding == "$EXI")
                    {
                        printExiLabel(oStream, label, len);
                    }
                    else if (encoding == "$XML")
                    {
                        std::string xml;
                        std::copy(label, label + len, std::back_inserter(xml));
                        oStream << xml;
                    }
                    labelsRead++;
                }
            );

            if (demux.hasLabelStream())
            {
                if (canStop(labelsRead, limit))
                {
                    break;
                }
            }
            else
            {
                std::cout << "No label in motion imagery file, " << getFilename(ifile) << std::endl;
                break;
            }
        }
    }
    catch (const std::exception& ex)
    {
        std::cout << "ERROR: " << ex.what() << endl;
        return -1;
    }

    std::cout << "Labels read: " << labelsRead << std::endl;
    std::cout << "TS Packets read: " << packetsRead << std::endl;
    return 0;
}

void printExiLabel(std::ostream& ostrm, const BYTE* label, size_t length)
{
    std::stringstream out;
    ThetaStream::XmlWriter decoder(out);
    decoder.decode((char*)label, length);
    if (ostrm.good())
    {
        ostrm << out.str();
    }
    else
    {
        std::cout << out.str();
    }
}

bool canStop(int num, int limit)
{
    if (limit == 0)
        return false;
    return num >= limit ? true : false;
}

std::string getFilename(std::string& path)
{
    std::string fname;
    std::string::const_reverse_iterator it;
    for (it = path.rbegin(); it != path.rend(); ++it)
    {
        if (*it == '\\' || *it == '/')
            break;
        fname.push_back(*it);
    }
    std::reverse(fname.begin(), fname.end());
    return fname;
}


std::shared_ptr<std::istream> createInput(std::string filepath)
{
    std::shared_ptr<std::istream> input;
    // read file from stdin
    if (filepath.empty())
    {
#ifdef _WIN32
        _setmode(_fileno(stdin), _O_BINARY);
#else
        freopen(NULL, "rb", stdin);
#endif
        input.reset(&std::cin, [](...) {});
    }
    else // read the file
    {
        std::ifstream* tsfile = new std::ifstream(filepath, std::ios::binary);
        if (!tsfile->is_open())
        {
            std::string err("Fail to open file: ");
            err += getFilename(filepath);
            throw std::ios_base::failure(err);
        }
        input.reset(tsfile);
    }
    return input;
}

void Banner()
{
    std::cerr << "ConfLabelReader: Confidentiality Label Reader Application v1.2.0" << std::endl;
    std::cerr << "Copyright (c) 2025 ThetaStream Consulting, jimcavoy@thetastream.com" << std::endl;
}