// Copyright 2022 Gorbachev Artem
#ifndef MODULES_TASK_2_GORBACHEV_A_RIBBON_VERTICAL_RIBBON_VERTICAL_H_
#define MODULES_TASK_2_GORBACHEV_A_RIBBON_VERTICAL_RIBBON_VERTICAL_H_

#include <vector>
#define Matrix std::vector<std::vector<int>>

Matrix& getZeroMatrix(int m, int n);
Matrix& getRandomMatrix(int m, int n);
Matrix& sequentialMultiplication(const Matrix& A, const Matrix& B);
Matrix& parallelMultiplication(const Matrix& A, const Matrix& B);

#endif  // MODULES_TASK_2_GORBACHEV_A_RIBBON_VERTICAL_RIBBON_VERTICAL_H_
