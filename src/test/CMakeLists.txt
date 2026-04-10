find_package(OpenCV)
if (OpenCV_FOUND)
    include_directories( ${OpenCV_INCLUDE_DIRS} )
    add_executable(compare_comps main.cpp)
    target_link_libraries(compare_comps
        PRIVATE core
        ${OpenCV_LIBS}
    )
endif()
