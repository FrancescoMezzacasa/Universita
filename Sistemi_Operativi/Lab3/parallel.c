#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define MAXCMD 256
#define MAXPROCESSES 256

char *sostituisciComando(char * daSostituire,char * cosaSostituire,char * simboloDaTogliere, char * risultato){
    char padre[256];
    char figlio[256];
    char pattern[256];

    strcpy(padre, daSostituire);
    strcpy(figlio, cosaSostituire);
    strcpy(pattern, simboloDaTogliere);

    char pezzo_finale[256];
    char pezzo_iniziale[256];

    //pezzo finale = tutta la parte dopo il pattern (compreso)
    strcpy(pezzo_finale, strstr(padre, pattern));
    

    int lunghezza_pezzo_iniziale = strlen(padre)-strlen(pezzo_finale);

    //metto in pezzo_iniziale i caratteri di padre fino al pattern
    strncpy(pezzo_iniziale, padre, lunghezza_pezzo_iniziale);

    //null-terminated
    pezzo_iniziale[lunghezza_pezzo_iniziale] = '\0';

    //aggiungi la parte prima del pattern in risultato
    strcpy(risultato, pezzo_iniziale);

    //aggiungo a risultato la stringa da sostituire a %
    //ma senza l'ultimo carattere che è un \n dato che lo prendo come riga di un file
    strncat(risultato, figlio, strlen(figlio) - 1);

    int l = strlen(risultato);
    int j = 0;
    //for per aggiungere tutto quello che c'è dopo il pattern
    for (int i = lunghezza_pezzo_iniziale; i < strlen(padre); i++)
    {   
        if(padre[i] != '%'){
            risultato[l + j] = padre[i];
            j++;
        }
    }
    risultato[l+j] = '\0';
    
    return(risultato);
}

int main (int argc, char *argv[]) {
    if(argc != 4){
        printf("Usage: ./parallel.out file numero comando\n");
        return 1;
    }

    FILE * file;
    file = fopen(argv[1], "r");

    if(file == NULL){
        perror("Errore nell'apertura del file");
    }
    //creo l'array per contenere i pid dei figli
    int* figli = malloc(sizeof(__pid_t) * atoi(argv[2]));
    //creo l'array di pipe
    int pipes [MAXPROCESSES][2];
    //variabile in cui andrà ogni comando letto dal padre
    char buffer[MAXCMD];
    //variabile in cui va ogni comando letto dal figlio
    char comando [MAXCMD];
    //variabile in cui andrà il comando sostituito
    char comando_completo[MAXCMD];
    for(int i = 0; i < atoi(argv[2]); i++)
    {
        pipe(pipes[i]);
    }
    for(int i = 0; i < atoi(argv[2]); i++){
        figli[i] = fork();
        if(figli[i] == 0){//se è il figlio
            close(pipes[i][1]);//chiudo il file in scrittura
            //i figli conoscono anche tutte le altre pipe oltre a loro, quindi devo chiuderle
            for (int j = 0; j < atoi(argv[2]); j++)
            {
                if(j != i){
                    close(pipes[j][0]);
                    close(pipes[j][1]);
                }
            }
            
            //devo leggere il comando e poi fare la sostituzione in argv[3]
            while(read(pipes[i][0], comando, MAXCMD) > 0){
                //faccio la sostituzione in argv[3]
                sostituisciComando(argv[3], comando, "%", comando_completo);
                //ora faccio eseguire il comando al figlio
                system(comando_completo);
                //printf("Figlio %d ha eseguito: %s\n", getpid(), comando_completo);
            }
            //quando read ha ritornato 0 finisce
            //printf("Figlio %d termina.\n", getpid());
            close(pipes[i][0]);
            exit(0);
        } else{//se è il padre
            close(pipes[i][0]);//chiudo il file in lettura
        }

    }

    //leggo il file e passo la riga letta al figlio tramite pipe
    //il figlio poi farà la sostituzione e esegue il comando
    //devo distribuire i comandi tra i figli
    int quale_figlio = 0;
    //quale figlio assumerà valori tra 0 e argv[2] dando un comando a testa ai figli
    while(fgets(buffer, MAXCMD, file) != NULL){
        write(pipes[quale_figlio][1], buffer, MAXCMD);
        quale_figlio = (quale_figlio + 1) % atoi(argv[2]);
    }
    //ho distribuito i comandi nelle pipe, ora quando
    //sono arrivato qui vuol dire che non c'è
    //più niente da leggere nel file
    for (int i = 0; i < atoi(argv[2]); i++)
    {
        close(pipes[i][1]);
        //printf("Padre: aspetto figlio %d\n", figli[i]);
        waitpid(figli[i], NULL, 0);
    }
    
    free(figli);
    
    //ricordarsi di liberare la memoria di figli
    return 0;
}