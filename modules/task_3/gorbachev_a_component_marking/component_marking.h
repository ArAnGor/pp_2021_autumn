// Copyright 2022 Gorbachev Artem
#ifndef MODULES_TASK_3_GORBACHEV_A_COMPONENT_MARKING_COMPONENT_MARKING_H_
#define MODULES_TASK_3_GORBACHEV_A_COMPONENT_MARKING_COMPONENT_MARKING_H_

int* getRandomBinary(int x, int y);
void closeGaps(int* image, int size);

struct Equivalence {
  int from;
  int to;
};
void addToTable(std::vector<Equivalence>& eqv, int a, int b);

int sequentialMarking(int* image, int x, int y);
int parallelMarking(int* image, int x, int y);

#endif // MODULES_TASK_3_GORBACHEV_A_COMPONENT_MARKING_COMPONENT_MARKING_H_