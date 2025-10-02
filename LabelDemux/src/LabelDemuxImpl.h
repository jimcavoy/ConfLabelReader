#pragma once
#include "Pid2TypeMap.h"
#include "AccessUnit.h"

#include <mp2tp/libmp2tp.h>

#include <functional>

#include <string>

namespace ThetaStream
{
    typedef std::function<void(std::string, const BYTE*, size_t)> OnLabel;
}

class LabelDemuxImpl :
    public lcss::TSParser
{
public:
    LabelDemuxImpl();
    ~LabelDemuxImpl();

    virtual void parse(const BYTE* stream, unsigned long len);

    virtual void onPacket(lcss::TransportPacket& pckt);

    bool hasLabel() const;

    const AccessUnit& label() const;

    Pid2TypeMap::STREAM_TYPE labelEncoding() const;

    void setCallback(ThetaStream::OnLabel cb);

    void executeCallback();

private:
    void processStartPayload(const lcss::TransportPacket& pckt);
    void processPayload(const lcss::TransportPacket& pckt);

private:
    lcss::ProgramAssociationTable _pat;
    lcss::ProgramMapTable _pmt;
    Pid2TypeMap _pmtHelper;
    AccessUnit _labelAccessUnit;
    AccessUnit _label;
    ThetaStream::OnLabel _onLabelCallback;
};

