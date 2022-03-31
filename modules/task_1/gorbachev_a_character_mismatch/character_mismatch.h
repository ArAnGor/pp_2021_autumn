// Copyright 2022 Gorbachev Artem
#ifndef MODULES_TASK_1_GORBACHEV_A_CHARACTER_MISMATCH_CHARACTER_MISMATCH_H_
#define MODULES_TASK_1_GORBACHEV_A_CHARACTER_MISMATCH_CHARACTER_MISMATCH_H_

#include <string>

std::string getRandomString(int size);
int parallelCountMismatch(const std::string str1, const std::string str2);
int sequentialCountMismatch(const std::string str1, const std::string str2);

#endif  // MODULES_TASK_1_GORBACHEV_A_CHARACTER_MISMATCH_CHARACTER_MISMATCH_H_
