//dato un array e un numero x restituire un array con gli indici dei due numeri che sommati danno x

#include <stdio.h>
#include <stdlib.h>

int *cerca_indici(int a[], int dim, int x){
    int *ris = NULL;
    for (int i = 0;  i < dim - 1; i++){
        for(int j = i + 1; j < dim; j++){
            if(a[i] + a[j] == x){
                ris = (int *)malloc(sizeof(int) * 2);
                ris[0] = i;
                ris[1] = j;
            }
        }
    }
    return ris;
}

int main () {
    int dim;
    printf("Inserire la dimensione dell array: ");
    scanf("%d", &dim);

    int x;
    printf("Inserire il numero x: ");
    scanf("%d", &x);

    int *a = (int *)malloc(sizeof(int) * dim);

    for (int i = 0; i < dim; i++) {
        printf("Inserire l'elemento %d dell array: ", i);
        scanf("%d", &a[i]); 
    }
    int *ris = cerca_indici(a, dim, x);

    for (int i = 0; i < 2; i++) {
        printf("%d ", ris[i]);
    }

    return 0;
}