cmake_minimum_required (VERSION 3.21)

project(ExiDecoder
    DESCRIPTION "Application to decode EXI streams."
    LANGUAGES CXX
)

add_executable(ExiDecoder)

target_sources(ExiDecoder 
  PRIVATE 
    src/main.cpp)

target_link_libraries(ExiDecoder PRIVATE exi2xml)

# Test Cases
enable_testing()

add_test(NAME DecodeExi
  COMMAND ExiDecoder ${PROJECT_SOURCE_DIR}/../Sample/example1.exi
)

set_tests_properties( DecodeExi
  PROPERTIES PASS_REGULAR_EXPRESSION 
    "<ns1:GenericValue>AUS</ns1:GenericValue>"
)