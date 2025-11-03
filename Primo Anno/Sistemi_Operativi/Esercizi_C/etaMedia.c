#include <stdio.h>

int main (int argc, char *argv[]) {
    int numero, conto = 0;
    float media, somma = 0;
    char nome[100];
    FILE * file;
    file = fopen("file/persone.txt", "r");
    if(file == NULL){
        perror("Errore: ");
        return 1;
    }

    while(fscanf(file, "%s %d\n", nome, &numero) != EOF){
        somma += numero;
        conto++;
    }
    media = somma/conto;
    printf("L'età media è: %f\n", media);
    fclose(file);
    return 0;
}