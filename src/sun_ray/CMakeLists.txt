configure_file(
    "${CMAKE_SOURCE_DIR}/src/version.h.in"
    "${PROJECT_BINARY_DIR}/sun_ray/version.h"
)

set(SUN_RAY_SCRIPT_SOURCES
  ${CMAKE_SOURCE_DIR}/sun_ray/script/ast.h
  ${CMAKE_SOURCE_DIR}/sun_ray/script/build_in_functions.h
  ${CMAKE_SOURCE_DIR}/sun_ray/script/class.h
  ${CMAKE_SOURCE_DIR}/sun_ray/script/diagnostic_message_handler.h
  ${CMAKE_SOURCE_DIR}/sun_ray/script/engine.h
  ${CMAKE_SOURCE_DIR}/sun_ray/script/format_helper.h
  ${CMAKE_SOURCE_DIR}/sun_ray/script/function_registry.h
  ${CMAKE_SOURCE_DIR}/sun_ray/script/function_object.h
  ${CMAKE_SOURCE_DIR}/sun_ray/script/function_traits.h
  ${CMAKE_SOURCE_DIR}/sun_ray/script/location.h
  ${CMAKE_SOURCE_DIR}/sun_ray/script/meta_class_fwd.h
  ${CMAKE_SOURCE_DIR}/sun_ray/script/meta_class.h
  ${CMAKE_SOURCE_DIR}/sun_ray/script/name_mangler.h
  ${CMAKE_SOURCE_DIR}/sun_ray/script/parser.h
  ${CMAKE_SOURCE_DIR}/sun_ray/script/scanner.h
  ${CMAKE_SOURCE_DIR}/sun_ray/script/stack_machine.h
  ${CMAKE_SOURCE_DIR}/sun_ray/script/stack.h
  ${CMAKE_SOURCE_DIR}/sun_ray/script/stream_reader.h
  ${CMAKE_SOURCE_DIR}/sun_ray/script/string_helper.h
  ${CMAKE_SOURCE_DIR}/sun_ray/script/symboltable.h
  ${CMAKE_SOURCE_DIR}/sun_ray/script/token.h
  ${CMAKE_SOURCE_DIR}/sun_ray/script/types.h
  ${CMAKE_SOURCE_DIR}/sun_ray/script/visitor/dump_visitor.h
  ${CMAKE_SOURCE_DIR}/sun_ray/script/visitor/stack_machine_visitor.h
  ${CMAKE_SOURCE_DIR}/sun_ray/script/visitor/validation_visitor.h
)

set(SUN_RAY_SCRIPT_OBJECTS
  ${CMAKE_SOURCE_DIR}/sun_ray/script/objects/camera.h
  ${CMAKE_SOURCE_DIR}/sun_ray/script/objects/canvas.h
  ${CMAKE_SOURCE_DIR}/sun_ray/script/objects/checker_pattern.h
  ${CMAKE_SOURCE_DIR}/sun_ray/script/objects/color.h
  ${CMAKE_SOURCE_DIR}/sun_ray/script/objects/cube.h
  ${CMAKE_SOURCE_DIR}/sun_ray/script/objects/disk.h
  ${CMAKE_SOURCE_DIR}/sun_ray/script/objects/gradient_pattern.h
  ${CMAKE_SOURCE_DIR}/sun_ray/script/objects/light.h
  ${CMAKE_SOURCE_DIR}/sun_ray/script/objects/material.h
  ${CMAKE_SOURCE_DIR}/sun_ray/script/objects/measurement.h
  ${CMAKE_SOURCE_DIR}/sun_ray/script/objects/object.h
  ${CMAKE_SOURCE_DIR}/sun_ray/script/objects/pattern.h
  ${CMAKE_SOURCE_DIR}/sun_ray/script/objects/plane.h
  ${CMAKE_SOURCE_DIR}/sun_ray/script/objects/point.h
  ${CMAKE_SOURCE_DIR}/sun_ray/script/objects/ring_pattern.h
  ${CMAKE_SOURCE_DIR}/sun_ray/script/objects/sphere.h
  ${CMAKE_SOURCE_DIR}/sun_ray/script/objects/stripe_pattern.h
  ${CMAKE_SOURCE_DIR}/sun_ray/script/objects/vector.h
  ${CMAKE_SOURCE_DIR}/sun_ray/script/objects/triangle.h
  ${CMAKE_SOURCE_DIR}/sun_ray/script/objects/world.h
)

