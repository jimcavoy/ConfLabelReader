cmake_minimum_required (VERSION 3.8)
project(exip)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB LIB_INCLUDE_FILES "./include/*.h")
file(GLOB COMMON_SRC "./src/common/src/*.c")
file(GLOB CONTENTIO_SRC "./src/contentIO/src/*.c")
file(GLOB GRAMMAR_SRC "./src/grammar/src/*.c")
file(GLOB GRAMMARGEN_SRC "./src/grammarGen/src/*.c")
file(GLOB STREAMIO_SRC "./src/streamIO/src/*.c")
file(GLOB STRINGTABLES_SRC "./src/stringTables/src/*.c")

set(SOURCES ${COMMON_SRC} ${CONTENTIO_SRC} ${GRAMMAR_SRC} ${GRAMMARGEN_SRC} ${STREAMIO_SRC} ${STRINGTABLES_SRC})

add_library(exip STATIC ${SOURCES})

include_directories("./include")
include_directories("./src/common/include")
include_directories("./src/contentIO/include")
include_directories("./src/grammar/include")
include_directories("./src/grammarGen/include")
include_directories("./src/streamIO/include")
include_directories("./src/stringTables/include")

if (WIN32)
    set(CMAKE_INSTALL_LIBDIR $ENV{APPDATA})
    include_directories("./build/gcc/pc")
    list(APPEND LIB_INCLUDE_FILES "./build/gcc/pc/exipConfig.h")
else()
	include_directories("./build/gcc/contiki")
    list(APPEND LIB_INCLUDE_FILES "./build/gcc/contiki/exipConfig.h")
endif()

target_include_directories(exip
          PUBLIC 
          "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
          "$<INSTALL_INTERFACE:include>"
          )


# install the target and create export-set
install(TARGETS exip 
    EXPORT exipTargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)

# install header file
install(FILES ${LIB_INCLUDE_FILES} DESTINATION include)

# generate and install export file
install(EXPORT exipTargets
        FILE exipTargets.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/lib/cmake/exip
)