// Copyright 2022 Gorbachev Artem
#include <gtest/gtest.h>
#include "ribbon_vertical.h"
#include <gtest-mpi-listener.hpp>

TEST(Parallel_Operations_MPI, Sequential_Square) {
  Matrix A = {
    { 1, 2 },
    { 3, 4 }
  };
  Matrix B = {
    { 1, 2 },
    { 1, 0 }
  };
  Matrix C = {
    { 3, 2 },
    { 7, 6 }
  };
  Matrix M = sequentialMultiplication(A, B);
  ASSERT_EQ(M, C);
}

TEST(Parallel_Operations_MPI, Sequential_Rectangle) {
  Matrix A = {
    { 1, 2, 3 },
    { 4, 0, 5 }
  };
  Matrix B = {
    { 1, 2 },
    { 3, 4 },
    { 1, 0 }
  };
  Matrix C = {
    { 10, 10 },
    {  9,  8 }
  };
  Matrix M = sequentialMultiplication(A, B);
  ASSERT_EQ(M, C);
}

TEST(Parallel_Operations_MPI, Parallel_Square) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  Matrix A, B, C;
  const int size = 200;

  if (rank == 0) {
    A = getRandomMatrix(size, size);
    B = getRandomMatrix(size, size);
  }

  C = parallelMultiplication(A, B);

  if (rank == 0) {
    Matrix M = sequentialMultiplication(A, B);
    ASSERT_EQ(M, C);
  }
}

TEST(Parallel_Operations_MPI, Parallel_Rectangle) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  Matrix A, B, C;
  const int m = 200, n = 300, k = 200;

  if (rank == 0) {
    A = getRandomMatrix(m, n);
    B = getRandomMatrix(n, k);
  }

  C = parallelMultiplication(A, B);

  if (rank == 0) {
    Matrix M = sequentialMultiplication(A, B);
    ASSERT_EQ(M, C);
  }
}

TEST(Parallel_Operations_MPI, Identity_Matrix) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  Matrix A, B, C;
  const int size = 300;

  if (rank == 0) {
    A = getRandomMatrix(size, size);
    B = Matrix(size);
    for (int i = 0; i < size; i++)
      for (int j = 0; j < size; j++)
        B[i].push_back(i == j ? 1 : 0);
  }

  C = parallelMultiplication(A, B);

  if (rank == 0) {
    Matrix M = sequentialMultiplication(A, B);
    ASSERT_EQ(M, A);
    ASSERT_EQ(M, C);
  }
}

TEST(Parallel_Operations_MPI, Zero_Matrix) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  Matrix A, B, C;
  const int size = 300;

  if (rank == 0) {
    A = getRandomMatrix(size, size);
    B = getZeroMatrix(size, size);
  }

  C = parallelMultiplication(A, B);

  if (rank == 0) {
    Matrix M = sequentialMultiplication(A, B);
    ASSERT_EQ(M, B);
    ASSERT_EQ(M, C);
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
