FetchContent_Declare(
  date
  URL https://github.com/HowardHinnant/date/archive/v3.0.0.tar.gz
)

FetchContent_GetProperties(date)
if(NOT date_POPULATED)
  FetchContent_Populate(date)
endif()

include_directories(SYSTEM "${date_SOURCE_DIR}/include")
