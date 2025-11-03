#include <stdio.h>

int main (int argc, char *argv[]) {

    if(argc != 2){
        printf("Inserire come argomento il nome del file!\n");
        return 1;
    }

    FILE * file;
    char *nome = argv[1];
    char contenuto[100];

    file = fopen(nome, "w");
    if(file == NULL){
        perror("Errore: ");
        return 1;
    }
    fputs("Ciao questa è una linea aggiunta\n", file);
    fputs("Ancora una linea\n", file);

    fclose(file);

    fopen(nome, "r");
    
    while(fgets(contenuto, 100, file)){
        printf("%s", contenuto);
    }
    
    
    
    fclose(file);
    return 0;
}