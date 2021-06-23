cmake_minimum_required (VERSION 3.8)
project(LabelDemux)

include(GenerateExportHeader)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)

if (WIN32)
    include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../exip/build/vs2010")
    include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../exip/include")
    link_directories("${CMAKE_CURRENT_SOURCE_DIR}/../exip/build/vs2010/x64/Debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /D_USRDLL /D_WINDLL")
    set(CMAKE_INSTALL_LIBDIR $ENV{APPDATA})
else()
    include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../exip/bin/headers")
    link_directories("${CMAKE_CURRENT_SOURCE_DIR}/../exip/bin/lib")
endif()

include(${CMAKE_INSTALL_LIBDIR}/lib/cmake/libmp2t/libmp2tTargets.cmake)

aux_source_directory(. SRC_LIST)
add_library(LabelDemux SHARED ${SRC_LIST})
target_link_libraries(LabelDemux PRIVATE lcss::libmp2t)
if (WIN32)
    target_link_libraries(LabelDemux PRIVATE exip.lib)
else()
    target_link_libraries(LabelDemux PRIVATE libexip.a)
endif()

generate_export_header(LabelDemux
    EXPORT_FILE_NAME "${CMAKE_CURRENT_SOURCE_DIR}/labeldemux_export.h")

target_include_directories(LabelDemux
          PUBLIC 
          "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
          "$<INSTALL_INTERFACE:include>"
          )

install(TARGETS LabelDemux 
    EXPORT LabelDemuxTargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)

install(FILES
    LabelDemux.h
    ${PROJECT_BINARY_DIR}/labeldemux_export.h  DESTINATION include)

# generate and install export file
install(EXPORT LabelDemuxTargets
        FILE LabelDemuxTargets.cmake
        NAMESPACE ThetaStream::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/lib/cmake/LabelDemux
)