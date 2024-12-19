#include <LabelDemux/pch.h>
#include "LabelDemuxImpl.h"

#include <sstream>
/////////////////////////////////////////////////////////////////////////////

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

const AccessUnit& LabelDemuxImpl::label() const
{
    return _label;
}

Pid2TypeMap::STREAM_TYPE LabelDemuxImpl::labelEncoding() const
{
    uint16_t pid = _pmtHelper.hasStreamType(Pid2TypeMap::STREAM_TYPE::CONFLABEL_EXI);
    if (pid)
    {
        return Pid2TypeMap::STREAM_TYPE::CONFLABEL_EXI;
    }

    pid = _pmtHelper.hasStreamType(Pid2TypeMap::STREAM_TYPE::CONFLABEL_XML);
    if (pid)
    {
        return Pid2TypeMap::STREAM_TYPE::CONFLABEL_XML;
    }

    return Pid2TypeMap::STREAM_TYPE::UNKNOWN;
}

void LabelDemuxImpl::setCallback(ThetaStream::OnLabel cb)
{
    _onLabelCallback = cb;
}

void LabelDemuxImpl::executeCallback()
{
    if (_onLabelCallback)
    {
        switch (labelEncoding())
        {
        case Pid2TypeMap::STREAM_TYPE::CONFLABEL_EXI:
        {
            _onLabelCallback(std::string{ "$EXI" }, (BYTE*) _label.data(), _label.length());
            break;
        }
        case Pid2TypeMap::STREAM_TYPE::CONFLABEL_XML:
        {
            _onLabelCallback(std::string{ "$XML" }, (BYTE*)_label.data(), _label.length());
            break;
        }
        default:
            _onLabelCallback(std::string{ "UNKNOWN" }, 0, 0);
        }
    }
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
            _pmt = lcss::ProgramMapTable(data, pckt.data_byte());
            // True if the PMT fits in one TS packet
            if (_pmt.parse())
            {
                _pmtHelper.update(_pmt);
                uint16_t pid = _pmtHelper.hasStreamType(Pid2TypeMap::STREAM_TYPE::CONFLABEL_EXI);
                if (pid == 0)
                {
                    pid = _pmtHelper.hasStreamType(Pid2TypeMap::STREAM_TYPE::CONFLABEL_XML);
                }
                _hasLabel = pid == 0 ? false : true;
            }
        }
    }
    else
    {
        lcss::PESPacket pes;
        UINT16 bytesParsed = pes.parse(data);
        if (bytesParsed > 0)
        {
            Pid2TypeMap::STREAM_TYPE st = _pmtHelper.packetType(pckt.PID());
            if (st == Pid2TypeMap::STREAM_TYPE::CONFLABEL_EXI || st == Pid2TypeMap::STREAM_TYPE::CONFLABEL_XML)
            {
                _label = _labelAccessUnit;
                _labelAccessUnit.clear();
                executeCallback();
                _labelAccessUnit.insert((char*)data + bytesParsed, pckt.data_byte() - bytesParsed);
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
            uint16_t pid = _pmtHelper.hasStreamType(Pid2TypeMap::STREAM_TYPE::CONFLABEL_EXI);
            if (pid == 0)
            {
                pid = _pmtHelper.hasStreamType(Pid2TypeMap::STREAM_TYPE::CONFLABEL_XML);
            }
            _hasLabel = pid == 0 ? false : true;
        }
    }

    Pid2TypeMap::STREAM_TYPE st = _pmtHelper.packetType(pckt.PID());
    if (st == Pid2TypeMap::STREAM_TYPE::CONFLABEL_EXI || st == Pid2TypeMap::STREAM_TYPE::CONFLABEL_XML)
    {
        _labelAccessUnit.insert((char*)data, pckt.data_byte());
    }
}


