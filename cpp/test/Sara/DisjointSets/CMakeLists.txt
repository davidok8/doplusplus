find_package(DO_Sara COMPONENTS DisjointSets REQUIRED)

file(GLOB test_disjoint_sets_SOURCE_FILES FILES *.cpp)

foreach (file ${test_disjoint_sets_SOURCE_FILES})
  get_filename_component(filename "${file}" NAME_WE)
  sara_add_test(
    NAME ${filename}
    SOURCES ${file}
    DEPENDENCIES ${DO_Sara_LIBRARIES}
    FOLDER DisjointSets)
endforeach ()
