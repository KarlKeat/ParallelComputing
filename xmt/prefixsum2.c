#include <omp.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#define NUMNODES 4

int main()
{
  omp_set_num_threads(NUMNODES);
  int threadID;
  const int num = 8;

  int height;

  int binaryTreeSize = 0;
  if(num%2 == 0)
  {
    binaryTreeSize = 2*num - 1;
    height = (int)(log2(binaryTreeSize+2) + .5);
  }
  else
  {
    binaryTreeSize = 2*num;
    height = (int)(log2(binaryTreeSize+1) + .5);
  }

  printf("%d\n", height);

  int upperBound = binaryTreeSize;
  int* binaryTree = calloc(binaryTreeSize, sizeof(int));

  int nums[8] = {3, 1, 4, 1, 5, 9, 2, 6};

  int x;

  int summation = 0;
  for(x = 0; x < 8; x++)
  {
    summation += nums[x];
  }

  int numvalues = num;

  for(x = 0; x < num; x++)
  {
    binaryTree[binaryTreeSize-num+x] = nums[x];
  }
  while(upperBound != 1)
  {
    #pragma omp parallel
    for(x = 0; x < numvalues; x+=2)
    {
      int index = upperBound - numvalues + x;
      int sum;
      if(index + 1 < upperBound)
      {
        sum = binaryTree[index] + binaryTree[index+1];
      }
      else
      {
        sum = binaryTree[index];
      }
      binaryTree[index/2] = sum;
    }
    upperBound -= numvalues;
    numvalues /= 2;
  }
  int* prefixsums = calloc(binaryTreeSize, sizeof(int));
  for(x = 0; x < height; x++)
  {
    int lower = (int)pow(2, x)-1;
    int upper = (int)pow(2, x+1);
    int pos = lower;
    while(pos < upper)
    {
      if(pos == lower)
      {
        prefixsums[pos] = binaryTree[pos];
      }
      else if(pos%2 != 0)
      {
        printf("odd %d\n", (int)pow(2, x-1) + (pos - lower + 1));
        prefixsums[pos] = binaryTree[pos] + binaryTree[(int)pow(2, x-1) + (pos - lower - 1)];
      }
      else
      {
        prefixsums[pos] = binaryTree[(int)pow(2, x-1) + (pos - lower)];
      }
      pos++;
    }

  }

  for(x = 0; x < binaryTreeSize; x++)
  {
    printf("%d ", binaryTree[x]);
  }
  printf("\n%d\n", summation);

  for(x = (int)pow(2, height) - 1; x < (int)pow(2, height) + num; x++)
  {
    printf("%d ", prefixsums[x]);
  }
  printf("\n");


  return 0;
}
