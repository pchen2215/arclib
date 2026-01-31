# Collect test source files
file(GLOB_RECURSE TEST_SOURCES "*.cpp")

# Add all tests
foreach(test_source ${TEST_SOURCES})
    # Get test name from source file name
    get_filename_component(test_name ${test_source} NAME_WE)
    
    # Create executable
    add_executable(${test_name} ${test_source})
    target_link_libraries(${test_name} PRIVATE arclib)

    # Add to CTest
    add_test(NAME ${test_name} COMMAND ${test_name})
endforeach()
