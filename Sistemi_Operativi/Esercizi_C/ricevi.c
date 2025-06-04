//fatto da ChatGPT per testare periodic-kill
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void handler(int sig) {
    printf("PID %d ha ricevuto segnale %d\n", getpid(), sig);
    fflush(stdout); // importante per vedere subito l'output
}

int main() {
    signal(SIGUSR1, handler);
    signal(SIGTERM, handler);
    signal(SIGINT, handler);
    // Aggiungi altri segnali se vuoi

    printf("In attesa... PID: %d\n", getpid());

    while (1) {
        pause(); // aspetta un segnale
    }

    return 0;
}
