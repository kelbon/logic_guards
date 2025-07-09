cmake_minimum_required(VERSION 3.21)

add_executable(zal ${CMAKE_CURRENT_SOURCE_DIR}/test_zal.cpp)
target_link_libraries(zal PUBLIC zallib)
add_test(NAME test_zal COMMAND zal)
set_target_properties(zal PROPERTIES
	CMAKE_CXX_EXTENSIONS OFF
	LINKER_LANGUAGE CXX
	CXX_STANDARD 20
	CMAKE_CXX_STANDARD_REQUIRED ON
)

file(GLOB ZAL_TESTS REGULAR_EXPRESSION ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
source_group(Tests FILES ${ZAL_TESTS})
