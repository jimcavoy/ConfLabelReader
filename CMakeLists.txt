# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.21)

project (ConfLabelReader
	VERSION 2.0.0
	DESCRIPTION "A program that parses STANAG 4609 Motion Imagery Stream for STANAG 4774 Confidentiality Metadata Labels."
	LANGUAGES C CXX
)

include(GNUInstallDirs)

# Include sub-projects.
add_subdirectory (LabelDemux)
add_subdirectory (exi2xml)
add_subdirectory (ExiDecoder)

find_package(unofficial-brotli CONFIG REQUIRED)
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost REQUIRED program_options url)

add_executable(ConfLabelReader)

target_sources(ConfLabelReader 
  PRIVATE 
    src/main.cpp
    src/CmdLineParser.cpp
    src/UdpOStream.cpp
    src/CmdLineParser.h
    src/UdpOStream.h
)

if (WIN32)
target_link_libraries(ConfLabelReader
  PRIVATE
    LabelDemux
    exi2xml
    unofficial::brotli::brotlidec
    Boost::program_options
    Boost::url
    wsock32
    ws2_32)
else()
target_link_libraries(ConfLabelReader
  PRIVATE
    LabelDemux
    exi2xml
    unofficial::brotli::brotlidec
    Boost::program_options
    Boost::url)
endif()

if (WIN32)
install(TARGETS ConfLabelReader
  RUNTIME_DEPENDENCIES
    PRE_EXCLUDE_REGEXES "api-ms-" "ext-ms-"
    POST_EXCLUDE_REGEXES ".*system32/.*\\.dll"
    DIRECTORIES "$<TARGET_FILE_DIR:ConfLabelReader>")
else ()
install(TARGETS ConfLabelReader)
endif ()

# Test Cases
enable_testing()

if (WIN32)
set(SCHEME "file:///")
else ()
set(SCHEME "file://")
endif ()

add_test(NAME ReadN
	COMMAND ConfLabelReader ${PROJECT_SOURCE_DIR}/Sample/foreman_cif_klv_4774.ts -n5 -o${SCHEME}${PROJECT_SOURCE_DIR}/build/labelsReadN.txt
	WORKING_DIRECTORY ${UNIT_TEST_BIN_OUTPUT_DIR}
)
set_tests_properties(ReadN
	PROPERTIES PASS_REGULAR_EXPRESSION "Labels read: 5
TS Packets read: 1764"
)

add_test(NAME ReadAll
	COMMAND ConfLabelReader ${PROJECT_SOURCE_DIR}/Sample/foreman_cif_klv_4774.ts -o${SCHEME}${PROJECT_SOURCE_DIR}/build/labelsReadAll.txt
	WORKING_DIRECTORY ${UNIT_TEST_BIN_OUTPUT_DIR}
)
set_tests_properties(ReadAll
	PROPERTIES PASS_REGULAR_EXPRESSION "Labels read: 11
TS Packets read: 4258"
)

add_test(NAME NoLabel
	COMMAND ConfLabelReader ${PROJECT_SOURCE_DIR}/Sample/foreman_cif_klv.ts -o${SCHEME}${PROJECT_SOURCE_DIR}/build/labels.txt
	WORKING_DIRECTORY ${UNIT_TEST_BIN_OUTPUT_DIR}
)
set_tests_properties(NoLabel
	PROPERTIES PASS_REGULAR_EXPRESSION "No label in motion imagery file, foreman_cif_klv.ts
Labels read: 0
TS Packets read: 49"
)

add_test(NAME ReadXml
	COMMAND ConfLabelReader ${PROJECT_SOURCE_DIR}/Sample/foreman_cif_klv_4774xml.ts -o${SCHEME}${PROJECT_SOURCE_DIR}/build/labelsReadXml.txt
	WORKING_DIRECTORY ${UNIT_TEST_BIN_OUTPUT_DIR}
)
set_tests_properties(ReadXml
	PROPERTIES PASS_REGULAR_EXPRESSION "Labels read: 11
TS Packets read: 4282"
)

add_test(NAME ReadBrotli
	COMMAND ConfLabelReader ${PROJECT_SOURCE_DIR}/Sample/foreman_cif_klv_4774brl.ts -o${SCHEME}${PROJECT_SOURCE_DIR}/build/labelsReadBrl.txt
	WORKING_DIRECTORY ${UNIT_TEST_BIN_OUTPUT_DIR}
)
set_tests_properties(ReadBrotli
	PROPERTIES PASS_REGULAR_EXPRESSION "Labels read: 11
TS Packets read: 4258"
)

add_test(NAME SendUDP
	COMMAND ConfLabelReader ${PROJECT_SOURCE_DIR}/Sample/foreman_cif_klv_4774brl.ts -o udp://239.3.3.11:50000?ttl=1
	WORKING_DIRECTORY ${UNIT_TEST_BIN_OUTPUT_DIR}
)
set_tests_properties(SendUDP
	PROPERTIES PASS_REGULAR_EXPRESSION "Labels read: 11
TS Packets read: 4258"
)