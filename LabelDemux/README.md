# LabelDemux Library
The LabelDemux library demultiplexes a MPEG-2 transport stream, and extracts Confidentiality Metadata Label.  The extract label will require to be decoded so it is human readable.  The exi2xml library in this project can transcode the
exi label to xml.

## Build
This library is depended on libmp2t library which can be cloned from here:

https://github.com/jimcavoy/mp2tp

Build and install libmp2t library outside the ConfLabelReader folder before building LabelDemux.