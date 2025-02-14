cmake_minimum_required (VERSION 3.21)

project(exi2xml
	VERSION 0.2.0
	DESCRIPTION "EXI to XML writer library"
	LANGUAGES C CXX)

include(GNUInstallDirs)

# set the postfix "d" for the resulting .so or .dll files when building the
# library in debug mode
set(CMAKE_DEBUG_POSTFIX d )

find_package(exip CONFIG REQUIRED)

add_library(exi2xml STATIC)

target_sources(exi2xml
  PRIVATE 
    src/XmlWriter.cpp
    include/exi2xml/XmlWriter.h)

# define the C++ standard needed to compile this library and make it visible to
# dependers
target_compile_features(
    exi2xml
    PUBLIC cxx_std_14
)

target_link_libraries(exi2xml PRIVATE exip)

target_include_directories(exi2xml
    PRIVATE 
      src
      include/exi2xml
    PUBLIC 
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

install(TARGETS exi2xml
    EXPORT exi2xml_export
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install (
    DIRECTORY ${PROJECT_SOURCE_DIR}/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install (EXPORT exi2xml_export
    FILE exi2xml-config.cmake
    NAMESPACE thetastream::
    DESTINATION lib/cmake/exi2xml
)

if (WIN32)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/Debug/exi2xmld.pdb DESTINATION lib OPTIONAL)
endif()

# Defines write_basic_package_version_file
include(CMakePackageConfigHelpers)

# Create a package version file for the package. This will allow consumers to specify
# version argument to select between several versions of the package via specifying VERSION argument
# to find_package().
# We're not explicitly specifying the VERSION parameter to allow `write_basic_package_version_file` to 
# get it from project(...) version (which is 1.0.0)
write_basic_package_version_file(
  "exi2xml-config-version.cmake"
  # Package compatibility strategy. SameMajorVersion is essentially `semantic versioning`.
  COMPATIBILITY SameMajorVersion
)

# Install command for deploying Config-file package files into the target system. It must be present
# in the same directory as `exi2xml-config.cmake` file.
install(FILES
  "${CMAKE_CURRENT_BINARY_DIR}/exi2xml-config-version.cmake"
  DESTINATION lib/cmake/exi2xml
)
