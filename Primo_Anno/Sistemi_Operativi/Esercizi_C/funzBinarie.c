#include <stdio.h>

int main (int argc, char *argv[]) {
    FILE * file;
    file = fopen("file/binario.txt", "rb+");
    if(file == NULL){
        perror("Errore: ");
        return 1;
    }

    int date[2] = {1990, 2025};
    int lettura[2];

    fwrite(date, sizeof(int), 2, file); //ora il cursore è alla fine, per leggere devo riposizionarlo
    fseek(file, 0, SEEK_SET); //torna all'inizio
    fread(lettura, sizeof(int), 2, file); //adesso puoi leggere
    for (int i = 0; i < 2; i++)
    {
        printf("%d ", lettura[i]);
    }
    printf("\n");
    fclose(file);
    return 0;
}