cmake_minimum_required (VERSION 3.1.3)

if(CMAKE_VERSION VERSION_GREATER 3.0.0)
	cmake_policy(SET CMP0043 OLD)
endif()

IF (USE_CXX11_ABI)
    ADD_DEFINITIONS (-D_GLIBCXX_USE_CXX11_ABI=1)
ELSE()
    ADD_DEFINITIONS (-D_GLIBCXX_USE_CXX11_ABI=0)
ENDIF()

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmakemodules")

INCLUDE (output_paths)

# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /W3 /MP /openmp")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W3 /MP /openmp")

project(math_core CXX)

set(MATH_CORE_DIR "${CMAKE_BINARY_DIR}/lib")

message("WARNING!: MathCore library default path is MATH_CORE_DIR = $ENV{MATH_CORE_DIR}")
message("Change the enviroment variable to let your project use MathCore libraries")

set(CMAKE_BUILD_WITH_INSTALL_RPATH true)
set(CMAKE_INSTALL_RPATH ".")
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_BUILD_TYPE "RelWithDebInfo")
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

include(add_source_groups)
include(group_files)
include(filter_sources)

FIND_PACKAGE (OMP REQUIRED)
FIND_PACKAGE (Boost 1.61.0 COMPONENTS
    filesystem
    iostreams
    regex
    program_options
    system
    thread
    unit_test_framework
REQUIRED)
# Since boost 1.55.0 we need -lrt for boost_thread
SET (Boost_THREAD_LIBS ${Boost_THREAD_LIBRARY} pthread rt)
INCLUDE_DIRECTORIES (SYSTEM ${Boost_INCLUDE_DIRS})

INCLUDE_DIRECTORIES (${PROJECT_SOURCE_DIR}/include)
ADD_SUBDIRECTORY (src)

ENABLE_TESTING ()
