include_directories(${cpp_INCLUDE_DIR})

if (SARA_BUILD_TESTS)
  include_directories(${Boost_INCLUDE_DIR})
  add_definitions(-DBOOST_ALL_NO_LIB)
endif ()

if (NOT WIN32)
  # add_subdirectory(Core)
  add_subdirectory(Compute)
  add_subdirectory(OpenCL)
endif ()

# add_subdirectory(Expression)
add_subdirectory(MatchPropagation)
add_subdirectory(NeuralNetworks)
add_subdirectory(Taskflow)
if (CUDA_FOUND AND NvidiaVideoCodec_ROOT)
  add_subdirectory(NvCodec)
endif ()

if (SARA_USE_HALIDE)
  add_subdirectory(Halide)
endif ()
