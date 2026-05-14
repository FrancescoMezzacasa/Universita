#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "merge.h"

int cmpfunc (const void * a, const void * b) {
  return ( *(int*)a - *(int*)b );
}

int * random_vector(int len)
{
  int * v = (int *) malloc (len * sizeof(int));
  for (int i = 0; i < len; i++) {
    v[i] = rand();
  }
  return v;
}

float test_merge(int n, void (* m) (int *, int, int *, int, int *))
{
  int * v1 = random_vector(n);
  int * v2 = random_vector(n);
  int * results = (int *) malloc(2 * n * sizeof(int));
  qsort(v1, n, sizeof(int), cmpfunc);
  qsort(v2, n, sizeof(int), cmpfunc);
  clock_t start = clock();
  m(v1, n, v2, n, results);
  clock_t end = clock();
  float ms = (float) (end - start) / (CLOCKS_PER_SEC / 1000.0);
  free(v1);
  free(v2);
  free(results);
  return ms;
}

float avg_test_merge(int n,
		    void (* m) (int *, int, int *, int, int *),
		    int repetitions)
{
  float avg = 0;
  for (int i = 0; i < repetitions; i++) {
    avg += test_merge(n, m);
  }
  return avg / repetitions;
}

void check_merge(void (* m) (int *, int, int *, int, int *))
{
  int a[5] = {4, 6, 9, 17, 20};
  int b[5] = {2, 5, 7, 10, 18};
  int c[10];
  m(a, 5, b, 5, c);
  for (int i = 0; i < 10; i++) {
    printf("%d ", c[i]);
  }
  printf("\n");
}

int main(int argc, char * argv[])
{
  srand(time(NULL));
  check_merge(merge);
  check_merge(merge_branchless);

  int repetitions;
  float branchTime;
  float branchlessTime;

  printf("Inserire numero di ripetizioni: ");
  scanf("%d", &repetitions);
  
  printf("Dim.array:\tTempo con branch(ms):\tTempo branchless(ms):\tDifferenza branch/branchless:\n");
  for(int i = 1000; i <=20000; i += 1000){
    branchTime = avg_test_merge(i, merge, repetitions);
    branchlessTime = avg_test_merge(i, merge_branchless, repetitions);
    printf("%d\t\t%f\t\t%f\t\t%f\n", i, branchTime, branchlessTime, branchTime - branchlessTime);
  }

  return 0;
}
