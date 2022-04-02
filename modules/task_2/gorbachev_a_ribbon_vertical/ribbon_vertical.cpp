#include <mpi.h>
#include <random>
#include "../../../modules/task_2/gorbachev_a_ribbon_vertical/ribbon_vertical.h"

std::vector<std::vector<int>>& getRandomMatrix(int m, int n)
{
  std::random_device dev;
  std::mt19937 gen(dev());
  std::vector<std::vector<int>> M(m);
  for (int i = 0; i < m; i++)
    for (int j = 0; j < n; j++)
      M[i][j] = gen();
  return M;
}

std::vector<std::vector<int>>& sequentialMultiplication(
  const std::vector<std::vector<int>>& A,
  const std::vector<std::vector<int>>& B,
  int m, int n, int k)
{
  std::vector<std::vector<int>> M(m);
  for (int i = 0; i < m; i++)
    for (int j = 0; j < k; j++) {
      M[i].push_back(0);
      for (int t = 0; t < n; t++)
        M[i][j] += A[i][t] * B[t][j];
    }
  return M;
}

std::vector<std::vector<int>>& parallelMultiplication(
  const std::vector<std::vector<int>>& A,
  const std::vector<std::vector<int>>& B,
  int m, int n, int k)
{
  // TODO: вставьте здесь оператор return
}
