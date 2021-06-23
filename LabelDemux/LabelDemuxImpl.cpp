#include "pch.h"
#include "LabelDemuxImpl.h"

#include "XmlWriter.h"

#include <sstream>
/////////////////////////////////////////////////////////////////////////////
namespace
{
	std::string printConfidentialityLabelXml(char* exiData, unsigned long len)
	{
		std::stringstream out;
		XmlWriter decoder(out);
		decoder.decode(exiData, len);
		return out.str();
	}
}

LabelDemuxImpl::LabelDemuxImpl()
	:_hasLabel(true)
{
}

LabelDemuxImpl::~LabelDemuxImpl()
{
}

void LabelDemuxImpl::parse(const BYTE* stream, unsigned long len)
{
	_label.clear();
	TSParser::parse(stream, len);
}

void LabelDemuxImpl::onPacket(lcss::TransportPacket& pckt)
{
	if (pckt.payloadUnitStart())
	{
		processStartPayload(pckt);
	}
	else
	{
		processPayload(pckt);
	}
}

bool LabelDemuxImpl::hasLabel() const
{
	return _hasLabel;
}

std::string LabelDemuxImpl::label() const
{
	return _label;
}

void LabelDemuxImpl::processStartPayload(const lcss::TransportPacket& pckt)
{
	const BYTE* data = pckt.getData();
	if (pckt.PID() == 0 && _pat.size() == 0) // Program Association Table
	{
		_pat.parse(data);
	}
	else if (_pat.find(pckt.PID()) != _pat.end())
	{
		auto it = _pat.find(pckt.PID());
		if (it->second != 0) // program 0 is assigned to Network Information Table
		{
			_pmt.clear();
			_pmt.add(data, pckt.data_byte());
			// True if the PMT fits in one TS packet
			if (_pmt.parse())
			{
				_pmtHelper.update(_pmt);
				_hasLabel = _pmtHelper.hasStreamType(Pid2TypeMap::STREAM_TYPE::$EXI) != 0;
			}
		}
	}
	else
	{
		lcss::PESPacket pes;
		UINT16 bytesParsed = pes.parse(data);
		if (bytesParsed > 0)
		{
			if (_pmtHelper.packetType(pckt.PID()) == Pid2TypeMap::STREAM_TYPE::$EXI)
			{
				if (_exiSample.length() > 0)
				{
					processLabel();
				}
				_exiSample.clear();
				_exiSample.insert((char*)data + bytesParsed, pckt.data_byte() - bytesParsed);
			}
		}
	}
}

void LabelDemuxImpl::processPayload(const lcss::TransportPacket& pckt)
{
	const BYTE* data = pckt.getData();
	// PMT spans across two or more TS packets
	auto it = _pat.find(pckt.PID());
	if (it != _pat.end() && it->second != 0)
	{
		_pmt.add(data, pckt.data_byte());
		if (_pmt.parse())
		{
			_pmtHelper.update(_pmt);
			_hasLabel = _pmtHelper.hasStreamType(Pid2TypeMap::STREAM_TYPE::$EXI) != 0;
		}
	}

	if (_pmtHelper.packetType(pckt.PID()) == Pid2TypeMap::STREAM_TYPE::$EXI)
	{
		_exiSample.insert((char*)data, pckt.data_byte());
	}
}

void LabelDemuxImpl::processLabel()
{
	_label = printConfidentialityLabelXml(_exiSample.data(), _exiSample.length());
}
