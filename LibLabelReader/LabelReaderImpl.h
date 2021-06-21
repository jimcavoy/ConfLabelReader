#pragma once
#include "Pid2TypeMap.h"
#include "AccessUnit.h"

#include "libmp2t/tsprsr.h"
#include "libmp2t/tspat.h"
#include "libmp2t/tspmt.h"

#include <string>

class LabelReaderImpl :
	public lcss::TSParser
{
public:
	LabelReaderImpl();
	~LabelReaderImpl();

	virtual void parse(const BYTE* stream, size_t len);

	virtual void onPacket(lcss::TransportPacket& pckt);

	bool hasLabel() const;

	std::string label() const;

private:
	void processStartPayload(const lcss::TransportPacket& pckt);
	void processPayload(const lcss::TransportPacket& pckt);
	void processLabel();

private:
	lcss::ProgramAssociationTable _pat;
	lcss::ProgramMapTable _pmt;
	Pid2TypeMap _pmtHelper;
	AccessUnit _exiSample;
	bool _hasLabel;
	std::string _label;
};

