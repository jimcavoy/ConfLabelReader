// ConfLabelReader.cpp : Defines the entry point for the application.
//

#include <iostream>
#include <fstream>
#include <sstream>

#include "LabelDemux.h"
#include "XmlWriter.h"

void printLabel(const BYTE* label, UINT32 len);

int main(int argc, char* argv[])
{
	const int tsPacketSize = 188;  // MPEG-2 TS packet size
	const int bufsize = tsPacketSize * 49; // Read multiple TS packets
	std::ifstream ifile;
	BYTE buffer[bufsize];
	ThetaStream::LabelDemux demux;
	int packetsRead = 0;
	int labelsRead = 0;

	if (argc != 2)
	{
		std::cout << "usage: ConfLabelReader <MPEG_transport_stream_file>" << std::endl;
		return -1;
	}

	ifile.open(argv[1], std::ios::binary);
	if (!ifile.is_open())
	{
		std::cout << "error: failed to open file, " << argv[1] << std::endl;
		return -1;
	}

	while (ifile.good())
	{
		ifile.read((char*)buffer, bufsize);

		std::streamsize len = ifile.gcount();
		packetsRead += (int)len / tsPacketSize;

		demux.parse(buffer, (UINT32) len);
		if (demux.hasLabelStream())
		{
			// Label can be null because the buffer had no label
			if (demux.label() != nullptr)
			{
				printLabel(demux.label(), demux.labelSize());
				labelsRead++;
			}
		}
		else
		{
			std::cout << "No label in motion imagery file, " << argv[1] << std::endl;
			break;
		}
	}

	std::cout << "Labels read: " << labelsRead << std::endl;
	std::cout << "TS Packets read: " << packetsRead << std::endl;
	return 0;
}

void printLabel(const BYTE* label, UINT32 length)
{
	std::stringstream out;
	ThetaStream::XmlWriter decoder(out);
	decoder.decode((char*)label, length);
	std::cout << out.str();
}