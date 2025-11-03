#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <stdlib.h>
#include <string.h>

char *stato_semaforo(sem_t *semaforo){
    int valore;
    sem_getvalue(semaforo, &valore);
    if(valore == 0){
        return("free");
    } else if(valore == 1){
        return("busy");
    } else{
        return("Error");
    }
    return("Error2");
}

int main (int argc, char *argv[]) {
    FILE * file;
    file = fopen("catalog.txt", "r");
    if(file == NULL){
        perror("Errore nell'apertura del file");
        return 1;
    }

    /* voglio creare un array di semafori uno per ogni macchina che leggo dal file
    e li chiamo come la macchina, salvo anche i nomi dei semafori in un array
    cosi li prendo piu facilmente*/

    //numero massimo di macchine e lunghezza massima nome macchina 256
    char macchine[256][256];
    sem_t *semafori[256];
    int N = 0; //contatore per array di nomi
    char nome_semaforo[256];
    while(fgets(macchine[N], 256, file) != NULL){
        strcpy(nome_semaforo, "/");
        strcat(nome_semaforo, macchine[N]); //adesso nome_semaforo dovrebbe contenere "/nomemacchina"
        semafori[N] = sem_open(nome_semaforo, O_CREAT, S_IRWXU, 0); //creato il semaforo
        N++;
    }

    char comando[256];
    char nome_macchina[256]; //qua ci metterò il nome della macchine senza il \n
    char nome_lock[256]; //qua ci andrà il nome della vettura messo dopo a lock
    char nome_release[256];
    int trovato = 0;//flag per vedere se trovo la macchina con lock
    int valore_lock;//ci metto il valore di ritorno di getvalue nel lock
    int valore_release;
    while(1){
        printf("Command: ");
        fgets(comando, sizeof(comando), stdin);
        comando[strcspn(comando, "\n")] = '\0';//strcspn restituisce l indice della prima occorrenza
        

        if(strcmp(comando, "quit") == 0){
            break;
        }else if(strcmp(comando, "view") == 0){
            for(int i = 0; i < N; i++){
                //devo togliere il \n dai nomi che prendo in macchine
                nome_macchina[0] = '\0'; //riazzero la stringa
                strncpy(nome_macchina, macchine[i], strlen(macchine[i]) - 1);
                nome_macchina[strlen(macchine[i]) - 1] = '\0'; //la termino
                printf("Car: %s, status: %s\n", nome_macchina, stato_semaforo(semafori[i]));
            }
        }else if(strncmp(comando, "lock ", 5) == 0){
            if(strlen(comando) == 4){
                printf("Inserire il nome della vettura dopo lock\n");
            }
            //prima devo estrapolare il nome della macchina dal comando "lock <vettura>""
            nome_lock[0] = '\0';
            sscanf(comando, "lock %s", nome_lock);
            //poi ciclo sull'array di nomi di macchine e gli tolgo man mano il \n
            //man mano anche confronto se è uguale a <vettura>, se lo è tengo quell indice e
            //cosi lo posso usare nell'array di semafori per fare il getvalue
            for(int i = 0; i < N; i++){
                nome_macchina[0] = '\0'; 
                strncpy(nome_macchina, macchine[i], strlen(macchine[i]) - 1);
                nome_macchina[strlen(macchine[i]) - 1] = '\0';
                if(strcmp(nome_lock, nome_macchina) == 0){// se l ho trovato
                    trovato = 1;
                    //devo verificare se è libera o occupata
                    sem_getvalue(semafori[i], &valore_lock);
                    if(valore_lock == 1){
                        printf("Error. Car %s already locked\n", nome_lock);
                        continue;
                    }else if(valore_lock == 0){
                        sem_post(semafori[i]);
                        printf("Car %s is now locked\n", nome_lock);
                    }

                }
            }
            if(trovato == 0){
                printf("Cannot find car %s\n", nome_lock);
            }
            
            
        }else if(strncmp(comando, "release ", 8) == 0){
            /*caso release quasi uguale a lock ma contrario*/
            if(strlen(comando) == 4){
                printf("Inserire il nome della vettura dopo release\n");
            }
            nome_release[0] = '\0';
            sscanf(comando, "release %s", nome_release);
            
            for(int i = 0; i < N; i++){
                nome_macchina[0] = '\0'; 
                strncpy(nome_macchina, macchine[i], strlen(macchine[i]) - 1);
                nome_macchina[strlen(macchine[i]) - 1] = '\0';
                if(strcmp(nome_release, nome_macchina) == 0){// se l ho trovato
                    trovato = 1;
                    //devo verificare se è libera o occupata
                    sem_getvalue(semafori[i], &valore_release);
                    if(valore_release == 0){
                        printf("Error. Car %s already released\n", nome_release);
                        continue;
                    }else if(valore_release == 1){
                        sem_wait(semafori[i]);
                        printf("Car %s is now free\n", nome_release);
                    }

                }
            }
            if(trovato == 0){
                printf("Cannot find car %s\n", nome_release);
            }
        }else{
            printf("Unknown Command\n");
        }
    }
    return 0;
}