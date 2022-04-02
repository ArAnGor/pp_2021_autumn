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
  Matrix M = sequentialMultiplication(A, B, 2, 2, 2);
  //printf("%d %d\n%d %d\n", M[0][0], M[0][1], M[1][0], M[1][1]);
  ASSERT_EQ(M, C);
}

TEST(Parallel_Operations_MPI, Sequential_Rectangle) {
  Matrix A = {
    { 1, 2, 3 },
    { 4, 0, 5 },
    { 6, 7, 8 }
  };
}

TEST(Parallel_Operations_MPI, Parallel_Square) {

}

TEST(Parallel_Operations_MPI, Parallel_Rectangle) {

}

TEST(Parallel_Operations_MPI, Identity_Matrix) {

}

TEST(Parallel_Operations_MPI, Zero_Matrix) {

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
