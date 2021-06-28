# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("ConfLabelReader")

# Include sub-projects.
add_subdirectory ("ConfLabelReader")
add_subdirectory ("LabelDemux")
add_subdirectory ("exi2xml")

# Test Cases
enable_testing()

message("Working dir: ${CMAKE_CURRENT_BINARY_DIR}")
add_test(NAME Usage 
	COMMAND ConfLabelReader
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/LabelDemux
	)
set_tests_properties(Usage
	PROPERTIES PASS_REGULAR_EXPRESSION "usage: ConfLabelReader <MPEG_transport_stream_file>"
	)
