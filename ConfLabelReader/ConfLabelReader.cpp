// ConfLabelReader.cpp : Defines the entry point for the application.
//

#include <iostream>
#include <fstream>
#include <tchar.h>

typedef unsigned char BYTE;
#include "LabelReader.h"

int main(int argc, char* argv[])
{
	const int tsPacketSize = 188;  // MPEG-2 TS packet size
	const int bufsize = tsPacketSize * 49; // Read multiple TS packets
	std::basic_ifstream<BYTE> ifile;
	BYTE buffer[bufsize];
	ThetaStream::LabelReader reader;
	int packetsRead = 0;
	int labelsRead = 0;

	if (argc != 2)
	{
		std::cerr << "usage: LabelReader filename" << std::endl;
		return -1;
	}

	ifile.open(argv[1], std::ios::in | std::ios::binary);
	if (!ifile.is_open())
	{
		std::cerr << "error: failed to open file, " << argv[1] << std::endl;
		return -1;
	}

	while (ifile.good())
	{
		ifile.read(buffer, bufsize);

		std::streamsize len = ifile.gcount();
		packetsRead += (int)len / tsPacketSize;

		reader.parse(buffer, len);
		if (reader.hasLabelStream())
		{
			// Label can be null because the buffer had no label
			if (reader.label() != nullptr)
			{
				std::cout << reader.label();
				labelsRead++;
			}
		}
		else
		{
			std::cout << "No label in motion imagery file, " << argv[1] << std::endl;
			break;
		}
	}

	std::cerr << "Labels read: " << labelsRead << std::endl;
	std::cerr << "TS Packets read: " << packetsRead << std::endl;

	return 0;
}