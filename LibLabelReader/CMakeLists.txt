cmake_minimum_required (VERSION 3.8)

if (WIN32)
    include_directories("C:/Program Files (x86)/GSL/include")
endif()

aux_source_directory(. SRC_LIST)
add_library(LibLabelReader STATIC ${SRC_LIST})

if(WIN32)
    target_link_libraries(LibLabelReader wsock32 ws2_32)
endif()

target_include_directories(LibLabelReader
          PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
          )

file(GLOB LIB_INCLUDE_FILES "./*.h")

install(TARGETS LibLabelReader 
    EXPORT LibLabelReaderTarget
    LIBRARY DESTINATION lib
    INCLUDES DESTINATION include)
install(FILES ${LIB_INCLUDE_FILES} DESTINATION include)