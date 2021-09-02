#pragma once
#include "labeldemux_export.h"
#include "LabelDemuxTypes.h"

class LabelDemuxImpl;

/// <summary>
/// LabelDemux reads sequence of bytes from a MPEG-2 Transport Stream and
/// extracts the STANAG 4774 Confidentiality Metadata Label from it if one
/// is available.  The client passes in the MPEG-2 Transport Stream in the 
/// parse() function and polls for the label by calling the label() function
/// after the parse() function returns.
/// </summary>
namespace ThetaStream
{
	class LABELDEMUX_EXPORT LabelDemux
	{
	public:
		LabelDemux();
		~LabelDemux();

		/// <summary>
		/// Parse the input byte stream and extract the Confidentiality Metadata
		/// Label.  Call the label() function after this function returns to 
		/// retrieve the EXI encoded Confidentiality Metadata Label.
		/// </summary>
		/// <param name="transport_stream">An array of bytes containing 
		/// MPEG-2 Transport Stream</param>
		/// <param name="len">The number of bytes of the transport_stream
		/// parameter.  It is recommended the len parameter should a multiple of 
		/// 188 bytes.  For example, len = 188 * 49.</param>
		void parse(const BYTE* transport_stream, UINT32 len);

		/// <summary>
		/// Call this function to determine if the stream passed into the parse()
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

	private:
		LabelDemuxImpl* _pimpl;
		BYTE* _label;
		UINT32 _length;
	};

}
