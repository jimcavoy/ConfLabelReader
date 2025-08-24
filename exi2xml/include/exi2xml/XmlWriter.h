#pragma once

#include <string>

namespace ThetaStream
{
    class XmlWriter
    {
    public:
        XmlWriter();
        ~XmlWriter();

        /// <summary>
        /// Decode EXI stream to XML
        /// </summary>
        /// <param name="buffer">Binary encoded EXI stream.</param>
        /// <param name="len">The length in bytes of the EXI stream contained in the buffer parameter.</param>
        /// <returns>Returns decoded XML document; otherwise, NULL if it fails.</returns>
        const char* decode(char* buffer, unsigned int len);

    public:
        std::string _xmldoc;
    };
}

