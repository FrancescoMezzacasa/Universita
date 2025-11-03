#include <stdio.h>

int quadrato(int x){
    return x*x;
}

int* applica(int *a, int dim, int(* funz)(int)){

    for (int i = 0; i < dim; i++)
    {
        a[i] = (*funz)(a[i]);
    }
    return a;
}

int main (int argc, char *argv[]) {
    int a[10];
    for (int i = 0; i < 10; i++){
        a[i] = i + 1;
    }
    applica(a, 10, quadrato);
    for (int i = 0; i < 10; i++){
        printf("%d ", a[i]);
    }
    printf("\n");
    return 0;
}