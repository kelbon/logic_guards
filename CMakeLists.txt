cmake_minimum_required(VERSION 3.21)
project(logic_guards LANGUAGES CXX)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  set(IS_TOPLEVEL_PROJECT TRUE)
else()
  set(IS_TOPLEVEL_PROJECT FALSE)
endif()

option(${PROJECT_NAME}_BUILD_TESTING "builds and enables testing" ${IS_TOPLEVEL_PROJECT})

set(${PROJECT_NAME}_HEADERS "include/${PROJECT_NAME}/${PROJECT_NAME}.hpp")
set(${PROJECT_NAME}_DOCS LICENSE README.md)

### visual ###

source_group(header FILES ${${PROJECT_NAME}_HEADERS})
source_group(docs   FILES ${${PROJECT_NAME}_DOCS})

include(GNUInstallDirs)

### ${PROJECT_NAME}lib ###

add_library(${PROJECT_NAME}lib INTERFACE "${${PROJECT_NAME}_HEADERS}" ${${PROJECT_NAME}_DOCS})

target_include_directories(${PROJECT_NAME}lib INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

set_target_properties(${PROJECT_NAME}lib PROPERTIES
  CMAKE_CXX_EXTENSIONS OFF
  LINKER_LANGUAGE CXX
  CMAKE_CXX_STANDARD_REQUIRED ON
  CXX_STANDARD 20)

### tests ###

include(CTest)
if(BUILD_TESTING AND ${PROJECT_NAME}_BUILD_TESTING)
  add_subdirectory(tests)
endif()
