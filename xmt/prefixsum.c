#include <omp.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>


int main()
{
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

  int x, y;
//Instantiate tree
  int** binaryTree = (int**)malloc(height * sizeof(int*));
  for(x = 0; x < height; x++)
  {
    binaryTree[x] = (int*)malloc(num * sizeof(int));
  }

  int nums[8] = {3, 1, 4, 1, 5, 9, 2, 6};

  int numvalues = num;

  for(x = 0; x < num; x++)
  {
    binaryTree[height-1][x] = nums[x];
  }
  int numnodes = num;
  for(x = height-2; x >= 0; x--)
  {
    omp_set_num_threads(numnodes);
    #pragma omp parallel
    for(y = 0; y < pow(2,x); y++)
    {
      int sum;
      sum = binaryTree[x+1][2*y] + binaryTree[x+1][2*y+1];
      binaryTree[x][y] = sum;
    }
    numnodes /= 2;
  }

  int** prefixsums = (int**)malloc(height * sizeof(int*));
  for(x = 0; x < height; x++)
  {
    prefixsums[x] = (int*)malloc(num * sizeof(int));
  }
  numnodes = 1;
  for(x = 0; x < height; x++)
  {
    omp_set_num_threads(numnodes);
    #pragma omp parallel
    for(y = 0; y < pow(2, x); y++)
    {
      if(y == 0)
        prefixsums[x][y] = binaryTree[x][y];
      else if(y % 2 == 1)
        prefixsums[x][y] = prefixsums[x-1][(y-1)/2];
      else
        prefixsums[x][y] = prefixsums[x-1][y/2-1] + binaryTree[x][y];
    }
    numnodes *= 2;
  }

  for(x = 0; x < height; x++)
  {
    for(y = 0; y < pow(2, x); y++)
      printf("%d ", binaryTree[x][y]);
    printf("\n");
  }
  puts("");
  for(x = 0; x < height; x++)
  {
    for(y = 0; y < pow(2, x); y++)
      printf("%d ", prefixsums[x][y]);
    printf("\n");
  }
  puts("");
/**
  for(x = (int)pow(2, height) - 1; x < (int)pow(2, height) + num; x++)
  {
    printf("%d ", prefixsums[x]);
  }
  printf("\n");
**/

  return 0;
}
