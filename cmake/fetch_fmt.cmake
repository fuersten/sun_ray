FetchContent_Declare(
  fmt
  URL https://github.com/fmtlib/fmt/archive/7.1.3.tar.gz
)

FetchContent_GetProperties(fmt)
if(NOT fmt_POPULATED)
  FetchContent_Populate(fmt)
endif()

include_directories(SYSTEM "${fmt_SOURCE_DIR}/include")
