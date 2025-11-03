#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>

int main (int argc, char *argv[]) {
    DIR * cartella;
    struct stat info;
    int maxsize = 0;
    struct dirent *elemento;
    char maxfile[1024];//qua ci va il nome del file massimo
    char path[1024];

    cartella = opendir(argv[1]);
    elemento = readdir(cartella);
    while(elemento != NULL){
        path[0] = 0;
        strcat(path, argv[1]);
        strcat(path, elemento->d_name);
        puts(path);
        stat(path, &info); //valorizzo la stat
        //ora vedo se è un file
        if(S_ISREG(info.st_mode)){
            //se è un file devo cercare il massimo
            if(info.st_size > maxsize){
                //devo anche vedere che non finisca con .gz
                if(strstr(elemento->d_name, ".gz") == NULL){//se non finisce con .gz
                    maxsize = info.st_size;
                    strcpy(maxfile, elemento->d_name);
                    //allora il massimo diventa lui
                }
            }
        }
        elemento = readdir(cartella);
    }
    //quando arrivo qua dovrei aver trovato il file massimo che non finisce con .gz
    char comando[1024];
    comando[0] = 0;
    strcat(comando, "gzip ");
    strcat(comando, argv[1]);
    strcat(comando, maxfile);
    printf("Eseguo %s\n", comando);

    system(comando);

    closedir(cartella);
    return 0;
}