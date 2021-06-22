cmake_minimum_required (VERSION 3.8)
project(LibLabelReader)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)


if (WIN32)
    include_directories("C:/Users/james/Documents/Work/Projects/LCSS/3rdParty/exip/include")
    include_directories("C:/Users/james/Documents/Work/Projects/LCSS/3rdParty/exip/build/vs2010")
    link_directories("C:/Users/james/Documents/Work/Projects/LCSS/Src/ConfBind/x64/Debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /DLIBLABELREADER_EXPORTS /D_USRDLL /D_WINDLL")
    set(CMAKE_INSTALL_LIBDIR $ENV{APPDATA})
else()
    include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../exip/bin/headers")
    link_directories("${CMAKE_CURRENT_SOURCE_DIR}/../exip/bin/lib")
endif()

include(${CMAKE_INSTALL_LIBDIR}/lib/cmake/libmp2t/libmp2tTargets.cmake)

aux_source_directory(. SRC_LIST)

if (WIN32)
    add_library(LibLabelReader SHARED ${SRC_LIST})
    target_link_libraries(LibLabelReader PRIVATE lcss::libmp2t)
    target_link_libraries(LibLabelReader PRIVATE exip.lib)
else()
    add_library(LibLabelReader STATIC ${SRC_LIST})
    target_link_libraries(LibLabelReader PRIVATE lcss::libmp2t)
    target_link_libraries(LibLabelReader PRIVATE libexip.a)
    set_target_properties(LibLabelReader 
        PROPERTIES
        OUTPUT_NAME LabelReader)
endif()

target_include_directories(LibLabelReader
          PUBLIC 
          "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
          "$<INSTALL_INTERFACE:include>"
          )

install(TARGETS LibLabelReader 
    EXPORT LibLabelReaderTargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)

install(FILES LibLabelReader.h LabelReader.h DESTINATION include)

# generate and install export file
install(EXPORT LibLabelReaderTargets
        FILE LibLabelReaderTargets.cmake
        NAMESPACE ThetaStream::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/lib/cmake/LibLabelReader
)