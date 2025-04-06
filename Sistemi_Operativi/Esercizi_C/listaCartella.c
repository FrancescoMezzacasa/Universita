#include <stdio.h>
#include <sys/stat.h>
#include <dirent.h>

int main (int argc, char *argv[]) {
    if(argc != 2){
        printf("Inserire il path nel terminale\n");
        return 1;
    }
    char *path = argv[1];
    DIR * cartella;
    cartella = opendir(path);
    struct dirent *elemento;

    elemento = readdir(cartella);
    while(elemento != NULL){
        printf("%s\n", elemento ->d_name);
        elemento = readdir(cartella);
    }
    closedir(cartella);
    return 0;
}