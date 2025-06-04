#include <stdio.h>
#include <unistd.h>
#include <wait.h>
#include <string.h>
#include <stdlib.h>

#define MAXLINES 256
#define MAXPATH 1024

void sumFile(char * path){
    FILE * file = fopen(path, "r");
    int somma = 0;
    int numero = 0;
    while(fscanf(file, "%d", &numero) != EOF){
        somma += numero;
    }
    printf("SOMMA:%s:%d\n", path, somma);
    fclose(file);
    return;
}

int main (int argc, char *argv[]) {
    int count = 0;
    int pid[MAXLINES];
    char path[MAXPATH];

    FILE * pathfile = fopen(argv[1], "r");

    while(fgets(path, MAXPATH, pathfile) != NULL){
        path[strcspn(path, "\n")] = 0;//tolgo il \n

        if(path[0] != 0){//se non è riga vuota
            pid[count] = fork();
            if(pid[count] == 0){
                //se sei il figlio
                sumFile(path);
                exit(0);
            }
            count++;
        }
    }
    //quando arrivo qua i processi dovrebbero aver calcolato le somme
    for (int i = 0; i < count; i++)
    {
        wait(NULL);
    }
    for (int i = 0; i < count; i++)
    {
        printf("%d\n", pid[i]);
    }
    
    fclose(pathfile);
    return 0;
}