cmake_minimum_required(VERSION 3.21)
project(logic_guards LANGUAGES CXX)

option(${PROJECT_NAME}_ENABLE_TESTING "enables testing" OFF)

file(GLOB ${PROJECT_NAME}_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME}/*.hpp")

### ${PROJECT_NAME}lib ###

add_library(${PROJECT_NAME}lib INTERFACE "${${PROJECT_NAME}_HEADERS}")

target_include_directories(${PROJECT_NAME}lib INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include")

set_target_properties(${PROJECT_NAME}lib PROPERTIES
	CMAKE_CXX_EXTENSIONS OFF
	LINKER_LANGUAGE CXX
	CMAKE_CXX_STANDARD_REQUIRED ON
	CXX_STANDARD 20)

### visual ###

source_group(Include FILES ${${PROJECT_NAME}_HEADERS})

### tests ###

if(${PROJECT_NAME}_ENABLE_TESTING)
	include(CTest)
	add_subdirectory(tests)
endif()
