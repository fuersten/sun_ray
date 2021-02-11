add_executable(silhouette
    main.cpp
)
target_link_libraries(silhouette PRIVATE Threads::Threads)
target_include_directories(silhouette PRIVATE ${PROJECT_SOURCE_DIR})
target_include_directories(silhouette PRIVATE ${PROJECT_BINARY_DIR})

target_compile_options(silhouette PRIVATE ${SUNRAY_COMPILE_OPTIONS})

