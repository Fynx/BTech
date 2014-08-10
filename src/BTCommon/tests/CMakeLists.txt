set (TESTS
	WalkRangeTest
)

foreach (test ${TESTS})
	set (test_SRC ${test}.cpp)
	set_source_files_properties (${test_SRC} PROPERTIES COMPILE_FLAGS "-DQT_NO_DATESTRING")
	qt5_wrap_cpp (test_SRC ${test}.h)
	add_executable (${test} ${test_SRC})
	target_link_libraries (${test} BTCommon ${Qt5Test_LIBRARIES})
	add_test (${test} ${EXECUTABLE_OUTPUT_PATH}/${test})
endforeach (test)
