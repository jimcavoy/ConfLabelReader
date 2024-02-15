cmake_minimum_required (VERSION 3.21)

project(exi2xml
	VERSION 0.2.0
	DESCRIPTION "EXI to XML writer library"
	LANGUAGES C CXX)

find_package(exip CONFIG REQUIRED)

add_library(exi2xml STATIC)

target_sources(exi2xml PRIVATE src/XmlWriter.cpp)

# define the C++ standard needed to compile this library and make it visible to
# dependers
target_compile_features(
    LabelDemux
    PUBLIC cxx_std_14
)

target_link_libraries(exi2xml PRIVATE exip)

target_include_directories(
    exi2xml
    PRIVATE src/exi2xml
    PUBLIC include
)
