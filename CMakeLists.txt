# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.21)

project (ConfLabelReader
	VERSION 0.2.0
	DESCRIPTION "A program that parses STANAG 4609 Motion Imagery Stream for STANAG 4774 Confidentiality Metadata Labels."
	LANGUAGES C CXX
)

include(GNUInstallDirs)

find_package(exip CONFIG REQUIRED)

# Include sub-projects.
add_subdirectory (LabelDemux)
add_subdirectory (exi2xml)

add_executable(ConfLabelReader)

target_sources(ConfLabelReader PRIVATE src/main.cpp)

list(APPEND EXTRA_LIBS LabelDemux exi2xml exip)

target_link_libraries(ConfLabelReader PRIVATE ${EXTRA_LIBS})

# Test Cases
enable_testing()

message("Working dir: ${CMAKE_CURRENT_BINARY_DIR}")

add_test(NAME ReadN
	COMMAND ConfLabelReader -i${PROJECT_SOURCE_DIR}/Sample/foreman_cif_klv_4774.ts -n5 -olabels.txt
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/LabelDemux/Debug
	)
set_tests_properties(ReadN
	PROPERTIES PASS_REGULAR_EXPRESSION "Labels read: 5
TS Packets read: 1764"
	)

add_test(NAME ReadAll
	COMMAND ConfLabelReader -i${PROJECT_SOURCE_DIR}/Sample/foreman_cif_klv_4774.ts -olabels.txt
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/LabelDemux/Debug
	)
set_tests_properties(ReadAll
	PROPERTIES PASS_REGULAR_EXPRESSION "Labels read: 11
TS Packets read: 4258"
	)

add_test(NAME NoLabel
	COMMAND ConfLabelReader -i${PROJECT_SOURCE_DIR}/Sample/foreman_cif_klv.ts -olabels.txt
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/LabelDemux/Debug
	)
set_tests_properties(NoLabel
	PROPERTIES PASS_REGULAR_EXPRESSION "No label in motion imagery file, foreman_cif_klv.ts
Labels read: 0
TS Packets read: 49"
	)