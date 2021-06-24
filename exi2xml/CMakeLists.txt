cmake_minimum_required (VERSION 3.8)
project(exi2xml)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if (WIN32)
    set(CMAKE_INSTALL_LIBDIR $ENV{APPDATA})
endif()

include(${CMAKE_INSTALL_LIBDIR}/lib/cmake/exip/exipTargets.cmake)

aux_source_directory(. SRC_LIST)
add_library(exi2xml STATIC ${SRC_LIST})
target_link_libraries(exi2xml PRIVATE exip)

target_include_directories(exi2xml
          PUBLIC 
          "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
          "$<INSTALL_INTERFACE:include>"
          )

file(GLOB LIB_INCLUDE_FILES "./*.h")

# install the target and create export-set
install(TARGETS exi2xml 
    EXPORT exi2xmlTargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)

# install header file
install(FILES ${LIB_INCLUDE_FILES} DESTINATION include)

# generate and install export file
install(EXPORT exi2xmlTargets
        FILE exi2xmlTargets.cmake
        NAMESPACE lcss::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/lib/cmake/exi2xml
)