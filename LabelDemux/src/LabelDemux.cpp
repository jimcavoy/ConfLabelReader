#ifdef WIN32
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <LabelDemux/pch.h>
#include <LabelDemux/LabelDemux.h>

#include "LabelDemuxImpl.h"
#include <string.h>

namespace ThetaStream
{
    class LabelDemux::Impl
    {
    public:
        Impl()
        {
            _pimpl = std::make_unique<LabelDemuxImpl>();
        }

        ~Impl()
        {
            delete[] _label;
        }

        void parse(const BYTE* stream, UINT32 len)
        {
            delete[] _label;
            _label = nullptr;
            _length = 0;
            _pimpl->parse(stream, len);

            const AccessUnit& au = _pimpl->label();
            if (au.length() > 0)
            {
                _label = new BYTE[au.length()];
                _length = au.length();
                std::copy(au.begin(), au.end(), _label);
            }
        }

        bool hasLabelStream() const
        {
            return _pimpl->hasLabel();
        }

        const BYTE* label() const
        {
            return _label;
        }

        UINT32 labelSize() const
        {
            return _length;
        }

        void setCallback(ThetaStream::OnLabel cb)
        {
            _pimpl->setCallback(cb);
        }

    private:
        std::unique_ptr<LabelDemuxImpl> _pimpl;
        BYTE* _label{};
        UINT32 _length{};
    };


    LabelDemux::LabelDemux()
    {
        _pimpl = std::make_unique<ThetaStream::LabelDemux::Impl>();
    }

    LabelDemux::~LabelDemux()
    {

    }

    void LabelDemux::parse(const BYTE* stream, UINT32 len)
    {
        _pimpl->parse(stream, len);
    }

    bool LabelDemux::hasLabelStream() const
    {
        return _pimpl->hasLabelStream();
    }

    const BYTE* LabelDemux::label() const
    {
        return _pimpl->label();
    }
    UINT32 LabelDemux::labelSize() const
    {
        return _pimpl->labelSize();
    }
    void LabelDemux::setLabelCallback(OnLabel cb)
    {
        _pimpl->setCallback(cb);
    }
}
