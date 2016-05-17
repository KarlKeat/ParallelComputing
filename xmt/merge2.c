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

void printArr(int* arr, size_t size)
{
  int i;
  for(i = 0; i < size; i++)
  {
    printf("%d ", arr[i]);
  }
  puts("");
}

int main(int argc, char** argv)
{
  //int arrA[] = {1, 2, 3, 4, 5, 6, 7, 8};
  //int arrB[] = {9, 10, 11, 12, 13, 14, 15, 16};
  int arrA[] = {1, 2, 6, 9, 10, 11, 12, 15};
  int arrB[] = {3, 4, 5, 7, 8, 13, 14, 16};


  int* arrC = (int*)malloc((sizeof(arrA)+sizeof(arrB)));
  int* rank = (int*)malloc(sizeof(arrA));
  int n = (int)(sizeof(arrA)/sizeof(int));
  int count = 0;
  int i, x;
  //omp_set_num_threads((int)log2(n));
  //#pragma omp parallel for
  for(i = 0; i < n; i += (int)(n/log2(n)))
  {
    rank[count] = binarySearch(arrB, n, arrA[i]);
    arrC[i + rank[count]] = arrA[i];
    count++;
    rank[count] = binarySearch(arrA, n, arrB[i]);
    arrC[i + rank[count]] = arrB[i];
    count++;
  }

  for(i = 0; i < (sizeof(arrA)+sizeof(arrB))/sizeof(int); i++)
  {
    printf("%d ", arrC[i]);
  }
  puts("");

  int posA = 0;
  int posB = 0;
  for (i = 0; i < count; i++)
  {
    int lowerIndex = rank[i];
    int currIndex = lowerIndex;

    while(arrC[currIndex] == 0 || currIndex < 16)
    {
      //printf("%d %d %d\n", posA, posB, currIndex);
      //printArr(arrC, (sizeof(arrA)+sizeof(arrB))/sizeof(int));
      if(posA > 7 && posB > 7)
      {
        break;
      }
      if(posB > 7 || (arrA[posA] < arrB[posB] && posA < 8))
      {
        arrC[currIndex] = arrA[posA];
        posA++;
        currIndex++;
      }
      else if(posA > 7 || (arrB[posB] < arrA[posA] && posB < 8))
      {
        arrC[currIndex] = arrB[posB];
        posB++;
        currIndex++;
      }
    }
    if(arrC[currIndex] == arrA[posA] && posA < 8)
    {
      posA++;
    }
    else if(arrC[currIndex] == arrB[posB] && posB < 8)
    {
      posB++;
    }
    else
      break;
  }


  for(i = 0; i < (sizeof(arrA)+sizeof(arrB))/sizeof(int); i++)
  {
    printf("%d ", arrC[i]);
  }
  puts("");
}
