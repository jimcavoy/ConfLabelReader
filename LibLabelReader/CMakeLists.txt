cmake_minimum_required (VERSION 3.8)

if (WIN32)
    include_directories("C:/Program Files (x86)/GSL/include")
    include_directories("C:/Users/james/Documents/Work/Projects/LCSS/3rdParty/exip/include")
    include_directories("C:/Users/james/Documents/Work/Projects/LCSS/3rdParty/exip/build/vs2010")
    link_directories("C:/Users/james/Documents/Work/Projects/LCSS/Src/ConfBind/x64/Debug")
endif()

include(${INSTALL_PREFIX}/lib/cmake/libmp2t/libmp2tTargets.cmake)

aux_source_directory(. SRC_LIST)
add_library(LibLabelReader SHARED ${SRC_LIST})
target_link_libraries(LibLabelReader PRIVATE lcss::libmp2t)
target_link_libraries(LibLabelReader PRIVATE exip.lib)

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