#include <stdlib.h>
#include <stdio.h>

#include "unrolled_linked_list.h"

unrolled_linked_list ulst_make(void)
{
  unrolled_linked_list lista = (unrolled_linked_list)malloc(sizeof(unrolled_linked_list));
  lista->head = NULL;
  return lista;
}

void ulst_delete(unrolled_linked_list lst)
{
  if(lst == NULL)
    return;
  unode cur = lst->head;
  unode prev;
  while(cur != NULL){
    prev = cur;
    cur = cur->next;
    free(prev);
  }
  free(lst);
}

void ulst_add(unrolled_linked_list lst, int key)
{
  if(lst == NULL)
    return;
  unode testa = lst->head;
  //contollo che testa non sia NULL (devo inserire il primo elemento, lista vuota)
  if(testa != NULL){
    for(int i = 0; i < UNROLLED_SIZE; i++){
      if(testa->valid[i] == false){
        testa->keys[i] = key;
        testa->valid[i] = true;
        return;
      }
    }
  }
  //se sono arrivato qua vuol dire che sono tutti occupati
  //oppure che la lista è vuota e devo inserire il primo
  unode nodo = (unode)malloc(sizeof(struct _unrolled_node));
  nodo->keys[0] = key;
  nodo->valid[0] = true;
  nodo->next = lst->head;
  lst->head = nodo;
}

bool ulst_search(unrolled_linked_list lst, int key)
{
    if(lst == NULL)
      return false;
    unode cur = lst->head;
    while(cur != NULL){
      for(int i = 0; i < UNROLLED_SIZE; i++){
        if(cur->valid[i] == true && cur->keys[i] == key)
          return true;
      }
      cur = cur->next;
    }
    return false;
}

void ulst_print(unrolled_linked_list lst)
{
  if (lst == NULL) {
    printf("NIL");
    return;
  }
  printf("(");
  unode current = lst->head;
  while (current != NULL) {
    printf("[");
    for (int i = 0; i < UNROLLED_SIZE; i++) {
      if (current->valid[i]) {
	printf("%d", current->keys[i]);
      } else {
	printf(".");
      }
      if (i < UNROLLED_SIZE - 1) {
	printf(" ");
      }
    }
    printf("]");
    if (current->next != NULL) {
      printf(" ");
    }
    current = current->next;
  }
  printf(")");
}
