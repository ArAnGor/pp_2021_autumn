#include <mpi.h>
#include <random>
#include "../../../modules/task_2/gorbachev_a_ribbon_vertical/ribbon_vertical.h"

Matrix& getRandomMatrix(int m, int n)
{
  std::random_device dev;
  std::mt19937 gen(dev());
  Matrix M(m);
  for (int i = 0; i < m; i++)
    for (int j = 0; j < n; j++)
      M[i][j] = gen();
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

Matrix& parallelMultiplication(const Matrix& A,const Matrix& B)
{
  int n = A[0].size();
  if (n != B.size())
    throw "size mismatch";
  int m = A.size(), k = B[0].size();
  Matrix M;
  return M;
}
