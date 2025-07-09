cmake_minimum_required(VERSION 3.21)

# ZA Logic
project(ZAL LANGUAGES CXX)

option(ZAL_ENABLE_TESTING "enables testing" OFF)

### zallib ###

add_library(zallib INTERFACE)

target_include_directories(zallib INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include")

set_target_properties(zallib PROPERTIES
	CMAKE_CXX_EXTENSIONS OFF
	LINKER_LANGUAGE CXX
	CMAKE_CXX_STANDARD_REQUIRED ON
	CXX_STANDARD 20)

### tests ###

if (ZAL_ENABLE_TESTING)
	include(CTest)
	add_subdirectory(tests)
endif()
