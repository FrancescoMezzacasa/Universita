#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stack.h"
#include "interpreter.h"
#include "operations.h"

//questa variabile globale mi serve per capire se il file da cui leggo i comandi è gia stato chiuso o no
//se è gia stato chiuso allora non serve richiuderlo altrimenti quando esco forzatamente con exit devo chiuderlo
//e lo faccio usando la funzione atexit
FILE * file = NULL;
//lo uso come un flag

//faccio la stessa identica cosa per lo stack, per capire se l'ho gia svuotato o no
struct stack * stackFlag = NULL;

void funzioneUscitaPulizia(){
    if(file != NULL){
        fclose(file);
        file = NULL;
    }
    if(stackFlag != NULL){
        clearStack(stackFlag);
        stackFlag = NULL;
    }
}

void interpreta(char * filename, struct stack *s){
    file = fopen(filename, "r");
    if(file == NULL){
        printf("Errore nell'apertura del file\n");
        exit(1);
    }
    //qua è come settare il mio flag dello stack a true (gli assegno uno stack vero cosi non è più false)
    stackFlag = s;

    //quando chiamerò exit farà questa funzione
    atexit(funzioneUscitaPulizia);

    char token[MAX_TOKEN];
    //fscanf ritorna quanti elementi è riuscita a "memorizzare", e salta tutti gli spazi per arrivare 
    //al %s che gli ho dato, quindi facendo finche è = 1 ogni volta legge un carattere e si ferma quando è EOF
    while(fscanf(file, "%s", token) == 1){
        if(strcmp(token, "[") == 0){//se trovo un tensore
            float buffer[MAX_TENSOR_SIZE];
            //uso un array di buffer di dimensione fissata dato che non posso già a priori creare il tensore non sapendo
            //quanti numeri leggeò
            int conto = 0;
            while(fscanf(file, "%s", token) == 1 && strcmp(token, "]") != 0){//finche leggo ancora cose e non trovo la fine del tensore
                if(conto >= MAX_TENSOR_SIZE){
                    printf("Dimensione massima del tensore scrivendo esplicitamente i valori raggiunta\n");
                    exit(1);
                }
                //devo convertire token ad un float per poterlo mettere in buffer e poi nel tensore
                //non posso usare atof perche se no non riesco a controllare se i numeri erano validi

                //uso strtof che oltre alla stringa da convertire prende anche un puntatore in cui mette
                //la parte di stringa a cui è arrivato dopo la conversione, cosi se vedo che non ha convertito niente
                //oppure che manca ancora qualcosa so che non era valido (tipo che erano tutte lettere o prima un float e poi lettere)

                char *parteMancante;
                float numero = strtof(token, &parteMancante);

                if(parteMancante == token || *parteMancante != '\0'){
                    //se la parte mancante è uguale al token iniziale non è successo niente (quindi non è un float)
                    //oppure se la parte mancante non è il terminatore vuol dire che ho ancora qualcosa
                    printf("Impossibile convertire %s ad un numero decimale\n", token);
                    exit(1);
                }
                //se sono qua ce l ha fatta a convertirlo
                buffer[conto] = numero;//aggiungo i numeri che leggo all'array 
                conto++;
            }
            if(feof(file)){//se sono uscito dal while perchè è finito il file (parentesi quadra aperta e non chiusa)
                printf("Parentesi quadra non chiusa, impossibile interpretare il tensore\n");
                exit(1);
            }
            //se sono qua vuol dire che osno uscito perchè ho trovato la parentesi chiusa
            //creo il tensore con 1 dimensione e come shape quanti elementi ho
            int shape[MAX_DIM] = {0};//lo metto tutto a zero anche se userò solo il primo
            shape[0] = conto;
            struct tensor * t = createTensor(1, shape);
            for(int i = 0; i < conto; i++){
                t->data[i] = buffer[i];
            }
            pushTensor(t, s);
        } else if(token[0] == '"'){//non è richiesto che ci sia uno spazio dopo le virgolette, quindi fscanf legge anche la prima parola del nome insieme
                                    //controllo quindi solo se il primo carattere sono le virgolette
            char stringa[MAX_STRING] = "";
            int pos = 0;

            if(strlen(token) > 1){//se token fosse lungo 1 vorrebbe dire che avrebbe effettivamente letto solo le virgolette
                //se non lo è aggiungere la parola alla stringa finale togliendo le virgolette all inizio
                for(int i = 1; i < strlen(token); i++){//aggiungo alla stringa shiftando di 1 (le virgolette che non voglio)
                    stringa[pos] = token[i];
                    pos++;
                }
            }
            //controllo se la stringa era formata da una sola parola senza spazi (esempio "nomefile.txt")
            if(stringa[pos - 1] == '"'){
                //se è formata da una sola parola sostituisco le virgolette finali con il terminatore e ho fatto
                stringa[pos - 1] = '\0';
            } else{
                //se non era formata da una sola parola devo prendere le altre
                //uso fgetc cosi prendo tutti i caratteri qualsiasi essi siano fino alle virgolette
                int c;//fgetc ritorna un intero
                while((c = fgetc(file)) != EOF && c != '"'){
                    if(pos < MAX_STRING - 1){
                        //controllo di non avere la stringa piena e inserisco
                        stringa[pos] = (char)c;
                        pos++;
                    } else{
                        printf("Lunghezza massima stringa raggiunta\n");
                        exit(1);
                    }
                }
                if(feof(file)){
                    //se sono uscito perchè è finito
                    printf("Virgolette non chiuse, impossibile interpretare la stringa\n");
                    exit(1);
                }
                //se sono qui è finito perchè ha trovato le virgolette
                stringa[pos] = '\0'; 
            }
            pushString(stringa, s);

        } else if(strcmp(token, "p") == 0){//se trovo la p (print)
            printOperation(s);
        } else if(strcmp(token, "d") == 0){
            dupOperation(s);
        } else if(strcmp(token, "D") == 0){
            dropOperation(s);
        } else if(strcmp(token, "s") == 0){
            swapOperation(s);
        } else if(strcmp(token, "o") == 0){
            overOperation(s);
        } else if(strcmp(token, "+") == 0){
            sommaOperation(s);
        } else if(strcmp(token, "-") == 0){
            sottrazioneOperation(s);
        } else if(strcmp(token, "*") == 0){
            prodottoOperation(s);
        } else if(strcmp(token, ">") == 0){
            maggioreOperation(s);
        } else if(strcmp(token, "<") == 0){
            minoreOperation(s);
        } else if(strcmp(token, "=") == 0){
            ugualeOperation(s);
        } else if(strcmp(token, "&") == 0){
            andOperation(s);
        } else if(strcmp(token, "|") == 0){
            orOperation(s);
        } else if(strcmp(token, "!") == 0){
            notOperation(s);
        } else if(strcmp(token, "$") == 0){
            selezioneOperation(s);
        } else if(strcmp(token, "r") == 0){
            reshapeOperation(s);
        } else if(strcmp(token, "@") == 0){
            matmulOperation(s);
        } else if(strcmp(token, "_") == 0){
            appiattisciOperation(s);
        } else if(strcmp(token, "#") == 0){
            shapeOperation(s);
        } else if(strcmp(token, "R") == 0){
            reluOperation(s);
        } else if(strcmp(token, "m") == 0){
            minOperation(s);
        } else if(strcmp(token, "M") == 0){
            maxOperation(s);
        } else if(strcmp(token, "S") == 0){
            riduzioneOperation(s);
        } else if(strcmp(token, "f") == 0){
            fillOperation(s);
        } else if(strcmp(token, "?") == 0){
            randomOperation(s);
        } else if(strcmp(token, ".") == 0){
            dotProductOperation(s);
        } else if(strcmp(token, "c") == 0){
            convoluzioneOperation(s);
        } else if(strcmp(token, "(") == 0){
            leggiImmagineOperation(s);
        } else if(strcmp(token, ")") == 0){
            scriviImmagineOperation(s);
        } else if(strcmp(token, "{") == 0){
            leggiTensoredaFileOperation(s);
        } else if(strcmp(token, "}") == 0){
            scriviTensoresuFileOperation(s);
        } else if (strcmp(token, "??") == 0) {
            // Questo è il nostro comando spia temporaneo!
            struct stackElement el = pop(s);
            if(el.tipo == STRING_T) {
                printf("[DEBUG] Stringa letta: \"%s\"\n", el.data.str);
            } else {
                printf("[DEBUG] Ops, non è una stringa!\n");
                // (opzionale) Se era un tensore, lo rimettiamo giù per non romperlo
                if(el.tipo == TENSOR_T) pushTensor(el.data.t, s);
            }
        } else {
            printf("Comando sconosciuto\n");
            exit(1);
        }
    }
    if(file != NULL){
        fclose(file);
        file = NULL;
    }
    //se sono arrivato qua vuol dire che non serve che venga svuotato lo stack dalla funzione atexit
    //ma basta che lo faccia il main, quindi rimetto il flag a false
    stackFlag = NULL;
    
}