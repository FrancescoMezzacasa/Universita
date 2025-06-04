#include <fcntl.h>     // open
#include <unistd.h>    // read, write, lseek, close
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    char comando[1024];
    printf("Inserisci comando: ");
    scanf("\"%s\"", comando);
    int codice = system(comando);
    printf("Codice = %d\n", codice);
    return 0;
}
