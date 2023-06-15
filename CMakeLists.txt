cmake_minimum_required (VERSION 3.14)

project(linedoc VERSION 23.06 LANGUAGES CXX)

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

option(DOTEST "Whether to run tests" OFF)

if(DOTEST)
  enable_testing()
endif()

file(GLOB HEADERS linedoc/*.hxx)
install(FILES ${HEADERS} DESTINATION ${CMAKE_INSTALL_PREFIX}/include/linedoc)

add_library(linedoc_includes INTERFACE)
target_include_directories(linedoc_includes INTERFACE 
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:include>)
set_target_properties(linedoc_includes PROPERTIES EXPORT_NAME linedoc::includes)

install(TARGETS linedoc_includes EXPORT linedocTargets)

add_library(linedoc::includes ALIAS linedoc_includes)

if(DOTEST)
  add_executable(linedoc_tests tests.cxx)
  target_link_libraries(linedoc_tests PRIVATE linedoc_includes)

  install(TARGETS linedoc_tests DESTINATION test)
  add_test(NAME linedoc_tests COMMAND linedoc_tests)
endif()

add_executable(dump-doc dump-doc.cxx)
target_link_libraries(dump-doc PRIVATE linedoc_includes)
install(TARGETS dump-doc DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${PROJECT_BINARY_DIR}/linedocConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)
configure_package_config_file(
  "${CMAKE_CURRENT_LIST_DIR}/cmake/Templates/linedocConfig.cmake.in"
  "${PROJECT_BINARY_DIR}/linedocConfig.cmake"
  INSTALL_DESTINATION cmake
  NO_SET_AND_CHECK_MACRO
  NO_CHECK_REQUIRED_COMPONENTS_MACRO
)

install(EXPORT linedocTargets
        DESTINATION lib/cmake/linedoc )

install(FILES "${PROJECT_BINARY_DIR}/linedocConfigVersion.cmake"
              "${PROJECT_BINARY_DIR}/linedocConfig.cmake"
        DESTINATION lib/cmake/linedoc)

configure_file(${CMAKE_CURRENT_LIST_DIR}/cmake/Templates/setup.linedoc.sh.in ${PROJECT_BINARY_DIR}/setup.linedoc.sh @ONLY)
install(PROGRAMS ${PROJECT_BINARY_DIR}/setup.linedoc.sh DESTINATION bin)