#include <fcntl.h>     // open
#include <unistd.h>    // read, write, lseek, close
#include <string.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    char * nome = "numeri.txt";
    if (strstr(nome, ".gz") != NULL)
    {
        printf("Trovata\n");
    }else{
        printf("Non trovata\n");
    }
    
    return 0;
}
