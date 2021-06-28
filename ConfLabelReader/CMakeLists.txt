# CMakeList.txt : CMake project for ConfLabelReader, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)
project(ConfLabelReader)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if (WIN32)
    set(CMAKE_INSTALL_LIBDIR $ENV{APPDATA})
else()
    set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
endif()

include(${CMAKE_INSTALL_LIBDIR}/lib/cmake/exip/exipTargets.cmake)

# Add source to this project's executable.
add_executable (ConfLabelReader "ConfLabelReader.cpp" )

list(APPEND EXTRA_LIBS LabelDemux exi2xml exip)

target_link_libraries(ConfLabelReader PUBLIC ${EXTRA_LIBS})

# Install targets
install(TARGETS ConfLabelReader 
    EXPORT ConfLabelReaderTargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)
