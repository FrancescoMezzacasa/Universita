//FRANCESCO MEZZACASA SM3201587
#ifndef STACK_H
#define STACK_H

#include "tensor.h"
#define MAX_STACK 1024

//nello stack potrò inserirci o un tensore o una stringa, ma non so a priori quale
//mi serve un modo per poter avere entrambi i tipi e usare quello giusto
//solo quando mi serve, uso una tagged union

typedef enum{
    TENSOR_T,
    STRING_T
} tipoElemento; //questo mi dice che tipoElemento potrà essere solo uno tra quei due valori

struct stackElement{
    tipoElemento tipo;
    union {
        struct tensor * t;
        char * str;
    } data;
};
//cosi facendo in base al tipo deciderò se trattarlo come un tensore o una stringa

struct stack{
    int top; //indice della cima dello stack
    //ora ci metto un array di elementi dello stack (che sono delle struct)
    struct stackElement elementi[MAX_STACK];
};

void createStack(struct stack * s);
void clearStack(struct stack * s);
void pushTensor(struct tensor * t, struct stack * s);
void pushString(char * stringa, struct stack * s);
struct stackElement pop(struct stack * s);
#endif