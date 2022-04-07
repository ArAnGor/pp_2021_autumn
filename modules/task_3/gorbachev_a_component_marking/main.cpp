// Copyright 2022 Gorbachev Artem
#include <gtest/gtest.h>
#include "component_marking.h"
#include <gtest-mpi-listener.hpp>

TEST(Parallel_Operations_MPI, Sequential_Simple) {
  int image[] = {
    1, 0,
    0, 1
  };
  int expected[] = {
    1, 0,
    0, 2
  };
  int res = sequentialMarking(image, 2, 2);
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 2; j++)
      printf("%d ", image[i * 2 + j]);
    printf("\n");
  }
  ASSERT_EQ(res, 2);
  //ASSERT_EQ(image, expected);
}

TEST(Parallel_Operations_MPI, Sequential_Collision) {
  int image[] = {
    0, 1, 0, 1, 0, 1,
    1, 1, 0, 1, 0, 1,
    0, 0, 0, 1, 0, 1,
    1, 1, 1, 1, 1, 1
  };
  int expected[] = {
    0, 1, 0, 2, 0, 2,
    1, 1, 0, 2, 0, 2,
    0, 0, 0, 2, 0, 2,
    2, 2, 2, 2, 2, 2
  };
  int res = sequentialMarking(image, 6, 4);
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 6; j++)
      printf("%d ", image[i * 6 + j]);
    printf("\n");
  }
  ASSERT_EQ(res, 2);
  //ASSERT_EQ(image, expected);
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  MPI_Init(&argc, &argv);

  ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
  ::testing::TestEventListeners& listeners =
    ::testing::UnitTest::GetInstance()->listeners();

  listeners.Release(listeners.default_result_printer());
  listeners.Release(listeners.default_xml_generator());

  listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
  return RUN_ALL_TESTS();
}
