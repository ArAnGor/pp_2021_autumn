// Copyright 2022 Gorbachev Artem
#include <gtest/gtest.h>
#include "component_marking.h"
#include <gtest-mpi-listener.hpp>

const int x = 20, y = 20;

TEST(Parallel_Operations_MPI, Sequential_Simple) {
  int image[] = {
    1, 0,
    0, 1
  };
  int res = sequentialMarking(image, 2, 2);
  ASSERT_EQ(res, 2);
}

TEST(Parallel_Operations_MPI, Sequential_Collision) {
  int image[] = {
    0, 1, 0, 1, 0, 1,
    1, 1, 0, 1, 0, 1,
    0, 0, 0, 1, 0, 1,
    1, 1, 1, 1, 1, 1
  };
  int res = sequentialMarking(image, 6, 4);
  ASSERT_EQ(res, 2);
}

TEST(Parallel_Operations_MPI, Parallel_Random) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int *sImage, *pImage;

  if (rank == 0) {
    pImage = getRandomBinary(x, y);
    int size = x * y;
    sImage = new int[size];
    for (int i = 0; i < size; i++)
      sImage[i] = pImage[i];
  }

  int res = parallelMarking(pImage, x, y);

  if (rank == 0) {
    int exp = sequentialMarking(sImage, x, y);
    for (int i = 0; i < y; i++) {
      for (int j = 0; j < x; j++) {
        printf("%4d", pImage[i * x + j]);
        ASSERT_EQ(pImage[i * x + j], sImage[i * x + j]);
      }
      printf("\n");
    }
    ASSERT_EQ(res, exp);
  }
}

TEST(Parallel_Operations_MPI, Parallel_Empty) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int *sImage, *pImage;

  if (rank == 0) {
    int size = x * y;
    sImage = new int[size];
    pImage = new int[size];
    for (int i = 0; i < size; i++)
      sImage[i] = pImage[i] = 0;
  }

  int res = parallelMarking(pImage, x, y);

  if (rank == 0) {
    int exp = sequentialMarking(sImage, x, y);
    ASSERT_EQ(res, exp);
    ASSERT_EQ(res, 0);
  }
}

TEST(Parallel_Operations_MPI, Parallel_Filled) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int *sImage, *pImage;

  if (rank == 0) {
    int size = x * y;
    sImage = new int[size];
    pImage = new int[size];
    for (int i = 0; i < size; i++)
      sImage[i] = pImage[i] = 1;
  }

  int res = parallelMarking(pImage, x, y);

  if (rank == 0) {
    int exp = sequentialMarking(sImage, x, y);
    ASSERT_EQ(res, exp);
    ASSERT_EQ(res, 1);
  }
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
