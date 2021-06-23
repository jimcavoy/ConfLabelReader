# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("ConfLabelReader")

# Include sub-projects.
add_subdirectory ("ConfLabelReader")
add_subdirectory ("LabelDemux")
add_subdirectory ("exi2xml")
