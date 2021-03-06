# ==============================================================================
# Helper classes to perform studies with Mikolajczyk et al.'s dataset.
set(Miko_INC_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(Miko_INC MikolajczykDataset.hpp StudyOnMikolajczykDataset.hpp)
set(Miko_SRC MikolajczykDataset.cpp StudyOnMikolajczykDataset.cpp)
set(Miko_DEPS
  "${DO_Sara_LIBRARIES};DO::Sara::MatchPropagation")

sara_make_library("MikolajczykDataset"
  "${Miko_INC_DIR}"
  "${Miko_INC}"
  "${Miko_SRC}"
  "${Miko_DEPS}")


# ==============================================================================
# Validate empirically all the parameters.
set(Study_INC_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(Study_INC
  EvaluateQualityOfLocalAffineApproximation.hpp
  EvaluateOutlierResistance.hpp
  StudyAffineMetric.hpp
  StudyInfluenceOfMatchNeighborhoodSizeOnPerformance.hpp
  StudyParametersOfMatchNeighborhood.hpp
  StudyRegionFusion.hpp
  StudySeedTriple.hpp)
set(Study_SRC
  EvaluateQualityOfLocalAffineApproximation.cpp
  EvaluateOutlierResistance.cpp
  StudyAffineMetric.cpp
  StudyInfluenceOfMatchNeighborhoodSizeOnPerformance.cpp
  StudyParametersOfMatchNeighborhood.cpp
  StudyRegionFusion.cpp
  StudySeedTriple.cpp)
set(Study_DEPS
  "${DO_Sara_LIBRARIES};DO::Sara::MatchPropagation;DO::Sara::MikolajczykDataset")

sara_make_library("EmpiricalValidation"
  "${Study_INC_DIR}"
  "${Study_INC}"
  "${Study_SRC}"
  "${Study_DEPS}")
