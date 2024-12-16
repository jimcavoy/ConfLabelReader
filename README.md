# ConfLabelReader
The ConfLabelReader project is a collection of libraries and a tool to extract 
STANAG 4774/4778 Confidentiality Metadata Labels from a STANAG 4609 Motion
Imagery stream (MPEG-2 Transport Stream).

__ConfLabelReader__ extracts and reads the STANAG 4774 Confidentiality Metadata Label based
on [STANAG 4609 Binding Profile](./BindingProfile.md).

The project is composed of multiple sub-projects, which are as follows:

* __LabelDemux__: A library that demultiplexes the MPEG-2 transport stream, and 
extracts the label.  The label is EXI encoded and has to be transcoded to XML
if it needs to be human-readable.  The `exi2xml` library can be used to transcode
the label to XML.  `LabelDemux` library has an external dependency on 
[mp2tp library](https://github.com/jimcavoy/mp2tp), which is needed to be built and installed.

* __exi2xml__: A library to transcode EXI-encoded labels to XML text format.  The library is
generic and can decode EXI labels based on different XML schemas.  This library has
an external dependency on [EXIP library](https://github.com/rwl/exip).  See [Readme.md](./exipCMake/README.md) in the 
/exipCMake folder how to build and install `exip` library.

* __ExiDecoder__: A console application that test the `exi2xml` library.

* __ConfLabelReader__: A tool that reads a MPEG-2 transport stream from a file or standard input stream (stdin), and
prints a Confidentiality Metadata Label in XML to a file or standard output stream (stdout).  The tool  
depends on `exi2xml` and `LabelDemux` libraries.

The project is cross-platform and can be built and run on both Windows and
Linux platforms.

## How to Build
This project uses CMake to generate build system, build, and install the application, __ConfLabelReader__. Do the following steps:

### 1. Prerequisites

__ConfLabelReader__ depends on external libraries for its implementation.  Therefore, before building the application do the following:

a. Build and install [mp2tp library](https://github.com/jimcavoy/mp2tp).

b. Build and install [EXIP library](https://github.com/rwl/exip).  See [README.md](./exipCMake/README.md) in the 
[exipCMake](./exipCMake) folder how to build and install `exip` library.

### 2. Generate Build System
On Windows
```
cmake -S . -B ./build -A x64
```
On Linux 
```
cmake -S . -B ./build
```
### 3. Build Application
```
cmake --build ./build --config <Debug|Release>
```

The `--config` option is either `Debug` for a debug build or `Release` for a release build. `Debug` is the default build configuration if the `--config` option is absent.

### 4. Install the Application

On Windows, you need Administrator role privilages before running the below command.
```
cmake --install ./build --config <Debug|Release>
```

On Linux

```
sudo cmake --install ./build --config <Debug|Release>
```

The `--config` option tells CMake either to install a debug or release build.  `Release` is the default build configuration if the `--config` option is absent.

## To Test
To test __ConfLabelReader__ build, enter the following:

```
ctest --test-dir ./build -C <Debug|Release>
```

The `-C` option specifies the build configuration to test, either `Debug` or `Release` build.

## Usage

```
Usage: ConfLabelReader -i <MPEG_transport_stream_file> -n <Count> -o <Output_file>

Options:
  -i    Input MPEG transport stream file path.
  -n    The minimum number of labels to read from the input file before exiting.
        Set to zero to read all. (default: 0).
  -o    Optional output file name (default: console).
  -?    Print this message.
```

## Examples

### 1. Read a File
Read the input MPEG-2 TS file `somefile.ts` and print the first Confidentiality Metadata Label to the file `labels.xml` and exit.

```
ConfLabelReader -i C:\Samples\somefile.ts -n 1 -o labels.xml
```

### 2. Output All Labels
Set the `-n` option to zero to print all the labels out to a the file `labels.xml`.  By default, the `-n` option value is 0; therefore, the `-n` option is optional. 

```
ConfLabelReader -i C:\Samples\somefile.ts -n 0 -o labels.xml
```

### 3. Using Pipes
Using [IReflxApp](https://github.com/jimcavoy/IReflx) to pipe a live MPEG-2 TS stream being transmitted on a multicast address into __ConfLabelReader__ where it then pipes the output labels to a file, `labels.xml`.

```
IReflxApp -s 239.3.1.11:50000 | ConfLabelReader > labels.xml
```
