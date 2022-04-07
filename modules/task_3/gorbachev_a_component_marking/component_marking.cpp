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
    image[i] = gen() % 2;
  return image;
}

int sequentialMarking(int* image, int x, int y)
{
  int count = 0;
  int top, left;
  struct Equivalence {
    int from;
    int to;
  };
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
          if (top != left) {
            if (top < left)
              eqv.push_back({ left, top });
            else
              eqv.push_back({ top, left });

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

  count -= eqv.size();
  return count;
}

int parallelMarking(int* image, int x, int y)
{
  return 0;
}
