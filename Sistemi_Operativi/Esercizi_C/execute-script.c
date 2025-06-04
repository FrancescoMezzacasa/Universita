#include <stdio.h>
#include <stdlib.h>

int main (int argc, char *argv[]) {
    FILE * file = fopen(argv[1], "r");
    char comando[1024];

    while(fgets(comando, 1024, file) != NULL){
        puts(comando);
        int status = system(comando);
        if(status != 0){
            printf("Esco, status = %d\n", status);
            exit(1);
        }
    }
    return 0;
}