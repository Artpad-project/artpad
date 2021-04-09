## This CMakeLists contains test suites to make the whole testing process easier

function(add_test_suite _testName _source)
    add_executable(${_testName} ${_source})
    target_link_libraries(${_testName} ${PROJECT_NAME})
    add_test(${_testName} ${CMAKE_CURRENT_BINARY_DIR}/${_testName})
endfunction()

add_test_suite(test_colorimetrie src/colorimetrie.c)
add_test_suite(test_gui src/gui.c)
add_test_suite(test_image src/image.c)
add_test_suite(test_main src/main.c)
add_test_suite(test_puissanceN src/puissanceN.c)
add_test_suite(test_rotation src/rotation.c)
add_test_suite(test_rotoscopie src/rotoscopie.c)
add_test_suite(test_saturation src/saturation.c)

