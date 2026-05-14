#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "stack.h"
#include "interpreter.h" 

int main(int argc, char *argv[]) {
    srand(time(NULL));
    
    if (argc < 2) {//controllo che sia passato un argomento da riga di comando
        printf("Uso: ./tensorforth file_di_prova\n");
    }
    //creo lo stack su cui poi lavorerò sempre
    struct stack s;
    createStack(&s);

    //chiamo il parser sull'argomento dato da riga di comando
    interpreta(argv[1], &s);
 
    clearStack(&s);
    return 0;
}