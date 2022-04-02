// Copyright 2022 Gorbachev Artem
#ifndef MODULES_TASK_1_GORBACHEV_A_RIBBON_VERTICAL_RIBBON_VERTICAL_H_
#define MODULES_TASK_1_GORBACHEV_A_RIBBON_VERTICAL_RIBBON_VERTICAL_H_

#include <vector>

std::vector<std::vector<int>>& getRandomMatrix(int m, int n);
std::vector<std::vector<int> >& sequentialMultiplication(
  const std::vector<std::vector<int> >& A,
  const std::vector<std::vector<int> >& B,
  int m, int n, int k);
std::vector<std::vector<int> >& parallelMultiplication(
  const std::vector<std::vector<int> >& A,
  const std::vector<std::vector<int> >& B,
  int m, int n, int k);

#endif  // MODULES_TASK_1_GORBACHEV_A_RIBBON_VERTICAL_RIBBON_VERTICAL_H_
