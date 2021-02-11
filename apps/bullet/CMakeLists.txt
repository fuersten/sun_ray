add_executable(bullet
    main.cpp
)
target_link_libraries(bullet PRIVATE Threads::Threads)
target_include_directories(bullet PRIVATE ${PROJECT_SOURCE_DIR})
target_include_directories(bullet PRIVATE ${PROJECT_BINARY_DIR})

target_compile_options(bullet PRIVATE ${SUNRAY_COMPILE_OPTIONS})

