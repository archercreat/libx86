# This file is automatically generated from cmake.toml - DO NOT EDIT
# See https://github.com/build-cpp/cmkr for more information

cmake_minimum_required(VERSION 3.15)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
	message(FATAL_ERROR "In-tree builds are not supported. Run CMake from a separate directory: cmake -B build")
endif()

# Regenerate CMakeLists.txt automatically in the root project
set(CMKR_ROOT_PROJECT OFF)
if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
	set(CMKR_ROOT_PROJECT ON)

	# Bootstrap cmkr
	include(cmkr.cmake OPTIONAL RESULT_VARIABLE CMKR_INCLUDE_RESULT)
	if(CMKR_INCLUDE_RESULT)
		cmkr()
	endif()

	# Enable folder support
	set_property(GLOBAL PROPERTY USE_FOLDERS ON)
endif()

# Create a configure-time dependency on cmake.toml to improve IDE support
if(CMKR_ROOT_PROJECT)
	configure_file(cmake.toml cmake.toml COPYONLY)
endif()

project(example)

include(FetchContent)

message(STATUS "Fetching libx86...")
FetchContent_Declare(
	libx86
	GIT_REPOSITORY
		https://github.com/archercreat/libx86.git
)
FetchContent_MakeAvailable(libx86)

# Target example
set(CMKR_TARGET example)
set(example_SOURCES "")

list(APPEND example_SOURCES
	"src/main.cpp"
)

list(APPEND example_SOURCES
	cmake.toml
)

set(CMKR_SOURCES ${example_SOURCES})
add_executable(example)

if(example_SOURCES)
	target_sources(example PRIVATE ${example_SOURCES})
endif()

get_directory_property(CMKR_VS_STARTUP_PROJECT DIRECTORY ${PROJECT_SOURCE_DIR} DEFINITION VS_STARTUP_PROJECT)
if(NOT CMKR_VS_STARTUP_PROJECT)
	set_property(DIRECTORY ${PROJECT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT example)
endif()

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${example_SOURCES})

target_compile_features(example PRIVATE
	cxx_std_20
)

target_link_libraries(example PRIVATE
	libx86
)

unset(CMKR_TARGET)
unset(CMKR_SOURCES)

