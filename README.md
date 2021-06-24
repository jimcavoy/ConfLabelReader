# ConfLabelReader
STANAG 4774/4778 Confidentiality Metadata Label reader project.  The purpose of
the reader is to extract Confidentiality Metadata Labels from a STANAG 4609
Motion Imagery stream.

The reader is composed of mutiple projects which are as follows:

* __LabelDemux__: A library that demultiplexes the MPEG-2 transport stream, and 
extracts the label.  The label is EXI encoded, and has to be transcoded to XML
if it needs to be human readable.  The `exi2xml` library can be used to transcode
the label to XML.  The library has an external dependency on 
[libmp2t library](https://github.com/jimcavoy/mp2tp).

* __exi2xml__: A library to transcode EXI encoded labels to XML.  The library is
generic, and decode EXI labels based on different XML schemas.  The library has 
an external dependency on [EXIP library](https://github.com/rwl/exip).

* __ConfLabelReader__: A test application that reads MPEG-2 transport stream, and
prints XML label to standard console out.

The libraries and application can be build, and run both on Windows and Linux.
