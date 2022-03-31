// Copyright 2022 Gorbachev Artem
#include <gtest/gtest.h>
#include "character_mismatch.h"
#include <gtest-mpi-listener.hpp>

TEST(Parallel_Operations_MPI, Sequential_Equal_Size) {
  const std::string str1 = "aaaaaaaa";
  const std::string str2 = "bbbbaaaa";
  int res = sequentialCountMismatch(str1, str2);
  ASSERT_EQ(res, 4);
}

TEST(Parallel_Operations_MPI, Sequential_Different_Size) {
  const std::string str1 = "aaaaaaaa";
  const std::string str2 = "aabbabaabb";
  int res = sequentialCountMismatch(str1, str2);
  ASSERT_EQ(res, 5);
}

TEST(Parallel_Operations_MPI, Parallel_Equal_Size) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::string str1, str2;
  const int size = 80;

  if (rank == 0) {
    str1 = getRandomString(size);
    str2 = getRandomString(size);
  }

  int res = parallelCountMismatch(str1, str2);

  if (rank == 0) {
    int expected = sequentialCountMismatch(str1, str2);
    ASSERT_EQ(res, expected);
  }
}

TEST(Parallel_Operations_MPI, Parallel_Different_Size) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::string str1, str2;
  const int size = 80;

  if (rank == 0) {
    str1 = getRandomString(size);
    str2 = getRandomString(size*2);
  }

  int res = parallelCountMismatch(str1, str2);

  if (rank == 0) {
    int expected = sequentialCountMismatch(str1, str2);
    ASSERT_EQ(res, expected);
  }
}

TEST(Parallel_Operations_MPI, Equal_Strings) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::string str1, str2;
  const int size = 80;

  if (rank == 0) {
    str1 = getRandomString(size);
    str2 = str1;
  }

  int res = parallelCountMismatch(str1, str2);

  if (rank == 0) {
    int expected = sequentialCountMismatch(str1, str2);
    ASSERT_EQ(res, 0);
    ASSERT_EQ(res, expected);
  }
}

TEST(Parallel_Operations_MPI, Empty_Strings) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::string str1, str2;

  if (rank == 0) {
    str1 = getRandomString(0);
    str2 = str1;
  }

  int res = parallelCountMismatch(str1, str2);

  if (rank == 0) {
    int expected = sequentialCountMismatch(str1, str2);
    ASSERT_EQ(res, 0);
    ASSERT_EQ(res, expected);
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
