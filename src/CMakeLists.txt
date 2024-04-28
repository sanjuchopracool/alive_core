add_subdirectory(core)
add_subdirectory(core_utility)
add_subdirectory(c_wrapper)


if(NOT WIN32)
add_subdirectory(test)
endif()
