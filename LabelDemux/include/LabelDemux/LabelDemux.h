#pragma once
#include <LabelDemux/export_labeldemux.hpp>
#include <LabelDemux/LabelDemuxTypes.h>
#include <memory>
#include <functional>
#include <string>

class LabelDemuxImpl;

/// <summary>
/// LabelDemux reads sequence of bytes from a MPEG-2 Transport Stream and
/// extracts the STANAG 4774 Confidentiality Metadata Label from it if one
/// is available.  To retrieve the label define a callback function based 
/// on the OnLabel() declaration and pass it in to the LabelDemux instance by calling 
/// setLabelCallback().  When the LabelDemux instance encounters a label
/// in the transport stream, it will invoke the OnLabel callback function
/// where the client code can handle processing the label.
/// </summary>
namespace ThetaStream
{
    /// <summary>
    /// The client defines a function passed on this signature, OnLabel, and is passed
    /// into LabelDemux::setLabelCallback() member function.  When LabelDemux
    /// encounters a label in the transport stream, it calls this function
    /// to allow the client code to handle this event.
    ///</summary>
    ///<param name="encoding">How the label is encoded.  If the value set to "$EXI",
    /// the label is EXI encoded and if it is set to "$XML" is XML text-formated; otherwise,
    /// "UNKNOWN" if the label encoding can not be determined.</param>
    /// <param name="label">The label.</param>
    /// <param name="len">The length of the label in bytes.</param>
    typedef std::function<void(std::string encoding, const BYTE* label, size_t len)> OnLabel;

    class LABELDEMUX_EXPORT LabelDemux
    {
    public:
        LabelDemux();
        ~LabelDemux();

        /// <summary>
        /// Parse the input byte stream and extract the Confidentiality Metadata
        /// Label.  If the client defines and sets a callback function based on the
        /// OnLabel declaration, this instance will call OnLabel function every time it
        /// encounters a label in the transport stream.
        /// </summary>
        /// <param name="transport_stream">An array of bytes containing 
        /// MPEG-2 Transport Stream</param>
        /// <param name="len">The number of bytes of the transport_stream
        /// parameter.  It is recommended the len parameter should a multiple of 
        /// 188 bytes.  For example, len = 188 * 49.</param>
        void parse(const BYTE* transport_stream, UINT32 len);

        /// <summary>
        /// Call this function to determine if the transport stream passed into the parse()
        /// function contains a Confidentiality Metadata Label program element.
        /// </summary>
        /// <returns>Returns true if the transport stream passed into the parse()
        /// function contains a Confidentiality Metadata Label program
        /// element; otherwise, false.</returns>
        bool hasLabelStream() const;

        /// <summary>
        /// Returns an EXI encoded Confidentiality Metadata Label from the last parse()
        /// invocation.  Can be null if no label was found in the last parse() call.
        /// </summary>
        /// <returns>Returns EXI encoded label; otherwise, null.</returns>
        const BYTE* label() const;

        /// <summary>
        /// Returns the size of the label in bytes. 
        /// </summary>
        /// <returns></returns>
        UINT32 labelSize() const;

        /// <summary>
        /// Pass in a function that implements the OnLabel declaration that will handle
        /// events when this instance encounters a label in the transport stream.
        /// </summary>
        /// <param name="cb">The client defined OnLabel function that this instance
        /// will call when it encounters a label in the transport stream.</param>
        void setLabelCallback(OnLabel cb);

    private:
        class Impl;
        std::unique_ptr<Impl> _pimpl;
    };

}
