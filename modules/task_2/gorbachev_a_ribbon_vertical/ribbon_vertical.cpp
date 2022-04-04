// Copyright 2022 Gorbachev Artem
#include <mpi.h>
#include <random>
#include "../../../modules/task_2/gorbachev_a_ribbon_vertical/ribbon_vertical.h"

Matrix& getZeroMatrix(int m, int n)
{
  Matrix& M = *new Matrix(m);
  for (int i = 0; i < m; i++)
    for (int j = 0; j < n; j++)
      M[i].push_back(0);
  return M;
}

Matrix& getRandomMatrix(int m, int n)
{
  std::random_device dev;
  std::mt19937 gen(dev());
  Matrix& M = *new Matrix(m);
  for (int i = 0; i < m; i++)
    for (int j = 0; j < n; j++)
      M[i].push_back(gen() % 10);
  return M;
}

Matrix& sequentialMultiplication(const Matrix& A, const Matrix& B)
{
  int n = A[0].size();
  if (n != B.size())
    throw "size mismatch";
  int m = A.size(), k = B[0].size();

  Matrix& M = *new Matrix(m);
  for (int i = 0; i < m; i++)
    for (int j = 0; j < k; j++) {
      M[i].push_back(0);
      for (int t = 0; t < n; t++)
        M[i][j] += A[i][t] * B[t][j];
    }
  return M;
}

Matrix& parallelMultiplication(const Matrix& A, const Matrix& B)
{
  int procNum, procRank;
  MPI_Comm_size(MPI_COMM_WORLD, &procNum);
  MPI_Comm_rank(MPI_COMM_WORLD, &procRank);

  int n, m, k;
  if (procRank == 0) {
    n = A[0].size();
    if (n != B.size())
      throw "size mismatch";
    m = A.size();
    k = B[0].size();
  }

  MPI_Bcast(&m, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&k, 1, MPI_INT, 0, MPI_COMM_WORLD);

  Matrix& M = getZeroMatrix(m, k);
  Matrix T = M;

  int portion = n / procNum;
  int remains = n % procNum;

  if (procRank == 0) {
    if (procNum > n)
      throw "too much processes";

    // if there is unallocated data,
    // for each column of matrix A
    for (int t = n - remains; t < n; t++)
      // for each element of this column
      for (int i = 0; i < m; i++)
        // for each element of the corresponding row of matrix B
        for (int j = 0; j < k; j++)
          // do multiplication (and addition if many columns)
          T[i][j] += A[i][t] * B[t][j];
  }

  // send columns of matrix A
  Matrix tmpA = getZeroMatrix(m, portion);
  int* sendbuf = new int[m*n];
  if (procRank == 0)
    for (int i = 0; i < m; i++)
      for (int j = 0; j < n; j++)
        sendbuf[i*n + j] = A[i][j];
  for (int i = 0; i < m; i++)
    MPI_Scatter(&sendbuf[i*n], portion, MPI_INT,
             tmpA[i].data(), portion, MPI_INT, 0, MPI_COMM_WORLD);
  
  int* tmpB = new int[n*k];
  if (procRank == 0)
    for (int i = 0; i < n; i++)
      for (int j = 0; j < k; j++)
        tmpB[i*k + j] = B[i][j];
  MPI_Bcast(tmpB, n*k, MPI_INT, 0, MPI_COMM_WORLD);
  
  for (int t = 0; t < portion; t++)
    for (int i = 0; i < m; i++)
      for (int j = 0; j < k; j++)
        T[i][j] += tmpA[i][t] * tmpB[(portion*procRank + t)*k + j];
  
  for (int i = 0; i < m; i++)
    MPI_Reduce(T[i].data(), M[i].data(), k, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
  return M;
}
