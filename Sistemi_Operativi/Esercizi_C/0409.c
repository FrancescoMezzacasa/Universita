#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

#define MAXLINES 256
#define MAXPATH 1024

void * sommaNumeri(void * path){
    FILE * file = fopen((char *)path, "r");
    if(file == NULL){
        perror("Errore");
        pthread_exit(NULL);
    }
    int numero;
    int somma = 0;
    while(fscanf(file, "%d", &numero) != EOF){
        somma += numero;
    }
    fclose(file);
    int * retVal = (int *)malloc(sizeof(int));
    *retVal = somma;
    return (void *)retVal;
    //deve ritornare un void * e non deve essere locale
    //quindi devo fare malloc e free fuori
}

int main (int argc, char *argv[]) {
    FILE * pathfile;
    pathfile = fopen(argv[1], "r");
    if(pathfile == NULL){
        perror("Errore");
    }
    int count = 0;//cosi tengo conto di quanti thread creo
    char path[MAXPATH][MAXLINES];//array di path
    pthread_t threads[MAXLINES];
    int sommeThread[MAXLINES];

    while(fgets(path[count], MAXPATH, pathfile) != NULL){
        path[count][strcspn(path[count], "\n")] = 0;//strcspn ti dice quanti caratteri ci sono primadi \n
        //deve fare tutto questo solo se la riga non è vuota (ultima del file)
        if(path[count][0] != '\0'){
            printf("Ciao sono nel while al giro %d\n", count);
            printf("QUesto è il path %s\n", path[count]);
            pthread_create(&threads[count], NULL, sommaNumeri, (void *)path[count]);
            count++;
        }
    }

    //quando arrivo qua dovrei aver creato tutti i thread
    //loro hanno calcolato e ritornato la somma
    //io devo fare join e prendermi la somma
    //inoltre in count ho quanti thread ho effettivamente creato
    //(quindi anche quante righe ha il file)

    for (int i = 0; i < count; i++)
    {
        void *retVal;
        pthread_join(threads[i], &retVal);
        sommeThread[i] = *(int *)retVal;
        free(retVal);
        printf("Somma del thread %d: %d\n", i, sommeThread[i]);
        //la join vuole un doppio puntatore quindi devo fare cosi
        //poi la casto e intero e la butto nell'array
    }
    
    //quando arrivo qua dovrei avere l'array con le somme parziali
    //calcolate dai thread
    int sommaTot = 0;
    for (int i = 0; i < count; i++)
    {
        sommaTot += sommeThread[i];
    }
    printf("La somma totale è %d\n", sommaTot);
    fclose(pathfile);
    return 0;
}