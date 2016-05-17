#include <omp.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

int binarySearch(int* arr, size_t len, int val)
{
  int index = len/2;
  int upper = len-1;
  int lower = 0;
  int x;
  for(x = 0; x < log2(len); x++)
  {
    if(arr[index] > val)
    {
      upper = index-1;
      index = lower + (upper - lower)/2;
    }
    else if(arr[index] < val)
    {
      lower = index+1;
      index = lower + (upper - lower)/2;
    }
    else
    {
      return index;
    }
  }
  return index;
}

int main(int argc, char** argv)
{
  int arrA[] = {1, 2, 6, 9, 10, 11, 12, 15};
  int arrB[] = {3, 4, 5, 7, 8, 13, 14, 16};
  int* arrC = (int*)malloc((sizeof(arrA)+sizeof(arrB)));
  int** rank = (int**)malloc(2*sizeof(int*));
  rank[0] = (int*)malloc(sizeof(arrA));
  rank[1] = (int*)malloc(sizeof(arrB));
  int i;
  omp_set_num_threads(sizeof(arrA)/sizeof(int));
  #pragma omp parallel for
  for(i = 0; i < sizeof(arrA)/sizeof(int); i++)
  {
    rank[0][i] = binarySearch(arrB, sizeof(arrB)/sizeof(int), arrA[i]);
    arrC[i+rank[0][i]] = arrA[i];
  }

  omp_set_num_threads(sizeof(arrA)/sizeof(int));
  #pragma omp parallel for
  for(i = 0; i < sizeof(arrB)/sizeof(int); i++)
  {
    rank[1][i] = binarySearch(arrA, sizeof(arrA)/sizeof(int), arrB[i]);
    arrC[i+rank[1][i]] = arrB[i];
  }

  for(i = 0; i < (sizeof(arrA)+sizeof(arrB))/sizeof(int); i++)
  {
    printf("%d ", arrC[i]);
  }
  puts("");
}
