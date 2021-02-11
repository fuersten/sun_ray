add_executable(scene
    main.cpp
)
target_link_libraries(scene PRIVATE Threads::Threads)
target_include_directories(scene PRIVATE ${PROJECT_SOURCE_DIR})
target_include_directories(scene PRIVATE ${PROJECT_BINARY_DIR})

target_compile_options(scene PRIVATE ${SUNRAY_COMPILE_OPTIONS})

