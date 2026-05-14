#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

void createStack(struct stack * s){
    //inizializzo dicendo che è vuoto
    s->top = -1;
}

void clearStack(struct stack * s){
    while(s->top != -1){
        struct stackElement el = pop(s);

        if(el.tipo == TENSOR_T){
            deleteTensor(el.data.t);
        }
        //se è una stringa non serve che faccio nientet perché tanto è statica
    }
}

//devo fare due funzioni divise per aggiungere un tensore e una stringa
void pushTensor(struct tensor * t, struct stack * s){
    if(s->top == MAX_STACK - 1){//se è pieno
        printf("Impossibile fare il push del tensore, stack pieno\n");
        exit(1);
    }
    //se sono qua non è pieno
    s->top++;
    //dico che questo elemento che aggiungo è di tipo tensore
    s->elementi[s->top].tipo = TENSOR_T;
    //scrivo nel campo t della union
    s->elementi[s->top].data.t = t;
}

void pushString(char * stringa, struct stack * s){
    if(s->top == MAX_STACK - 1){//se è pieno
        printf("Impossibile fare il push della stringa, stack pieno\n");
        exit(1);
    }

    s->top++;
    //dico che è di tipo stringa
    s->elementi[s->top].tipo = STRING_T;
    //scrivo nel campo str della union
    s->elementi[s->top].data.str = stringa;
}

//per il pop invece devo fare una funzione unica perche quando la chiamerò ancora non so che elemento troverò
struct stackElement pop(struct stack * s){
    if(s->top == -1){
        printf("Stack vuoto\n");
        exit(1);
    }
    //se sono qua non è vuoto
    
    struct stackElement elemento = s->elementi[s->top];
    //a questo punto già dentro alla struct stackelement si saprà cos è
    s->top--;
    return elemento;
}