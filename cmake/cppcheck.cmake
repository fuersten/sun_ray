
if (USE_CPPCHECK)
  message(STATUS "Setting up cpp check")
  find_program(CMAKE_CXX_CPPCHECK NAMES cppcheck)
  if (CMAKE_CXX_CPPCHECK)
      list(
          APPEND CMAKE_CXX_CPPCHECK
              "--enable=warning,performance,style,portability"
              "--inconclusive"
              "--force"
              "--inline-suppr"
              "--verbose"
              "--quiet"
              "--std=c++14"
      )
  endif()
endif()
