# ConfLabelReader
The ConfLabelReader project is a collection of libraries and a tool to extract 
STANAG 4774/4778 Confidentiality Metadata Labels from a STANAG 4609 Motion
Imagery stream (MPEG-2 Transport Stream).

The project is composed of multiple sub-projects, which are as follows:

* __LabelDemux__: A library that demultiplexes the MPEG-2 transport stream, and 
extracts the label.  The label is EXI encoded, and has to be transcoded to XML
if it needs to be human readable.  The `exi2xml` library can be used to transcode
the label to XML.  `LabelDemux` library has an external dependency on 
[mp2tp library](https://github.com/jimcavoy/mp2tp), which is needed to be build and install.

* __exi2xml__: A library to transcode EXI encoded labels to XML text format.  The library is
generic, and can decode EXI labels based on different XML schemas.  This library has 
an external dependency on [EXIP library](https://github.com/rwl/exip).  See Readme.md in the 
exipCMake folder how to build and install [exip] library.

* __ConfLabelReader__: A tool that reads a MPEG-2 transport stream from a file or standard input stream (stdin), and
prints Confidentiality Metadata Label in XML to a file or standard output stream (stdout).  The tool  
depends on `exi2xml` and `LabelDemux` libraries.

The project is cross-platform, and can be build and run on both Windows and
Linux platforms.
