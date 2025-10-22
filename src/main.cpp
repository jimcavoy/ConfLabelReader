// ConfLabelReader.cpp : Defines the entry point for the application.
//

#include "CmdLineParser.h"
#include "UdpOStream.h"
#include <cstdint>
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
#include <brotli/decode.h>
#include <boost/url.hpp>

// Forward declarations
std::string decompressExiLabel(const BYTE* label, size_t len);
bool canStop(int num, int limit);
std::string getFilename(std::string& path);
std::shared_ptr<std::istream> createInput(std::string filepath);
std::shared_ptr<std::ostream> createOutput(std::string filepath);
void Banner();
std::string decompressBrotliLabel(const uint8_t* input, size_t input_size);
bool hasBOM(const std::string& xmlText);

// main
int main(int argc, char* argv[])
{
    using namespace std;
    const int tsPacketSize = 188;  // MPEG-2 TS packet size
    const int bufsize = tsPacketSize * 49; // Read multiple TS packets
    BYTE buffer[bufsize]{};
    ThetaStream::LabelDemux demux;
    int packetsRead = 0;
    int labelsRead = 0;

    Banner();

    CmdLineParser cmdline;
    if (cmdline.parse(argc, argv) != 0)
    {
        return -1;
    }

    try
    {
        shared_ptr<istream> input = createInput(cmdline.input());
        shared_ptr<ostream> output = createOutput(cmdline.output());

        while (input->good())
        {
            input->read((char*)buffer, bufsize);

            std::streamsize len = input->gcount();
            packetsRead += (int)len / tsPacketSize;

            demux.parse(buffer, (UINT32)len);

            // Defined a lambda function to handle events when the LabelDemux encounters a label
            // in the transport stream.
            demux.setLabelCallback([&](std::string encoding, const BYTE* label, size_t len)
                {
                    if (len == 0 || label == 0)
                        return;

                    std::string xml;
                    if (encoding == "$EXI")
                    {
                        xml = decompressExiLabel(label, len);
                    }
                    else if (encoding == "$XML")
                    {
                        std::copy(label, label + len, std::back_inserter(xml));
                    }
                    else if (encoding == "$BRL")
                    {
                        xml = decompressBrotliLabel(label, len);
                    }
                    else
                    {
                        *output << "UNKNOWN" << endl;
                    }

                    if (!xml.empty())
                    {
                        int offset = hasBOM(xml) ? 3 : 0; // Do not output BOM
                        *output << xml.c_str() + offset << endl;
                        labelsRead++;
                    }
                }
            );

            if (demux.hasLabelStream())
            {
                if (canStop(labelsRead, cmdline.number()))
                {
                    break;
                }
            }
            else
            {
                std::cout << "No label in motion imagery file, " << getFilename(cmdline.input()) << std::endl;
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

std::string decompressExiLabel(const BYTE* label, size_t length)
{
    ThetaStream::XmlWriter decoder;
    const char* xmldoc = decoder.decode((char*)label, length);
    return std::string(xmldoc);
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
    if (filepath == "-")
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

std::shared_ptr<std::ostream> createOutput(std::string filepath)
{
    using namespace boost;
    namespace url = boost::urls;
    std::string fname;

    system::result<url_view> res = url::parse_uri(filepath);
    if (!res.has_value())
    {
        char szErr[255]{};
        sprintf(szErr, "Malformed URL, %s", filepath.c_str());
        throw std::runtime_error(szErr);
    }

    url::url_view urlView = res.value();
    if (urlView.scheme() == "file")
    {
#ifdef _WIN32
        fname = urlView.encoded_path().data() + 1;
#else
        fname = urlView.encoded_path().data();
#endif
    }
    else if (urlView.scheme() == "udp")
    {
        fname = filepath;
    }
    else
    {
        throw std::runtime_error("Only support file or udp scheme.");
    }
    std::shared_ptr<std::ostream> output;

    if (fname.empty())
    {
        output.reset(&std::cout, [](...) {});
    }
    else if (urlView.scheme() == "udp")
    {
        UdpOStream* udpStrm = new UdpOStream(fname);
        output.reset(udpStrm);
    }
    else // read the file
    {
        std::ofstream* tsfile = new std::ofstream(fname);
        if (!tsfile->is_open())
        {
            std::string err("Fail to open file: ");
            err += getFilename(fname);
            throw std::ios_base::failure(err);
        }
        output.reset(tsfile);
    }
    return output;
}

void Banner()
{
    std::cerr << "ConfLabelReader: Confidentiality Label Reader Application v2.0.0" << std::endl;
    std::cerr << "Copyright (c) 2025 ThetaStream Consulting, jimcavoy@thetastream.com" << std::endl << std::endl;
}

std::string decompressBrotliLabel(const uint8_t* input, size_t input_size)
{
    std::string label;
    size_t output_size = input_size * 3;
    uint8_t* output = new uint8_t[output_size];

    // Decompress the data
    BrotliDecoderResult result = BrotliDecoderDecompress(
        input_size,          // Input size
        input,               // Input data
        &output_size,        // Output size (updated by the function)
        output               // Output buffer
    );

    // Check the result
    if (result == BROTLI_DECODER_RESULT_SUCCESS)
    {
        const char* str = (char*)output;
        label.assign(str, str + output_size);
        label.push_back(0);
    }

    delete[] output;
    return label;
}

bool hasBOM(const std::string& xmlText)
{
    return xmlText[0] == (char)0xef && xmlText[1] == (char)0xbb && xmlText[2] == (char)0xbf;
}
