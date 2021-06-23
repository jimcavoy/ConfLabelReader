# CMakeList.txt : CMake project for ConfLabelReader, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)
project(ConfLabelReader)

if (WIN32)
    set(CMAKE_INSTALL_LIBDIR $ENV{APPDATA})
    link_directories("${CMAKE_CURRENT_SOURCE_DIR}/../exip/build/vs2010/x64/Debug")
else()
    link_directories("${CMAKE_CURRENT_SOURCE_DIR}/../exip/bin/lib")
    set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
endif()

# Add source to this project's executable.
add_executable (ConfLabelReader "ConfLabelReader.cpp" )

list(APPEND EXTRA_LIBS LabelDemux exi2xml)

target_link_libraries(ConfLabelReader PUBLIC ${EXTRA_LIBS})
if (WIN32)
    target_link_libraries(ConfLabelReader PRIVATE exip.lib)
else()
    target_link_libraries(ConfLabelReader PRIVATE libexip.a)
endif()

# TODO: Add tests and install targets if needed.
install(TARGETS ConfLabelReader 
    EXPORT ConfLabelReaderTargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)