source_group("Header Files\\script" FILES ${SUN_RAY_SCRIPT_SOURCES})
source_group("Header Files\\script\\objects" FILES ${SUN_RAY_SCRIPT_OBJECTS})

add_executable(sun_ray
    main.cpp

    application.h
    options.h

    ${SUN_RAY_SCRIPT_SOURCES}
    ${SUN_RAY_SCRIPT_OBJECTS}

    ${CMAKE_SOURCE_DIR}/sun_ray/canvas_writer.h
    ${CMAKE_SOURCE_DIR}/sun_ray/canvas_ppm3_writer.h
    ${CMAKE_SOURCE_DIR}/sun_ray/canvas_ppm6_writer.h
    ${CMAKE_SOURCE_DIR}/sun_ray/canvas_file_writer.h

    ${CMAKE_SOURCE_DIR}/sun_ray/feature/camera.h
    ${CMAKE_SOURCE_DIR}/sun_ray/feature/canvas.h
    ${CMAKE_SOURCE_DIR}/sun_ray/feature/checker_pattern.h
    ${CMAKE_SOURCE_DIR}/sun_ray/feature/color.h
    ${CMAKE_SOURCE_DIR}/sun_ray/feature/cube.h
    ${CMAKE_SOURCE_DIR}/sun_ray/feature/disk.h
    ${CMAKE_SOURCE_DIR}/sun_ray/feature/gradient_pattern.h
    ${CMAKE_SOURCE_DIR}/sun_ray/feature/intersection_state.h
    ${CMAKE_SOURCE_DIR}/sun_ray/feature/intersection.h
    ${CMAKE_SOURCE_DIR}/sun_ray/feature/light.h
    ${CMAKE_SOURCE_DIR}/sun_ray/feature/material.h
    ${CMAKE_SOURCE_DIR}/sun_ray/feature/math_helper.h
    ${CMAKE_SOURCE_DIR}/sun_ray/feature/matrix.h
    ${CMAKE_SOURCE_DIR}/sun_ray/feature/object.h
    ${CMAKE_SOURCE_DIR}/sun_ray/feature/pattern.h
    ${CMAKE_SOURCE_DIR}/sun_ray/feature/plane.h
    ${CMAKE_SOURCE_DIR}/sun_ray/feature/ray.h
    ${CMAKE_SOURCE_DIR}/sun_ray/feature/ring_pattern.h
    ${CMAKE_SOURCE_DIR}/sun_ray/feature/sphere.h
    ${CMAKE_SOURCE_DIR}/sun_ray/feature/stripe_pattern.h
    ${CMAKE_SOURCE_DIR}/sun_ray/feature/transformation.h
    ${CMAKE_SOURCE_DIR}/sun_ray/feature/triangle.h
    ${CMAKE_SOURCE_DIR}/sun_ray/feature/tuple.h
    ${CMAKE_SOURCE_DIR}/sun_ray/feature/world.h
)

target_link_libraries(sun_ray PRIVATE Threads::Threads)
target_include_directories(sun_ray PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(sun_ray PRIVATE ${PROJECT_SOURCE_DIR})
target_include_directories(sun_ray PRIVATE ${PROJECT_BINARY_DIR})

if(COVERAGE)
  target_compile_options(sun_ray PUBLIC --coverage)
  target_link_libraries(sun_ray PUBLIC --coverage)
else()
  target_compile_options(sun_ray PRIVATE ${SUNRAY_COMPILE_OPTIONS})
endif()

