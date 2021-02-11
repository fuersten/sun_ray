
if (USE_CLANG_TIDY)
  message(STATUS "Setting up clang tidy")  
  set(CMAKE_CXX_CLANG_TIDY
    clang-tidy;
    -header-filter=.*;
    -checks=-*,bugprone-*,cppcoreguidelines-*,clang-analyzer-*,misc-*,modernize-*,performance-*,portability-*,readability-*,-modernize-use-trailing-return-type,-readability-uppercase-literal-suffix,-cppcoreguidelines-pro-type-union-access,-modernize-use-nodiscard,-cppcoreguidelines-pro-bounds-pointer-arithmetic,-cppcoreguidelines-avoid-magic-numbers,-readability-magic-numbers;)
endif()
