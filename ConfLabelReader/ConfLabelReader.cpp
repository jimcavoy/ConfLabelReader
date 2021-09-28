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

#include "LabelDemux.h"
#include "XmlWriter.h"


// Forward declarations
void printLabel(std::ostream& ostrm, const BYTE* label, UINT32 len);
bool canStop(int num, int limit);
std::string getFilename(std::string& path);

// main
int main(int argc, char* argv[])
{
	using namespace std;
	const int tsPacketSize = 188;  // MPEG-2 TS packet size
	const int bufsize = tsPacketSize * 49; // Read multiple TS packets
	std::string ifile;
	std::string ofile;
	BYTE buffer[bufsize];
	ThetaStream::LabelDemux demux;
	int packetsRead = 0;
	int labelsRead = 0;
	char c;
	int limit = 0;

	const char* usage = "Usage: ConfLabelReder -i<MPEG_transport_stream_file> -n<Count> -o<Output_file>";
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
			ifile = *argv + 1;
			break;
		case 'o':
			ofile = *argv + 1;
			break;
		case 'n':
			limit = std::stoi(*argv + 1);
			break;
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

	shared_ptr<istream> input;
	// read file from stdin
	if (ifile.empty())
	{
#ifdef _WIN32
		_setmode(_fileno(stdin), _O_BINARY);
#else
		freopen(NULL, "rb", stdin);
#endif
		input.reset(&cin, [](...) {});
	}
	else // read the file
	{
		ifstream* tsfile = new ifstream(ifile, std::ios::binary);
		if (!tsfile->is_open())
		{
			cout << "Error: Fail to open input file, " << getFilename(ifile) << endl;
			return -1;
		}
		input.reset(tsfile);
	}

	ofstream oStream(ofile);

	while (input->good())
	{
		input->read((char*)buffer, bufsize);

		std::streamsize len = input->gcount();
		packetsRead += (int)len / tsPacketSize;

		demux.parse(buffer, (UINT32) len);
		if (demux.hasLabelStream())
		{
			// Label can be null because the buffer had no label
			if (demux.label() != nullptr)
			{
				if (oStream.is_open())
				{
					printLabel(oStream, demux.label(), demux.labelSize());
				}
				else
				{
					printLabel(cout, demux.label(), demux.labelSize());
				}
				labelsRead++;
			}

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

	std::cout << "Labels read: " << labelsRead << std::endl;
	std::cout << "TS Packets read: " << packetsRead << std::endl;
	return 0;
}

void printLabel(std::ostream& ostrm, const BYTE* label, UINT32 length)
{
	std::stringstream out;
	ThetaStream::XmlWriter decoder(out);
	decoder.decode((char*)label, length);
	ostrm << out.str();
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