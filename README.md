# ConfLabelReader
The ConfLabelReader project is a collection of libraries and a tool to extract 
STANAG 4774/4778 Confidentiality Metadata Labels from a STANAG 4609 Motion
Imagery stream.

The project is composed of mutiple sub-projects which are as follows:

* __LabelDemux__: A library that demultiplexes the MPEG-2 transport stream, and 
extracts the label.  The label is EXI encoded, and has to be transcoded to XML
if it needs to be human readable.  The `exi2xml` library can be used to transcode
the label to XML.  This library has an external dependency on 
[libmp2t library](https://github.com/jimcavoy/mp2tp).

* __exi2xml__: A library to transcode EXI encoded labels to XML text format.  The library is
generic, and can decode EXI labels based on different XML schemas.  This library has 
an external dependency on [EXIP library](https://github.com/rwl/exip).

* __ConfLabelReader__: A tool that reads a MPEG-2 transport stream file, and
prints Confidentiality Metadata Label in XML to standard console out.  The tool is 
depended on `exi2xml` and `LabelDemux` libraries.

The project is cross-platform, and can be build and run on both Windows and
Linux platforms.
