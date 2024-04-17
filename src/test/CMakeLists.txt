cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED )

include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable(compare_comps main.cpp)
target_link_libraries(compare_comps
    PRIVATE core
    ${OpenCV_LIBS}
)
