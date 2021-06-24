# EXIP cmake file
The ConfLabelReader project requires EXIP library (Embeddable EXI Processor in C), http://exip.sourceforge.net/.

Steps to build EXIP library:

1. git clone https://github.com/rwl/exip.git outside ConfLabelReader folder.

2. Copy CMakeLists.txt into the exip root directory.

3. Use cmake to build and install the exip library.

Only tested when using Visual Studio 2019 to build on Windows and Linux.