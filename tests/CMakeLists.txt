cmake_minimum_required(VERSION 3.21)

add_executable(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/test_${PROJECT_NAME}.cpp")
target_link_libraries(${PROJECT_NAME} PUBLIC ${PROJECT_NAME}lib)
add_test(NAME test_${PROJECT_NAME} COMMAND ${PROJECT_NAME})
set_target_properties(${PROJECT_NAME} PROPERTIES
  CMAKE_CXX_EXTENSIONS OFF
  LINKER_LANGUAGE CXX
  CXX_STANDARD 20
  CMAKE_CXX_STANDARD_REQUIRED ON
)

set(${PROJECT_NAME}_TESTS test_logic_guards.cpp)
source_group(tests FILES ${${PROJECT_NAME}_TESTS})
