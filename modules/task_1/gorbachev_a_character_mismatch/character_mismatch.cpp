// Copyright 2022 Gorbachev Artem
#include <mpi.h>
#include <string>
#include <random>
#include "../../../modules/task_1/gorbachev_a_character_mismatch/character_mismatch.h"

std::string getRandomString(int size) {
  std::random_device dev;
  std::mt19937 gen(dev());
  std::string s;
  for (int i = 0; i < size; i++)
    s += 32 + gen() % (128 - 32);
  return s;
}

int parallelCountMismatch(std::string str1, std::string str2) {
  if (str2.length() < str1.length()) {
    std::string buf = str1;
    str1 = str2;
    str2 = buf;
  }

  int res, procRank, procNum;
  const int length = str1.length();
  int portion, remains;
  int tmpRes = 0;

  MPI_Comm_size(MPI_COMM_WORLD, &procNum);
  MPI_Comm_rank(MPI_COMM_WORLD, &procRank);

  if (procRank == 0) {
    if (procNum > length)
      procNum = length;
    if (procNum == 1)
      remains = length;
    else {
      portion = length / (procNum - 1);
      remains = length % (procNum - 1);
      for (int i = 1; i < procNum; i++)
        MPI_Send(&portion, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
    }
    for (int i = length - remains; i < length; i++)
      if (str1[i] != str2[i])
        tmpRes++;
  }

  else {
    MPI_Status status;
    MPI_Recv(&portion, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    for (int i = portion * (procRank - 1); i < portion * procRank; i++)
      if (str1[i] != str2[i])
        tmpRes++;
  }

  MPI_Reduce(&tmpRes, &res, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
  res += str2.length() - str1.length();
  return res;
}

int sequentialCountMismatch(std::string str1, std::string str2) {
  int res = 0;
  if (str2.length() < str1.length()) {
    std::string buf = str1;
    str1 = str2;
    str2 = buf;
  }
  for (int i = 0; i < str1.length(); i++)
    if (str1[i] != str2[i])
      res++;
  res += str2.length() - str1.length();
  return res;
}
