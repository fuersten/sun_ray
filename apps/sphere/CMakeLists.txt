add_executable(sphere
    main.cpp
)
target_link_libraries(sphere PRIVATE Threads::Threads)
target_include_directories(sphere PRIVATE ${PROJECT_SOURCE_DIR})
target_include_directories(sphere PRIVATE ${PROJECT_BINARY_DIR})

target_compile_options(sphere PRIVATE ${SUNRAY_COMPILE_OPTIONS})

