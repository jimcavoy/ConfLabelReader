# EXIP cmake file
The ConfLabelReader project requires EXIP library [Embeddable EXI Processor in C](http://exip.sourceforge.net/).
This folder contains a CMake file to assist in building the library on Windows and Linux.  

## Build Steps
Steps to build and install the  EXIP library:

### 1. Clone EXIP 

Clone the EXIP project outside ConfLabelReader folder using the following command:
```
git clone https://github.com/rwl/exip.git 
```

### 2. Copy CMakeLists.txt 

Copy the `CMakeLists.txt` file into the EXIP root directory.

### 3 Build and Install EXIP Library

To build and install the EXIP library do the following steps:

**a. Generate the Build System**

On Windows
```
cmake -S . -B ./out -A x64
```

On Linux
```
cmake -S . -B ./out
```

**b. Build**
```
cmake --build ./out  --config <Debug|Release>
```

**c. Install**

On Windows, ensure you are Administrator before running the below command.
```
cmake --install ./out --config <Debug|Release>
```

On Linux
```
sudo cmake --install ./out --config <Debug|Release>
```