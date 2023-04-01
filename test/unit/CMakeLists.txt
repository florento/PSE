
set(gtest ${CMAKE_CURRENT_SOURCE_DIR})

set(gtest_src
  ${gtest}/TestUtil.cpp
  ${gtest}/TestSegmentDrum.cpp
  ${gtest}/TestSWTA.cpp
  ${gtest}/TestSegmentInterval.cpp
  ${gtest}/TestInterval.cpp
  ${gtest}/TestMTUList.cpp
  ${gtest}/TestCountingWeight.cpp
  ${gtest}/TestTropical.cpp
  ${gtest}/TestRational.cpp
  ${gtest}/TestViterbiWeight.cpp
  ${gtest}/TestCountingSWTA.cpp
  ${gtest}/TestLearningEnv.cpp
  ${gtest}/TestTransition.cpp
  ${gtest}/TestJamMono.cpp
  ${gtest}/TestCWD.cpp
  ${gtest}/TestUtil.cpp
  ${gtest}/TestWeight.cpp
  ${gtest}/TestSegment.cpp
  ${gtest}/TestMTU.cpp
  ${gtest}/TestSymbLabel.cpp
  ${gtest}/TestScoreBuilder.cpp
  ${gtest}/TestSegmentMono.cpp
  ${gtest}/TestJamDrum.cpp
  ${gtest}/TestTempoCst.cpp
PARENT_SCOPE
)



