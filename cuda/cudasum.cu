//#include <omp.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
__global__ void parallel1(int a, int** binaryTree, int** prefixsums)
{
    int b = threadIdx.x;
    int sum;
    sum = binaryTree[a+1][2*b] + binaryTree[a+1][2*b+1];
    binaryTree[a][b] = sum;
} 
__global__ void parallel2(int a, int** binaryTree, int** prefixsums) //pow(2, x) nodes
{
    int b = threadIdx.x;
    if(b == 0)
        prefixsums[a][b] = binaryTree[a][b];
    else if(b % 2 == 1)
        prefixsums[a][b] = prefixsums[a-1][(b-1)/2];
    else
        prefixsums[a][b] = prefixsums[a-1][b/2-1] + binaryTree[a][b];
}
int main()
{
  const int num = 8;

  int height;
  int binaryTreeSize = 0;
  if(num%2 == 0)
  {
      binaryTreeSize = 2*num - 1;
      height = (int)(log2((float)binaryTreeSize+2) + .5);
  }
  else
  {
      binaryTreeSize = 2*num;
      height = (int)(log2((float)binaryTreeSize+1) + .5);
  }

  printf("%d\n", height);

  int x, y;
  //Instantiate tree
  int** binaryTree;
  cudaMalloc(&binaryTree, height * sizeof(int*));
  for(x = 0; x < height; x++)
  {
      cudaMalloc(&binaryTree[x], num * sizeof(int));
  }
  int** prefixsums;
  cudaMalloc(&prefixsums, height * sizeof(int*));
  for(x = 0; x < height; x++)
  {
      cudaMalloc(&prefixsums[x], num * sizeof(int));
  }

  int nums[8] = {2, 4, 6, 3, 4, 1, 0, 2};

  for(x = 0; x < num; x++)
  {
    binaryTree[height-1][x] = nums[x];
  }
  for(x = height-2; x >= 0; x--)
  {
    parallel1<<<1, (int)pow(2,x)>>>(x, binaryTree, prefixsums); 
  }


  for(x = 0; x < height; x++)
  {
    //omp_set_num_threads(numnodes);
    //#pragma omp parallel
    parallel2<<<1, (int)pow(2, x)>>>(x, binaryTree, prefixsums);
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
