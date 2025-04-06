#include <stdio.h>

int somma(int x, int y){
    return x + y;
}
int prodotto(int x, int y){
    return x * y;
}

void combinaStampa(int x, int y, int(* funz)(int, int)){
    int f = (*funz)(x, y); //la dereferenzio = la applico
    printf("Combinazione: %d\n", f);
}
int main (int argc, char *argv[]) {
    combinaStampa(3, 4, somma);
    combinaStampa(3, 4, prodotto);
    return 0;
}