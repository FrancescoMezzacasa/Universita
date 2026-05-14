#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#include "ctree.h"

ctree make_cnode(int key, float val)
{
  ctree ct = (ctree)malloc(sizeof(struct _ctree_node));
  ct->key[0] = key;
  ct->val[0] = val;
  ct->first_free = 1;
  return ct;
}

void insertInOrder(int * keys, float * vals, int key, float val, int first_free, int pos){//pos è l'indice del primo piu grande di key
  for(int j = first_free-1; j >= pos; j--){
      keys[j+1] = keys[j];
      vals[j+1] = vals[j];
  }
  keys[pos] = key;
  vals[pos] = val;
}

ctree cinsert(ctree t, int key, float val)
{
  if(t == NULL){
    t = make_cnode(key, val);
    return t;
  }
  int i = 0;
  while(i < t->first_free && t->key[i] <= key){
    if(t->key[i] == key){
      t->val[i] = val;
      return t;
    }
    i++;
  }

  /*ora sono uscito o perche ho trovato uno più grande o perchè è finito lo spazio
  ma in ogni caso i mi dice dove dovrebbe andare il nuovo valore
  se il nodo ha ancora spazio lo inserisco a quell'indice, 
  se invece è pieno devo andare nel sottoalbero corrispondente (come la ricerca)*/

  if(t->first_free < N){//se non è pieno
    insertInOrder(t->key, t->val, key, val, t->first_free, i);
    t->first_free++;
    return t;
  }
  //ora se sono qui è perchè il nodo è pieno
  t->children[i] = cinsert(t->children[i], key, val);
  /*ho chiamato la insert sul figlio perche cosi se è vuoto me lo crea lui, 
  altrimenti fa solo la chiamata ricorsiva*/
  return t;
}

bool csearch(ctree t, int key, float * val)
{
  if(t == NULL)
    return false;
  int i = 0;
  while(i < t->first_free && t->key[i] <= key){
    if(t->key[i] == key){
      t->val[i] = *val;
      return true;
    }
    i++;
  }
  //quando sono qua sono uscito o perche ho finito l'array keys o perche ho trovato una chiave maggiore
  //verifico se i è ancora minore di firstfree cosi so che sono nel secondo caso
  if(i < t->first_free){
    return(csearch(t->children[i], key, val));
  }
  //se sono qua vuol dire che ho keys pieno e la chiave maggiore di tutte
  //quindi devo cercare in quello piu a destra di tutti
  return(csearch(t->children[N], key, val));
}

void print_ctree(ctree t)
{
  if(t == NULL){
    printf(".");
    return;
  }
  printf("(");
  for(int i = 0; i < t->first_free; i++){
    print_ctree(t->children[i]);
    printf(" %d ", t->key[i]);
  }
  print_ctree(t->children[N]);
  printf(")");
}

