#include <stdio.h>

int main (int argc, char *argv[]){
    int n;
    printf("Inserisci il numero: ");
    scanf("%d", &n);

    FILE * file;
    file = fopen("file/numero.txt", "w");
    if(file == NULL){
        perror("Errore: ");
        return 1;
    }

    for(int i = 1; i <= n; i++){
        fprintf(file, "%d\n", i);
    }
    fclose(file);
    return 0;
}