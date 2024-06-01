set(CORE_THIRD_PARTY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party)
set(CORE_THIRD_PARTY_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/third_party)
include(skia.cmake)
add_subdirectory(third_party/lua)

set(INAE_CORE_INCLUDE_PATH
    ${SKIA_PATH}
    ${CORE_THIRD_PARTY_DIR}/lua
)

include_directories(
    ${INAE_CORE_INCLUDE_PATH}
)

add_library(core
    common_types.h
    model/property/dynamic_property_animator.h
    model/property/dynamic_property.h
    model/property/final_properties.h
    model/property/layer_descriptor.h
    model/property/object.cpp
    model/property/object_descriptor.h
    model/property/object.h

    model/property/property_macros.h
    model/property/property_animator.h
    model/property/property.h
    model/property/property_descriptor.h

    model/keyframes/keyframe.h
    model/property/property_update_listener.h
    model/property/update_listener.h
    model/keyframes/keyframe_value_provider.h
    model/keyframes/keyframe_utility.h
    model/transform.h
    model/transform.cpp
    model/layers/layer.h
    model/layers/layer.cpp
    model/layers/solid_layer.h
    model/layers/solid_layer.cpp
    model/layers/shape_layer.h
    model/layers/shape_layer.cpp
    model/composition.h
    model/composition.cpp
    serializer/serializer.h
    serializer/serializer.cpp
    serializer/json_helper.h
    serializer/json_property_parser.h
    inae_version.h
    color_type.h
    profiler.h

    composition/interfaces/painting_element.h
    composition/composition_node.h
    composition/animation_nodes/transformation_node.h
    composition/layer_node.h
    composition/composition_layer_factory.h
    composition/solid_layer_node.h
    composition/animation_nodes/transformation_node.cpp
    composition/layer_node.cpp
    composition/composition_layer_factory.cpp
    composition/solid_layer_node.cpp
    composition/shape_layer_node.h
    composition/shape_layer_node.cpp
    composition/composition_node.cpp
    composition/text_layer_node.h
    composition/text_layer_node.cpp
    composition/null_layer_node.h
    composition/null_layer_node.cpp
    composition/animation_nodes/group_node.h
    composition/animation_nodes/group_node.cpp
    composition/animation_nodes/rectangle_node.h
    composition/animation_nodes/rectangle_node.cpp
    composition/animation_nodes/shape_node.h
    composition/animation_nodes/shape_node.cpp
    composition/animation_nodes/ellipse_node.h
    composition/animation_nodes/ellipse_node.cpp
    composition/animation_nodes/fill_node.h
    composition/animation_nodes/fill_node.cpp
    composition/animation_nodes/repeater_node.h
    composition/animation_nodes/repeater_node.cpp
    composition/animation_nodes/trim_node.h
    composition/animation_nodes/trim_node.cpp
    composition/animation_nodes/stroke_node.h
    composition/animation_nodes/stroke_node.cpp
    composition/animation_nodes/shape_transformation_node.h
    composition/animation_nodes/shape_transformation_node.cpp
    composition/animation_nodes/shape_path_node.h
    composition/animation_nodes/shape_path_node.cpp
    composition/animation_nodes/compound_trim_path.h
    composition/animation_nodes/compound_trim_path.cpp
    composition/animation_nodes/animation_node_factory.h
    composition/animation_nodes/animation_node_factory.cpp
    composition/animation_nodes/path_algorithm.h
    composition/animation_nodes/skia_path_algorithm.cpp
    utility.h
    exporter/exporter.h exporter/exporter.cpp
    script/expression_value_provider.h
    script/expression_engine.h
    script/expression_engine.cpp
    model/keyframes/keyframe_base.h
    model/keyframes/keyframe_base.cpp
    model/shape_items/shape_item.h
    model/shape_items/shape_item.cpp
    model/shape_items/group.h
    model/shape_items/group.cpp
    model/shape_items/shape_factory.h
    model/shape_items/shape_factory.cpp
    model/shape_items/rectangle.h
    model/shape_items/rectangle.cpp
    model/shape_items/shape.h
    model/shape_items/shape.cpp
    model/shape_items/ellipse.h
    model/shape_items/ellipse.cpp
    model/shape_items/fill.h
    model/shape_items/fill.cpp
    model/shape_items/stroke.h
    model/shape_items/stroke.cpp
    model/shape_items/repeater.h
    model/shape_items/repeater.cpp
    model/shape_items/trim.h
    model/shape_items/trim.cpp
    model/shape_items/shape_transformation.h
    model/shape_items/shape_transformation.cpp
    path_primitives/bezier_path.h
    path_primitives/bezier_path.cpp
    path_primitives/path_element.h
    path_primitives/path_element.cpp
    path_primitives/curve_vertex.h
    path_primitives/curve_vertex.cpp
    serializer/shape_serializer.h
    serializer/shape_serializer.cpp
    exporter/design.txt
    IMPORTANT_INFORMATION.txt
    model/property/trim_properties.h
    model/property/property_value_validator.h
    model/shape_items/shape_properties_macros.h
    composition/paint_context.h
    composition/paint_context.cpp
    composition/composition_utility.h
    composition/composition_utility.cpp
    model/layers/text_layer.h
    model/layers/text_layer.cpp
    model/text_primitives/text_document_property.h
    model/text_primitives/text_document_property.cpp
    model/layers/text_layer_properties.h
    model/tree_item.h
    model/property/property.cpp
    model/tree_item.cpp
    color.cpp
    private/json.h
    model/property/property_type.h
    model/property/property_descriptor.cpp
    model/property/object_descriptor.cpp
    model/property/property_descriptors.h
    model/shape_items/repeater_properties.h
    initializer.h initializer.cpp
    logging/logger.h logging/logger.cpp
    model/layers/image_layer.h model/layers/image_layer.cpp
    composition/image_layer_node.h composition/image_layer_node.cpp
    script/expression_engine_interface.h
    inae_types/types.h
    model/layers/null_layer.h model/layers/null_layer.cpp
    resource_manager.h resource_manager.cpp
    asset_management/asset_manager.h asset_management/asset_manager.cpp
    Developer.txt
    asset_management/asset.h
    asset_management/font_manager.h
    font.h
    asset_management/font_manager.cpp
    skia_conversion/skia_to_inae.h skia_conversion/skia_to_inae.cpp
    serializer/inae_parser.h serializer/inae_parser.cpp
    serializer/json_helper.cpp
    asset_management/database.h
    empty_values.h
    asset_management/asset.cpp
    asset_management/database.cpp
    asset_management/file_database.h asset_management/file_database.cpp
    # model/common/font_setting.h model/common/font_setting.cpp
)

