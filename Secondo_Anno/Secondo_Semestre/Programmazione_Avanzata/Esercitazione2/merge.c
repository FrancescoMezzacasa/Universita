#include <stdio.h>
#include "merge.h"

void merge(int * v1, int n1, int * v2, int n2, int * results)
{
  int i = 0;
  int j = 0;
  int k = 0;
  while(i < n1 && j < n2){
    if(v1[i] < v2[j]){
      results[k] = v1[i];
      i++;
    }else{
      results[k] = v2[j];
      j++;
    }
    k++;
  }
  //vedo se i vettori non sono ancora finiti
  
  while(i < n1){//lo fa se ci sono ancora elementi nel primo array
    results[k] = v1[i];
    k++;
    i++;
  }
  while(j < n2){
    results[k] = v2[j];
    k++;
    j++;
  }
  return;
}

void merge_branchless(int * v1, int n1, int * v2, int n2, int * results)
{
  int i = 0;
  int j = 0;
  int k = 0;
  int p;
  while(i < n1 && j < n2){
    p = (v1[i] < v2[j]);
    //p sarà 0 o 1 in base alla verità della condizione
    results[k] = v1[i]*p + v2[j]*(1-p);
    k++;
    //devo trovare un modo di incrementare solo i o j in base a quella che ho usato
    i = i + p;
    j = j + (1 - p);
  }
  //vedo se i vettori non sono ancora finiti
  
  while(i < n1){//lo fa se ci sono ancora elementi nel primo array
    results[k] = v1[i];
    k++;
    i++;
  }
  while(j < n2){
    results[k] = v2[j];
    k++;
    j++;
  }
  return;
}
