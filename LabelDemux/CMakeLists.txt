cmake_minimum_required (VERSION 3.21)
project(LabelDemux
    VERSION 1.0.0
    DESCRIPTION "Confidentiality Metadata Label Demuxer Library"
    LANGUAGES C CXX
)

# set the postfix "d" for the resulting .so or .dll files when building the
# library in debug mode
set(CMAKE_DEBUG_POSTFIX
    d
)

find_package(mp2tp 1 CONFIG REQUIRED)

add_library(LabelDemux SHARED)

# set properties for the target. VERSION set the library version to the project
# version * SOVERSION set the compatibility  version for the library to the
# major number of the version
set_target_properties(
    LabelDemux
    PROPERTIES VERSION ${PROJECT_VERSION}
               SOVERSION ${PROJECT_VERSION_MAJOR}
)

file(GLOB SRC_LIST src/*.cpp)

target_sources(LabelDemux PRIVATE ${SRC_LIST})

# define the C++ standard needed to compile this library and make it visible to
# dependers
target_compile_features(
    LabelDemux
    PUBLIC cxx_std_14
)

if (WIN32)
    set(WS wsock32 ws2_32)
endif()

target_link_libraries(LabelDemux PRIVATE lcss::mp2tp ${WS})

# set the include directories
target_include_directories(
    LabelDemux
    PRIVATE src/LabelDemux
    PUBLIC include
)

# if using limited visibility, set CXX_VISIBILILTY_PRESET to "hidden"
include(GenerateExportHeader)
set_property(
    TARGET LabelDemux
    PROPERTY CXX_VISIBILITY_PRESET "hidden"
)
# Hide in-lined functions by default, reducing the size of the library
set_property(
    TARGET LabelDemux
    PROPERTY VISIBILITY_INLINES_HIDDEN TRUE
)
# this command generates a header file in the CMAKE_CURRENT_BINARY_DIR which
# sets the visibility attributes according to the compiler settings
generate_export_header(
    LabelDemux
    EXPORT_FILE_NAME
    export/LabelDemux/export_labeldemux.hpp
)
# Add CMAKE_CURRENT_BINARY_DIR to the include path so the generated header can
# be found
target_include_directories(
    LabelDemux
    PUBLIC "${CMAKE_CURRENT_BINARY_DIR}/export"
)

install(TARGETS LabelDemux
    LIBRARY DESTINATION bin
)