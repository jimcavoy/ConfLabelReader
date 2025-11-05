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
an external dependency on [EXIP library](https://github.com/jimcavoy/exip).

* __ExiDecoder__: A console application that test the `exi2xml` library.

* __ConfLabelReader__: A tool that reads a MPEG-2 transport stream from a file or standard input stream (stdin), and
prints a Confidentiality Metadata Label in XML to a file or standard output stream (stdout).  The tool  
depends on `exi2xml` and `LabelDemux` libraries.

The project is cross-platform and can be built and run on both Windows and
Linux platforms.

## How to Build
This project uses CMake to generate build system, build, and install the application, __ConfLabelReader__. Do the following steps:

### 1. Prerequisites

__ConfLabelReader__ depends on external libraries for its implementation.  You should clone and build these external projects outside this project.

#### 1.a MPEG-2 TS Library
Build and install [mp2tp library](https://github.com/jimcavoy/mp2tp).  See the project's README which describes how to build and install the library.

#### 1.b EXIP Library
To build and install the EXIP library do the following steps:

__Step a.__ Clone the EXIP project.
```
git clone https://github.com/jimcavoy/exip.git
```

Change directory to the `exip` folder.
```
cd exip
```

__Step b.__ Generate a build toolchain.

```
cmake --preset=<windows-base|linux-base>
```

Set the `--preset` option to `windows-base` if you are on a Windows host machine; otherwise, `linux-base` if you are on a linux host machine.

__Step c.__ Build the library.
```
cmake --build ./out
```

__Step d.__ Install the EXIP library.

On Windows, open a console window as a Administrator.
```
cmake --install ./out
```

On Linux, enter the following in the terminal.
```
sudo cmake --install ./out
```

#### 1.c Install vcpkg

The __ConfLabelReader__ project depends on other third party libraries.  The project uses a [vcpkg](https://vcpkg.io/en/) package manager to download these dependencies.
Ensure that __vcpkg__ is install on your development machine and the environment variable, `VCPKG_ROOT`, is set the file path where __vcpkg__ is installed.

### 2. Generate a Build System
You will need to generate a toolchain to build the __ConfLabelReader__.  At the root of the project directory enter the following on the terminal:

On Windows
```
cmake --preset=windows-base
```
On Linux 
```
cmake --preset=linux-base
```
### 3. Build the Application
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
ConfLabelReader: Confidentiality Label Reader Application v2.0.0
Copyright (c) 2025 ThetaStream Consulting, jimcavoy@thetastream.com

Allowed options:
  -? [ --help ]              Produce help message.
  -i [ --input ] arg         Input Motion Imagery stream or file. (default: -).
  -n [ --numberOfReads ] arg The minimum number of labels to read from the
                             input Motion Imagery stream before exiting. Set to
                             zero to read all. (default: 0).
  -o [ --output ] arg        Output URL for the Motion Imagery stream.
                             (default: file://-).
```

If the `--output` option URL's __scheme__ is `udp`, it can have an optional query component with the following attribute-value pairs :

- __ttl__. The time-to-live parameter.
- __localaddr__. Transmit on a network adapter with an assigned IP address.

For example, you want to stream using UDP on a multicast address of 239.3.1.11 with a time-to-live of 16 and transmit
onto a network adapter with an assigned IP address of 192.168.0.24:

```
--output=udp://239.3.1.11:50000?ttl=16&localaddr=192.168.0.24
```

__Note__: Presently, __ConfLabelReader__ only supports UDP protocol.
## Examples

### 1. Read a File
Read the input MPEG-2 TS file `somefile.ts` and print the first Confidentiality Metadata Label to the file `labels.xml` and exit.
If the first option is the input file, the `-i|--input` option flag is optional.

```
ConfLabelReader C:\Samples\somefile.ts -n 1 -o file:///C:/Output/labels.xml
```

### 2. Output All Labels
Set the `-n` option to zero to print all the labels out to a the file `labels.xml`.  By default, the `-n` option value is 0; therefore, the `-n` option is optional. 

```
ConfLabelReader -i ~/Samples/somefile.ts -n 0 -o file://~/Output/labels.xml
```

### 3. Using Pipes
Using [IReflxApp](https://github.com/jimcavoy/IReflx) to pipe a live MPEG-2 TS stream being transmitted on a multicast address into __ConfLabelReader__ where it then pipes the output labels to a file, `labels.xml`.

```
IReflxApp -s 239.3.1.11:50000 | ConfLabelReader > labels.xml
```

### 4. Transmit Labels over UDP
In this example, using [IReflxApp](https://github.com/jimcavoy/IReflx) to pipe a live MPEG-2 TS stream being transmitted on a multicast address into __ConfLabelReader__ where it then pipes the output labels over UDP.

```
IReflxApp -s 239.3.1.11:50000 | ConfLabelReader -o udp://239.3.3.1:50000?ttl=16
```