#if (WIN32)
#else()
#    if(APPLE)
#        target_link_directories(core PUBLIC "/usr/local/opt/llvm@15/lib/c++")
#        target_link_libraries(core PUBLIC c++ c++abi c++experimental fontconfig)
#        message(STATUS "WORKS")
#    endif()
#endif()

set_property(TARGET core PROPERTY COMPILE_WARNING_AS_ERROR ON)

cmake_path(GET CMAKE_CURRENT_SOURCE_DIR PARENT_PATH INAE_CORE_SOURCE_PARENT_DIR)
target_include_directories(core
    PUBLIC ${INAE_CORE_SOURCE_PARENT_DIR}
    ${INAE_CORE_INCLUDE_PATH}
)

target_link_directories (
    core
    PUBLIC
    ${SKIA_PATH}/${SKIA_LIBS_PATH}
)

target_link_libraries(core PUBLIC spdlog::spdlog $<$<BOOL:${MINGW}>:ws2_32>)
target_link_libraries (
    core
    PUBLIC bezier
    ${SKIA_LIBS}
    PRIVATE
    fmt
    lua
    nlohmann_json::nlohmann_json
)


add_executable(skia_cpu skia_test_app/main_cpu.cpp)
target_link_directories (
    skia_cpu
    PRIVATE
    ${SKIA_PATH}/${SKIA_LIBS_PATH}
)


target_link_libraries (
    skia_cpu
    ${SKIA_LIBS}
)

add_executable(skia_gpu skia_test_app/main_gpu.cpp)
target_link_directories (
    skia_gpu
    PRIVATE
    ${SKIA_PATH}/${SKIA_LIBS_PATH}
)


target_link_libraries (
    skia_gpu
    ${SKIA_LIBS}
)


add_executable(script_sandbox script_sandbox/main.cpp)
target_link_directories (
    script_sandbox
    PUBLIC
    ${SKIA_PATH}/${SKIA_LIBS_PATH}
)
target_link_libraries (
    script_sandbox
    ${SKIA_LIBS}
    lua
)

#function(dump_cmake_variables)
#    get_cmake_property(_variableNames VARIABLES)
#    list (SORT _variableNames)
#    foreach (_variableName ${_variableNames})
#        if (ARGV0)
#            unset(MATCHED)
#            string(REGEX MATCH ${ARGV0} MATCHED ${_variableName})
#            if (NOT MATCHED)
#                continue()
#            endif()
#        endif()
#        message(STATUS "${_variableName}=${${_variableName}}")
#    endforeach()
#endfunction()

#dump_cmake_variables()
# add_executable(webp_sandbox webp_sandbox/main.cpp)
# target_link_libraries(webp_sandbox core)


# message(STATUS ${SKIA_PATH}/${SKIA_LIBS_PATH})
# message(STATUS ${SKIA_LIBS})
# message(STATUS PkgConfig::AOM)
#include(GNUInstallDirs)
#install(TARGETS skia_cpu
#    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
#)
