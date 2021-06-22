# CMakeList.txt : CMake project for ConfLabelReader, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

if (WIN32)
    set(CMAKE_INSTALL_LIBDIR $ENV{APPDATA})
else()
    link_directories("${CMAKE_CURRENT_SOURCE_DIR}/../exip/bin/lib")
endif()

# Add source to this project's executable.
add_executable (ConfLabelReader "ConfLabelReader.cpp" )

list(APPEND EXTRA_LIBS LibLabelReader)

target_link_libraries(ConfLabelReader PUBLIC ${EXTRA_LIBS})

# TODO: Add tests and install targets if needed.
install(TARGETS ConfLabelReader 
    EXPORT ConfLabelReaderTargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)