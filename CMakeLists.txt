cmake_minimum_required(VERSION 3.6)
project(gravity VERSION "0.1.0")

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
include(CMakePackageConfigHelpers)
include(cotire)
include(GravityExportTarget)

set(ConfigPackageLocation "lib/cmake/gravity")

if(UNIX OR MINGW)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fconcepts -std=c++1z -Wall -Wextra -Wpedantic -Wno-cpp -fdiagnostics-color=always")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -flto")
endif()
if ("" STREQUAL "Clang")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wdocumentation -Wthread-safety")
endif()

find_package(Doxygen)
if(Doxygen_FOUND)
	file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/doc)
	if(Doxygen_DOT_FOUND)
		set(USE_DOT "YES")
	else()
		set(USE_DOT "NO")
	endif()
	configure_file("${CMAKE_SOURCE_DIR}/Doxyfile.in" "${CMAKE_BINARY_DIR}/Doxyfile" @ONLY)
	#add_custom_target(doc	${DOXYGEN_EXECUTABLE} "${CMAKE_BINARY_DIR}/Doxyfile"
	#		WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/doc")
endif()

add_subdirectory(3rdparty)
add_subdirectory(src bin)
add_subdirectory(test test EXCLUDE_FROM_ALL)
