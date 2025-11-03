#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>

#define MAXLINE 1024

int lineCount(char * path){
    FILE * file = fopen(path, "r");
    int count = 0;
    char buffer[MAXLINE];

    while(fgets(buffer, MAXLINE, file) != NULL){
        count++;
    }
    fclose(file);
    return(count);
}

int main (int argc, char *argv[]) {
    int nfile = argc - 1;


    for(int i = 1; i < argc; i++){
        if(fork() != 0){
            continue;
        }else{//se sei il figlio
            char writefile_name[1024];
            strcpy(writefile_name, argv[i]);
            strcat(writefile_name, ".count");
            FILE * writefile = fopen(writefile_name, "w");

            int righe = lineCount(argv[i]);

            fprintf(writefile, "%d", righe);

            fclose(writefile);
            exit(0);
        }
    }

    for(int i = 0; i < nfile; i++){
        wait(NULL);
    }
    printf("Operazione avvenuta con successo\n");
    return 0;
}