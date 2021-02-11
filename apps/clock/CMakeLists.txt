add_executable(clock
    main.cpp
)
target_link_libraries(clock PRIVATE Threads::Threads)
target_include_directories(clock PRIVATE ${PROJECT_SOURCE_DIR})
target_include_directories(clock PRIVATE ${PROJECT_BINARY_DIR})

target_compile_options(clock PRIVATE ${SUNRAY_COMPILE_OPTIONS})

