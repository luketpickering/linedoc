cmake_minimum_required (VERSION 2.8)

project(linedoc)

#Changes default install path to be a subdirectory of the build dir.
#Can set build dir at configure time with -DCMAKE_INSTALL_PREFIX=/install/path
if(CMAKE_INSTALL_PREFIX STREQUAL "" OR CMAKE_INSTALL_PREFIX STREQUAL
  "/usr/local")
  SET(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/${CMAKE_SYSTEM_NAME}")
elseif(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/${CMAKE_SYSTEM_NAME}")
endif()
message(STATUS "Installing to: ${CMAKE_INSTALL_PREFIX}")


if(NOT DEFINED DOTEST)
  SET(DOTEST FALSE)
endif()

if(NOT DEFINED DOCOV)
  SET(DOCOV FALSE)
endif()

if(DOTEST)
  enable_testing()
  set(CMAKE_BUILD_TYPE Debug)
endif()

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

SET(CXX_WARNINGS -Wall)

SET(EXTRA_CXX_FLAGS)
if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 5.0)
  LIST(APPEND EXTRA_CXX_FLAGS -std=c++1y --coverage -g -O0)
elseif(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 4.8)
  LIST(APPEND EXTRA_CXX_FLAGS -std=c++11 --coverage -g -O0)
endif()

string(TOLOWER ${CMAKE_BUILD_TYPE} CMAKE_BUILD_TYPE_lower)
if(CMAKE_BUILD_TYPE_lower STREQUAL "debug")
  LIST(APPEND EXTRA_CXX_FLAGS -g -O0)
endif()

if(DOCOV)
  LIST(APPEND EXTRA_CXX_FLAGS --coverage)
endif()

string(REPLACE ";" " " STR_EXTRA_CXX_FLAGS "${EXTRA_CXX_FLAGS}")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${STR_EXTRA_CXX_FLAGS} ${CXX_WARNINGS}")

if(DOTEST)
  add_executable(linedoc_tests tests.cxx)

  if(DOCOV)
    include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/CodeCoverage.cmake)
    set(COVERAGE_EXCLUDES '5*' '*.cxx')
    setup_target_for_coverage(NAME linedoc_coverage EXECUTABLE linedoc_tests DEPENDENCIES linedoc_tests)
  endif()

  install(TARGETS linedoc_tests DESTINATION test)
  add_test(NAME linedoc_tests COMMAND linedoc_tests)

endif()

file(GLOB HEADERS linedoc/*.hxx)
install(FILES ${HEADERS} DESTINATION ${CMAKE_INSTALL_PREFIX}/include/linedoc)

add_executable(dump-doc dump-doc.cxx)
install(TARGETS dump-doc DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
