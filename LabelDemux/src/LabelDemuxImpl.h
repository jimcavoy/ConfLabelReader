#pragma once
#include "Pid2TypeMap.h"
#include "AccessUnit.h"

#include <mp2tp/libmp2tp.h>

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

private:
    void processStartPayload(const lcss::TransportPacket& pckt);
    void processPayload(const lcss::TransportPacket& pckt);

private:
    lcss::ProgramAssociationTable _pat;
    lcss::ProgramMapTable _pmt;
    Pid2TypeMap _pmtHelper;
    AccessUnit _labelAccessUnit;
    AccessUnit _label;
    bool _hasLabel;
};

