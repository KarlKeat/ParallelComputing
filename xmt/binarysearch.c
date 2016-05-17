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
  return -1;
}

int main(int argc, char** argv)
{
  int val = atoi(argv[1]);
  int array[] = {1, 3, 5, 7, 9, 11, 13};
  int search = binarySearch(array, sizeof(array)/sizeof(int), val);
  printf("%d\n", search);
}
