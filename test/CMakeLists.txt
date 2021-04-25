add_executable(sun_ray_test
  main.cpp

  application_test.cpp
  canvas_file_writer_test.cpp
  canvas_writer_test.cpp
  helper_test.cpp
  options_test.cpp

  temporary_directory.h

  feature/camera_test.cpp
  feature/canvas_test.cpp
  feature/color_test.cpp
  feature/cube_test.cpp
  feature/cylinder_test.cpp
  feature/disk_test.cpp
  feature/intersect_test.cpp
  feature/intersection_state_test.cpp
  feature/light_test.cpp
  feature/material_test.cpp
  feature/matrix_test.cpp
  feature/object_test.cpp
  feature/pattern_test.cpp
  feature/plane_test.cpp
  feature/ray_test.cpp
  feature/sphere_test.cpp
  feature/transformation_test.cpp
  feature/triangle_test.cpp
  feature/tuple_test.cpp
  feature/world_test.cpp

  script/camera_test.cpp
  script/canvas_test.cpp
  script/checker_pattern_test.cpp
  script/color_test.cpp
  script/cube_test.cpp
  script/diagnostic_message_test.cpp
  script/disk_test.cpp
  script/dump_visitor_test.cpp
  script/engine_test.cpp
  script/function_object_test.cpp
  script/function_registry_test.cpp
  script/function_test.cpp
  script/gradient_pattern_test.cpp
  script/language_test.cpp
  script/light_test.cpp
  script/location_test.cpp
  script/material_test.cpp
  script/measurement_test.cpp
  script/meta_class_test.cpp
  script/name_mangler_test.cpp
  script/parser_test.cpp
  script/plane_test.cpp
  script/point_test.cpp
  script/ring_pattern_test.cpp
  script/scanner_test.cpp
  script/sphere_test.cpp
  script/stack_machine_test.cpp
  script/stack_machine_visitor_test.cpp
  script/stack_test.cpp
  script/stream_reader_test.cpp
  script/string_helper_test.cpp
  script/stripe_pattern_test.cpp
  script/token_test.cpp
  script/triangle_test.cpp
  script/types_test.cpp
  script/vector_test.cpp
  script/world_test.cpp
)

include(${PROJECT_SOURCE_DIR}/cmake/fetch_catch2.cmake)

target_link_libraries(sun_ray_test PRIVATE Threads::Threads Catch2::Catch2)
target_include_directories(sun_ray_test SYSTEM PRIVATE "${catch2_SOURCE_DIR}/single_include")
target_include_directories(sun_ray_test PRIVATE ${PROJECT_SOURCE_DIR})
target_include_directories(sun_ray_test PRIVATE ${PROJECT_BINARY_DIR})

if(COVERAGE)
  target_link_libraries(sun_ray_test PUBLIC --coverage)
  target_compile_options(sun_ray_test PUBLIC --coverage -g)
else()
  target_compile_options(sun_ray_test PRIVATE ${SUNRAY_COMPILE_OPTIONS})
endif()

include(ParseAndAddCatchTests)
ParseAndAddCatchTests(sun_ray_test)
