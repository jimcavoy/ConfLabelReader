#pragma once

#include <iostream>

namespace ThetaStream
{
	class XmlWriter
	{
	public:
		XmlWriter(std::ostream& file);
		~XmlWriter();

		/// <summary>
		/// Decode EXI stream to XML
		/// </summary>
		/// <param name="buffer">Binary encoded EXI stream</param>
		/// <param name="len">The lenght of the buffer</param>
		/// <returns>Returns 0 if successful; otherwise, return non-zero for errors.</returns>
		int decode(char* buffer, unsigned int len);

	private:
		std::ostream& _ofile;
	};
}

