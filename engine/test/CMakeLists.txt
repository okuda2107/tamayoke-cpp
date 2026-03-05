set(ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/..")

file(GLOB_RECURSE TEST_FILES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*/*.cpp"
)

add_subdirectory("${ROOT_DIR}/exlibs/googletest-1.17.0" "${ROOT_DIR}/exlibs/googletest-1.17.0/build")

enable_testing()

add_executable(testApp "${TEST_FILES}")

target_link_libraries(testApp PRIVATE GTest::gtest_main engine)

include(GoogleTest)

gtest_discover_tests(testApp)

add_custom_target(test
    COMMAND $<TARGET_FILE:testApp>
    DEPENDS testApp
)
