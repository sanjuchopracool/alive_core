add_library(c_wrapper SHARED
    inae_api.h
    inae_api.cpp
)
target_link_libraries (
    c_wrapper
    PRIVATE core
)

target_compile_definitions(c_wrapper PRIVATE INAE_CORE_SHARED_LIBRARY)

add_executable(api_tester main.cpp)
target_link_libraries(api_tester
    c_wrapper
)
