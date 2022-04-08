// Copyright 2022 Gorbachev Artem
#include <gtest/gtest.h>
#include "component_marking.h"
#include <gtest-mpi-listener.hpp>

TEST(Parallel_Operations_MPI, Sequential_Simple) {
  int image[] = {
    1, 0,
    0, 1
  };
  int res = sequentialMarking(image, 2, 2);
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 2; j++)
      printf("%d ", image[i * 2 + j]);
    printf("\n");
  }
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
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 6; j++)
      printf("%d ", image[i * 6 + j]);
    printf("\n");
  }
  ASSERT_EQ(res, 2);
}

TEST(Parallel_Operations_MPI, Parallel_Random) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int *sImage, *pImage;
  const int x = 10, y = 10;

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
      for (int j = 0; j < x; j++)
        printf("%3d", pImage[i * x + j]);
      printf("\n");
    }
    ASSERT_EQ(res, exp);
  }
}

TEST(Parallel_Operations_MPI, Parallel_Empty) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int sImage[16] = { 0 };
  int pImage[16] = { 0 };
  const int x = 4, y = 4;

  int res = parallelMarking(pImage, x, y);

  if (rank == 0) {
    int exp = sequentialMarking(sImage, x, y);
    for (int i = 0; i < y; i++) {
      for (int j = 0; j < x; j++)
        printf("%d ", pImage[i * x + j]);
      printf("\n");
    }
    ASSERT_EQ(res, exp);
    ASSERT_EQ(res, 0);
  }
}

TEST(Parallel_Operations_MPI, Parallel_Filled) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int *sImage, *pImage;
  const int x = 4, y = 4;

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
    for (int i = 0; i < y; i++) {
      for (int j = 0; j < x; j++)
        printf("%d ", pImage[i * x + j]);
      printf("\n");
    }
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
