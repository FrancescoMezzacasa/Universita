#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
void handler(int segnale){
    char risposta;
    printf("\n");
    while(1){
        printf("Vuoi terminare il programma? ");
        scanf(" %c", &risposta);
        if(risposta == 'Y'){
            exit(0);//se dice si finisci tutto
        }
        else if(risposta == 'N'){
            return;//se dice no finisci solo la funzione
        }
    }
}

int main (int argc, char *argv[]) {
    //per inviare sigkill mi basta fare kill
    //per quando preme ctrl+c mi serve la funzione handler

    int pid;

    struct sigaction newaction = {0};
    newaction.sa_handler = handler;
    newaction.sa_flags = 0;
    sigemptyset(&newaction.sa_mask);

    sigaction(SIGINT, &newaction, NULL);


    while(1){
        printf("Inserire il PID: ");
        scanf("%d", &pid);
        if(pid > 0)
            kill(pid, SIGKILL);
    }
    return 0;
}