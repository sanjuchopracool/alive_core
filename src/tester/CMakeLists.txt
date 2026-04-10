find_package(OpenCV)
include_directories( ${OpenCV_INCLUDE_DIRS} )

add_executable(alive_tester main.cpp
    tester.h tester.cpp
    alivetest_fixture.h alivetest_fixture.cpp
    directory_comparator.cpp
)
target_link_libraries (
    alive_tester
    cxxopts
    gtest_main
    core
    ${OpenCV_LIBS}
)

install (
    TARGETS
    alive_tester
)

# How to Run
# pass these arguments
# --resources_dir /Users/user/alive_resources
# --test_files_dir /Users/user/alive_core_tests
# --golden_output_dir /Users/user/PROJECTS/TEMP_FILES/golden_output_alive_core
# --gtest_filter="/Users/sanju/alive_core_tests.tests/basic_tests/layers/layers_1.json"
