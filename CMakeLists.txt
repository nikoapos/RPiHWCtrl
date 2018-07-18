cmake_minimum_required(VERSION 3.1.0)
project(RPiHWCtrl)


####################################
# Set the directories of the build #
####################################

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)


###############################
# Set the necessary libraries #
###############################

find_package(Boost COMPONENTS filesystem REQUIRED)
list (APPEND LINK_LIBS ${Boost_LIBRARIES})


#########################
# Build the C++ library #
#########################

# Define that we want to use C++14. Unfortunately the gcc that comes with
# Raspbian does not support C++17.
set (CMAKE_CXX_STANDARD 14)

# Use the include directory for the header files
include_directories(include)

# Generate a library from all files under src/lib
file(GLOB_RECURSE SOURCES "src/lib/*.cpp")
#add_library(rpihwctrl SHARED ${SOURCES})


####################################
# Generate the example executables #
####################################

file(GLOB EXAMPLE_FILES "src/examples/*.cpp")
foreach(example_file ${EXAMPLE_FILES})
    get_filename_component(example_name ${example_file} NAME_WE)
    add_executable(${example_name} ${example_file})
    target_link_libraries(${example_name} rpihwctrl ${LINK_LIBS})
endforeach(example_file)
