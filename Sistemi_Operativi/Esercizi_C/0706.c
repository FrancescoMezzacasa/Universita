#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main (int argc, char *argv[]) {
    int lettura = open(argv[1], O_RDONLY);
    int scrittura = open(argv[2], O_RDWR);
    
    int lunghezza = lseek(lettura, 0, SEEK_END);//mi ritorna la lunghezza del file
    char letto;

    //devo far scorrere il cursore del primo finche torna all inizio
    for(int i = lunghezza - 1; i >= 0; i--){
        lseek(lettura, i, SEEK_SET);
        read(lettura, &letto, 1);
        write(scrittura, &letto, 1);
    }
    close(lettura);
    close(scrittura);

    return 0;
}