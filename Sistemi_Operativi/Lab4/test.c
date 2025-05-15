#include <stdio.h>
#include <string.h>


int main (int argc, char *argv[]) {
    char comando[256];
    printf("Command: ");
    scanf("%s", comando);

    if(strcmp(comando, "quit") == 0){
        printf("Sono uguali\n");
    }else{
        printf("non sono uguali\n");
    }
    return 0;
}