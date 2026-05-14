#include <stdio.h>
//provo a fare la funzione che shifta gli elementi piu grandi a destra di uno
void insertInOrder(int * keys, int key, int first_free, int pos){//pos è l'indice del primo piu grande di key
    for(int j = first_free-1; j >= pos; j--){
        keys[j+1] = keys[j];
    }
    keys[pos] = key;
}

int main (int argc, char *argv[]) {
    int a[] = {2, 4, 5, 11, 12, -1, -1, -1};
    int x = 14;
    insertInOrder(a, x, 5, 5);
    for(int i = 0; i < 8; i++){
        printf("%d ", a[i]);
    }
    printf("\n");
    return 0;
}