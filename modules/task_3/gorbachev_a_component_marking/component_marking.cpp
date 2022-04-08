// Copyright 2022 Gorbachev Artem
#include <mpi.h>
#include <random>
#include <vector>
#include "../../../modules/task_3/gorbachev_a_component_marking/component_marking.h"

int* getRandomBinary(int x, int y)
{
  std::random_device dev;
  std::mt19937 gen(dev());
  int* image = new int[x * y];
  for (int i = 0; i < x * y; i++)
    image[i] = ((gen() % 5) == 0);
  return image;
}

void closeGaps(int* image, int size)
{
  std::vector<int> rename;
  for (int i = 0; i < size; i++)
    if (image[i]) {
      bool attend = false;
      for (int j = 0; j < rename.size(); j++)
        if (image[i] == rename[j]) {
          attend = true;
          image[i] = j + 1;
          break;
        }
      if (!attend) {
        rename.push_back(image[i]);
        image[i] = rename.size();
      }
    }
}

void addToTable(std::vector<Equivalence>& eqv, int a, int b)
{
  if (a < b)
    eqv.push_back({ b, a });
  else
    eqv.push_back({ a, b });

  // check relation with other equivalences
  for (int t = 0; t < eqv.size() - 1; t++) {
    if (eqv[t].from == eqv[eqv.size() - 1].from) {

      // there is a duplicate
      if (eqv[t].to == eqv[eqv.size() - 1].to)
        eqv.pop_back();

      // equal 'from' and different 'to'
      else {
        if (eqv[eqv.size() - 1].to < eqv[t].from)
          eqv[eqv.size() - 1].from = eqv[eqv.size() - 1].to;
        else
          eqv[eqv.size() - 1].from = eqv[t].to;
        if (eqv[eqv.size() - 1].to < eqv[t].to)
          eqv[t].to = eqv[eqv.size() - 1].to;
        else
          eqv[eqv.size() - 1].to = eqv[t].to;
      }
      break;
    }

    // double replacement
    else if (eqv[t].to == eqv[eqv.size() - 1].from ||
      eqv[t].from == eqv[eqv.size() - 1].to)
      if (eqv[eqv.size() - 1].to < eqv[t].to)
        eqv[t].to = eqv[eqv.size() - 1].to;
      else
        eqv[eqv.size() - 1].to = eqv[t].to;
  }
}

int sequentialMarking(int* image, int x, int y)
{
  int count = 0;
  int top, left;
  std::vector<Equivalence> eqv;

  // first row
  if (image[0])
    count++;
  for (int i = 1; i < x; i++)
    if (image[i])
      if (image[i - 1])
        image[i] = image[i - 1];
      else {
        count++;
        image[i] = count;
      }
  
  // other rows
  for (int i = 1; i < y; i++) {
    // first pixel
    if (image[x * i])
      if (image[x * (i - 1)])
        image[x * i] = image[x * (i - 1)];
      else {
        count++;
        image[x * i] = count;
      }
    // other pixels
    for (int j = 1; j < x; j++) 
      if (image[x * i + j]) {
        top = image[x * (i - 1) + j];
        left = image[x * i + j - 1];
        if (!top && !left) {
          count++;
          image[x * i + j] = count;
        }
        if (!top && left)
          image[x * i + j] = left;
        if (top && !left)
          image[x * i + j] = top;
        if (top && left) {
          image[x * i + j] = top;
          // collision
          if (top != left)
            addToTable(eqv, top, left);
        }
      }
  }
  
  //second pass
  int size = x * y;
  for (int i = 0; i < size; i++)
    for (int j = 0; j < eqv.size(); j++)
      if (image[i] == eqv[j].from) {
        image[i] = eqv[j].to;
        break;
      }

  closeGaps(image, size);

  count -= eqv.size();
  return count;
}

int parallelMarking(int* image, int x, int y)
{
  int procNum, procRank;
  MPI_Comm_size(MPI_COMM_WORLD, &procNum);
  MPI_Comm_rank(MPI_COMM_WORLD, &procRank);

  if (procNum > y)
    throw "too much processes";

  int portion = y / procNum;
  int remains = y % procNum;

  int tmpSize = portion * x;
  int* tmp = new int[tmpSize];
  MPI_Scatter(image, tmpSize, MPI_INT,
                tmp, tmpSize, MPI_INT, 0, MPI_COMM_WORLD);

  int count = sequentialMarking(tmp, x, portion);

  int* counts = new int[procNum];
  MPI_Allgather(&count, 1, MPI_INT, counts, 1, MPI_INT, MPI_COMM_WORLD);
  int extraCount = 0;
  for (int i = 0; i < procRank; i++)
    extraCount += counts[i];
  for (int i = 0; i < tmpSize; i++)
    if (tmp[i])
      tmp[i] += extraCount;

  if (procRank == 0) {
    if (remains) {
      int rcount = sequentialMarking(&image[tmpSize * procNum], x, remains);
      count += rcount;
      for (int i = 0; i < procNum; i++)
        extraCount += counts[i];
      int size = x * y;
      for (int i = tmpSize * procNum; i < size; i++)
        if (image[i])
          image[i] += extraCount;
    }
    for (int i = 1; i < procNum; i++)
      count += counts[i];
  }

  MPI_Gather(tmp, tmpSize, MPI_INT,
           image, tmpSize, MPI_INT, 0, MPI_COMM_WORLD);

  if (procRank == 0) {
    std::vector<Equivalence> eqv;
    for (int i = portion; i < y; i += portion)
      for (int j = 0; j < x; j++)
        if (image[i * x + j] && image[(i - 1) * x + j])
          addToTable(eqv, image[i * x + j], image[(i - 1) * x + j]);

    int size = x * y;
    for (int i = 0; i < size; i++)
      for (int j = 0; j < eqv.size(); j++)
        if (image[i] == eqv[j].from) {
          image[i] = eqv[j].to;
          break;
        }

    closeGaps(image, size);
    count -= eqv.size();
  }
  
  return count;
}
