# EXIP cmake file
The ConfLabelReader project requires EXIP library [Embeddable EXI Processor in C](http://exip.sourceforge.net/).
This folder contains a CMake file to assist in building the library on Windows and Linux.  

Steps to build EXIP library:

1. git clone https://github.com/rwl/exip.git outside ConfLabelReader folder.

2. Copy CMakeLists.txt into the exip root directory.

3. Use cmake to build and install the [exip] library.

We only tested this procedure when running Visual Studio 2022 on Windows to 
build EXIP library on both Windows and Linux.