cmake_minimum_required(VERSION 2.6)

project(DO_Core_tests)

message(STATUS "  - DO_Core_Tests")

find_package(DO_Sara COMPONENTS Core REQUIRED)

set(TestGroupName "Core")

file(GLOB test_core_SOURCE_FILES FILES *.cpp)

foreach (file ${test_core_SOURCE_FILES})
  get_filename_component(filename "${file}" NAME_WE)

  if (${filename} STREQUAL "test_core_expression")
    continue ()
  endif ()

  sara_add_test(${filename} ${file} "${DO_Sara_LIBRARIES}" ${TestGroupName})
endforeach ()


if (UNIX AND NOT APPLE)
  target_link_libraries(test_core_ipc_cond1 rt)
endif ()

find_package(HDF5 COMPONENTS CXX REQUIRED)
target_include_directories(test_core_hdf5 PRIVATE ${HDF5_INCLUDE_DIRS})
target_link_libraries(test_core_hdf5 PRIVATE ${HDF5_LIBRARIES})