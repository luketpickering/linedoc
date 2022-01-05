cmake_minimum_required (VERSION 3.14)

project(linedoc)
set(linedoc_VERSION 0.9)

if(NOT DEFINED CMAKE_INSTALL_PREFIX OR 
    "${CMAKE_INSTALL_PREFIX}x" STREQUAL "x" OR 
    "${CMAKE_INSTALL_PREFIX}x" STREQUAL "/usr/localx")
  set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/${CMAKE_SYSTEM_NAME}")
endif()

if(NOT DEFINED CMAKE_BUILD_TYPE OR 
    "${CMAKE_BUILD_TYPE}x" STREQUAL "x")
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_STANDARD 14)

if(NOT DEFINED DOTEST)
  SET(DOTEST FALSE)
endif()

if(DOTEST)
  enable_testing()
endif()

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

if(DOTEST)
  add_executable(linedoc_tests tests.cxx)

  install(TARGETS linedoc_tests DESTINATION test)
  add_test(NAME linedoc_tests COMMAND linedoc_tests)
endif()

file(GLOB HEADERS linedoc/*.hxx)
install(FILES ${HEADERS} DESTINATION ${CMAKE_INSTALL_PREFIX}/include/linedoc)

add_executable(dump-doc dump-doc.cxx)
install(TARGETS dump-doc DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)

configure_file(cmake/setup.sh.in
  "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/setup.sh" @ONLY)
install(FILES
  "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/setup.sh" DESTINATION
  ${CMAKE_INSTALL_PREFIX})

install(FILES
  ${CMAKE_SOURCE_DIR}/cmake/linedocConfig.cmake DESTINATION
  ${CMAKE_INSTALL_PREFIX}/cmake)

FILE(WRITE ${CMAKE_INSTALL_PREFIX}/cmake/linedocVersion.cmake 
  "SET(linedoc_VERSION ${linedoc_VERSION})\n" )
