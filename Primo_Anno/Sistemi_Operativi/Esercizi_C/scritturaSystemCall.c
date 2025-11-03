#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main (int argc, char *argv[]) {
    int fd = open("file/test.txt", O_RDWR | O_CREAT);
    if(fd == -1){
        perror("Errore: ");
        return 1;
    }
    char testo[] = "Voglio scrivere questo dentro al file\nsperiamo che lo faccia\n";
    char lettura[101];

    int lunghezza = strlen(testo);
    int scritti = write(fd, testo, lunghezza); //voglio vedere il valore di ritorno
    printf("Ho scritto %d byte e la stringa era lunga %d byte\n", scritti, lunghezza);

    lseek(fd, 0, SEEK_SET);
    int letti = read(fd, lettura, 100);
    lettura[letti] = '\0'; //per usarla come stringa vera devo aggiungere il terminatore
    printf("Ho letto %d byte\n", letti);
    printf("%s", lettura);

    close(fd);
    return 0;
}