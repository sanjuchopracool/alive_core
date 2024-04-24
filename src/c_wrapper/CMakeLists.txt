add_library(c_wrapper EXCLUDE_FROM_ALL SHARED
    inae_api.h
    inae_api.cpp
)
target_link_libraries (
    c_wrapper
    PRIVATE core
)

target_compile_definitions(c_wrapper PRIVATE INAE_CORE_SHARED_LIBRARY)

add_executable(api_tester EXCLUDE_FROM_ALL main.cpp)
target_link_libraries(api_tester
    c_wrapper
)
