//FRANCESCO MEZZACASA SM3201587
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <omp.h>
#include <sys/mman.h>
#include <unistd.h>//per la close

#include "tensor.h"

//alla create passo solo ndim e shape perche tanto i numeri li inserirò man mano che li leggo
//mi serve passarli entrambi perche se li ricevo in input avranno sicuro ndim=1 ma ci sono molte operazioni
//che chiameranno questa funzione per creare matrici quindi ndim=2
struct tensor * createTensor(int ndim, int * shape){
    if(ndim <= 0){
        printf("Il numero di dimensioni deve essere positivo\n");
        exit(1);
    }
    if(ndim > MAX_DIM){
        printf("Il numero di dimensioni non può essere maggiore del massimo consentito\n");
        exit(1);
    }
    struct tensor * t = (struct tensor *)malloc(sizeof(struct tensor));
    if(t == NULL){
        printf("Errore nell'allocazione di memoria per il tensore\n");
        exit(1);
    }
    
    t->ndim = ndim;

    for(int i = 0; i < ndim; i++){
        t->shape[i] = shape[i];
    }

    //devo allocare lo spazio per questo numero di elementi in data
    //devo trovare quanti elementi avrò in totale, se ndim è 1 sarà solamente shape[0]
    //se ndim è > 1 allora sarà il prodotto di shape, tipo se ho una matrice 2 x 3 ho 6 elementi
    long long int numElementi = shape[0];
    if(ndim > 1){
        for(int i = 1; i < ndim; i++){
            numElementi *= shape[i];
        }
    }

    //decido di salvarmi questo valore in un campo della struct tensor
    //dato che in tutte le operazioni elemento per elemento il ciclo for sarà sempre fino a questo numero
    t->numElementi = numElementi;

    t->data = (float *)malloc(sizeof(float)*numElementi);
    if(t->data == NULL){
        printf("Errore nell'allocazione di memoria del campo data del tensore\n");
        exit(1);
    }

    t->ref_count = 1; //quando lo creo sicuro ho un riferimento
    t->on_disk = false;
    return t; 
}

void deleteTensor(struct tensor * t){
    if(t == NULL)
        return;
    //printf("%d\n", t->ref_count);
    if(t->ref_count >= 1){
        t->ref_count--;
        if(t->ref_count == 0){//se è diventato 0 non lo punta piu nessuno quindi cancella definitivamente
            if(t->on_disk == true){//se è su disco (fatto con mmap)
                //munmap vuole il puntatore ritornato da mmap originariamente, che è un void * 
                //poi come prima lo casto a char * che è 1 byte cosi mi muovo di 1 alla volta.
                //per tornare alla mappatura iniziale devo partire dall'inizio dei dati e tornare indietro di offset
                void * inizioMappatura = (void *)((char *)t->data - t->offset);
                //munmap vuole anche la size della zona mappata che in quesot caso è l'offset + la dimensione di un float * quanti ne ho
                size_t sizeMappatura = t->offset + t->numElementi*sizeof(float);
                munmap(inizioMappatura, sizeMappatura);
                close(t->fd);
                free(t);
            } else { //se è un tensore normale (non su disco)
                if(t->data != NULL){
                    free(t->data);
                }
                free(t);
            }
            
        }
    } else{
        printf("Errore nella cancellazione del tensore\n");
    }
    return;
}

void printTensor(struct tensor * t){
    if(t == NULL){
        printf("Nessun tensore da stampare\n");
        return;
    }
    printf("Tensor(shape=[");
    for(int i = 0; i < t->ndim; i++){
        printf("%d", t->shape[i]);
        if(i != t->ndim - 1){//stampo lo spazio tra i numeri quindi non quando sono all'ultimo numero
            printf(" ");
        }
    }
    printf("], data=[");

    for(int i = 0; i < t->numElementi; i++){
        printf("%.2f", t->data[i]);
        if(i != t->numElementi - 1){
            printf(" ");
        }
    }
    printf("])\n");
}

struct tensor *tensorSum(struct tensor * t1, struct tensor * t2){
    if(t1->ndim != t2->ndim){
        printf("La somma è definita solo tra tensori con lo stesso numero di dimensioni\n");
        exit(1);
    }
    //controllo anche che tutte le singole dimensioni siano uguali
    for(int i = 0; i < t1->ndim; i++){
        if(t1->shape[i] != t2->shape[i]){
            printf("Non tutte le dimensioni sono uguali\n");
            exit(1);
        }
    }

    //creo il tensore risultato
    struct tensor * ris = createTensor(t1->ndim, t1->shape);

    //sommo elemento per elemento (parallelizzando)
    #pragma omp parallel for
    for(int i = 0; i < t1->numElementi; i++){
        ris->data[i] = t1->data[i] + t2->data[i];
    }
    return ris;
}

struct tensor *tensorSubtraction(struct tensor * t1, struct tensor * t2){
    if(t1->ndim != t2->ndim){
        printf("La sottrazione è definita solo tra tensori con lo stesso numero di dimensioni\n");
        exit(1);
    }
    //controllo anche che tutte le singole dimensioni siano uguali
    for(int i = 0; i < t1->ndim; i++){
        if(t1->shape[i] != t2->shape[i]){
            printf("Non tutte le dimensioni sono uguali\n");
            exit(1);
        }
    }
    //creo il tensore risultato
    struct tensor * ris = createTensor(t1->ndim, t1->shape);

    //sottraggo elemento per elemento (parallelizzando)
    #pragma omp parallel for
    for(int i = 0; i < t1->numElementi; i++){
        ris->data[i] = t1->data[i] - t2->data[i];
    }
    return ris;
}

struct tensor *tensorProduct(struct tensor * t1, struct tensor * t2){
    if(t1->ndim != t2->ndim){
        printf("Il prodotto è definito solo tra tensori con lo stesso numero di dimensioni\n");
        exit(1);
    }
    //controllo anche che tutte le singole dimensioni siano uguali
    for(int i = 0; i < t1->ndim; i++){
        if(t1->shape[i] != t2->shape[i]){
            printf("Non tutte le dimensioni sono uguali\n");
            exit(1);
        }
    }

    //creo il tensore risultato
    struct tensor * ris = createTensor(t1->ndim, t1->shape);

    //faccio il prodotto elemento per elemento (parallelizzando)
    #pragma omp parallel for
    for(int i = 0; i < t1->numElementi; i++){
        ris->data[i] = t1->data[i] * t2->data[i];
    }
    return ris;
}

struct tensor *tensorMaggiore(struct tensor * t1, struct tensor * t2){
    if(t1->ndim != t2->ndim){
        printf("Il confronto è definito solo tra tensori con lo stesso numero di dimensioni\n");
        exit(1);
    }
    //controllo anche che tutte le singole dimensioni siano uguali
    for(int i = 0; i < t1->ndim; i++){
        if(t1->shape[i] != t2->shape[i]){
            printf("Non tutte le dimensioni sono uguali\n");
            exit(1);
        }
    }

    //creo il tensore risultato
    struct tensor * ris = createTensor(t1->ndim, t1->shape);

    //confronto elemento per elemento (parallelizzando)
    #pragma omp parallel for
    for(int i = 0; i < t1->numElementi; i++){
        ris->data[i] = (t1->data[i] > t2->data[i]) ? 1 : 0;
    }
    return ris;
}

struct tensor *tensorMinore(struct tensor * t1, struct tensor * t2){
    if(t1->ndim != t2->ndim){
        printf("Il confronto è definito solo tra tensori con lo stesso numero di dimensioni\n");
        exit(1);
    }
    //controllo anche che tutte le singole dimensioni siano uguali
    for(int i = 0; i < t1->ndim; i++){
        if(t1->shape[i] != t2->shape[i]){
            printf("Non tutte le dimensioni sono uguali\n");
            exit(1);
        }
    }

    //creo il tensore risultato
    struct tensor * ris = createTensor(t1->ndim, t1->shape);

    //confronto elemento per elemento (parallelizzando)
    #pragma omp parallel for
    for(int i = 0; i < t1->numElementi; i++){
        ris->data[i] = (t1->data[i] < t2->data[i]) ? 1 : 0;
    }
    return ris;
}

struct tensor *tensorUguale(struct tensor * t1, struct tensor * t2){
    if(t1->ndim != t2->ndim){
        printf("Il confronto è definito solo tra tensori con lo stesso numero di dimensioni\n");
        exit(1);
    }
    //controllo anche che tutte le singole dimensioni siano uguali
    for(int i = 0; i < t1->ndim; i++){
        if(t1->shape[i] != t2->shape[i]){
            printf("Non tutte le dimensioni sono uguali\n");
            exit(1);
        }
    }

    //creo il tensore risultato
    struct tensor * ris = createTensor(t1->ndim, t1->shape);

    //confronto elemento per elemento (parallelizzando)
    #pragma omp parallel for
    for(int i = 0; i < t1->numElementi; i++){
        ris->data[i] = (t1->data[i] == t2->data[i]) ? 1 : 0;
    }
    return ris;
}

struct tensor *tensorAndLogico(struct tensor * t1, struct tensor * t2){
    if(t1->ndim != t2->ndim){
        printf("Le operazioni logiche sono definite solo tra tensori con lo stesso numero di dimensioni\n");
        exit(1);
    }
    //controllo anche che tutte le singole dimensioni siano uguali
    for(int i = 0; i < t1->ndim; i++){
        if(t1->shape[i] != t2->shape[i]){
            printf("Non tutte le dimensioni sono uguali\n");
            exit(1);
        }
    }

    //per le operazioni logiche controllo che siano formati solo da 1 e 0
    bool tuttiGiusti = true;

    #pragma omp parallel for
    for(int i = 0; i < t1->numElementi; i++){
        if(((t1->data[i] != 0.0) && (t1->data[i] != 1.0)) || ((t2->data[i] != 0.0) && (t2->data[i] != 1.0))){
            #pragma omp critical
            {
                tuttiGiusti = false;
            }
        }
    }
    //se è false ne ha visto almeno uno che non va bene quindi è un errore
    if(tuttiGiusti == false){
        printf("I tensori tra cui fare l'AND logico devono essere composti solo da 1 o 0\n");
        exit(1);
    }
    //creo il tensore risultato
    struct tensor * ris = createTensor(t1->ndim, t1->shape);

    //una volta che sono sicuro che siano formati solo da 0 e 1 mi basta fare il prodotto
    //se entrambi sono 1 verrà 1 altrimenti 0
    #pragma omp parallel for
    for(int i = 0; i < t1->numElementi; i++){
        ris->data[i] = t1->data[i] * t2->data[i];
    }
    return ris;
}

struct tensor *tensorOrLogico(struct tensor * t1, struct tensor * t2){
    if(t1->ndim != t2->ndim){
        printf("Le operazioni logiche sono definite solo tra tensori con lo stesso numero di dimensioni\n");
        exit(1);
    }
    //controllo anche che tutte le singole dimensioni siano uguali
    for(int i = 0; i < t1->ndim; i++){
        if(t1->shape[i] != t2->shape[i]){
            printf("Non tutte le dimensioni sono uguali\n");
            exit(1);
        }
    }

    //per le operazioni logiche controllo che siano formati solo da 1 e 0
    bool tuttiGiusti = true;

    #pragma omp parallel for
    for(int i = 0; i < t1->numElementi; i++){
        if(((t1->data[i] != 0.0) && (t1->data[i] != 1.0)) || ((t2->data[i] != 0.0) && (t2->data[i] != 1.0))){
            #pragma omp critical
            {
                tuttiGiusti = false;
            }
        }
    }
    //se è false ne ha visto almeno uno che non va bene quindi è un errore
    if(tuttiGiusti == false){
        printf("I tensori tra cui fare l'OR logico devono essere composti solo da 1 o 0\n");
        exit(1);
    }
    //creo il tensore risultato
    struct tensor * ris = createTensor(t1->ndim, t1->shape);

    //una volta che sono sicuro che siano tutti formati da 0 e 1 mi basta fare la somma
    //se entrambi 0 fa 0 altrimenti 1 (se entrambi 1 deve fare 1)
    #pragma omp parallel for
    for(int i = 0; i < t1->numElementi; i++){
        //controllo se la somma è maggiore di 0 e se lo è vuol dire che non erano tutti e due 0 quindi deve fare 1
        ris->data[i] = (t1->data[i] + t2->data[i] > 0.0) ? 1 : 0;
    }
    return ris;
}

struct tensor *tensorNotLogico(struct tensor * t){
    //controllo che sia formato solo da 0 e 1
    bool tuttiGiusti = true;

    #pragma omp parallel for
    for(int i = 0; i < t->numElementi; i++){
        if((t->data[i] != 0.0) && (t->data[i] != 1.0)){
            #pragma omp critical
            {
                tuttiGiusti = false;
            }
        }
    }
    //se è false ne ha visto almeno uno che non va bene quindi è un errore
    if(tuttiGiusti == false){
        printf("Il tensore su cui fare il NOT deve essere composto solo da 1 o 0\n");
        exit(1);
    }

    struct tensor * ris = createTensor(t->ndim, t->shape);

    #pragma omp parallel for
    for(int i = 0; i < t->numElementi; i++){
        //controllo se un elemento è 0 e se lo è lo metto a 1
        ris->data[i] = (t->data[i] == 0.0) ? 1 : 0;
    }
    return ris;

}

struct tensor *tensorSelection(struct tensor * t1, struct tensor * t2, struct tensor * mask){
    if(t1->ndim != t2->ndim || t1->ndim != mask->ndim){
        printf("La selezione si può fare solo tra tensori con lo stesso numero di dimensioni\n");
        exit(1);
    }
    //controllo anche che tutte le singole dimensioni siano uguali
    for(int i = 0; i < t1->ndim; i++){
        if(t1->shape[i] != t2->shape[i] || t1->shape[i] != mask->shape[i]){
            printf("Non tutte le dimensioni sono uguali\n");
            exit(1);
        }
    }

    //per la selezione controllo che la maschera sia fatta solo da 0 e 1
    bool tuttiGiusti = true;

    #pragma omp parallel for
    for(int i = 0; i < mask->numElementi; i++){
        if((mask->data[i] != 0.0) && (mask->data[i] != 1.0)){
            #pragma omp critical
            {
                tuttiGiusti = false;
            }
        }
    }
    //se è false ne ha visto almeno uno che non va bene quindi è un errore
    if(tuttiGiusti == false){
        printf("La maschera di selezione deve essere composta solo da 1 o 0\n");
        exit(1);
    }
    //creo il tensore risultato
    struct tensor * ris = createTensor(t1->ndim, t1->shape);

    //faccio il confronto in base alla maschera
    #pragma omp parallel for
    for(int i = 0; i < t1->numElementi; i++){
        ris->data[i] = (mask->data[i] == 1.0) ? t1->data[i] : t2->data[i];
    }
    return ris;
}

struct tensor *tensorMatmul(struct tensor * t1, struct tensor * t2){
    int newShape[2] = {t1->shape[0], t2->shape[1]};

    //in questa funzione suppongo che le matrici che mi arrivano siano compatibili a fare il prodotto
    //quindi deve valere che il numero di colonne di t1 è uguale al numero di righe di t2, lo chiamerò n
    int n = t1->shape[1];
    struct tensor * ris = createTensor(2, newShape);

    #pragma omp parallel for collapse(2)
    for (int i = 0; i < newShape[0]; i++) {
        for (int j = 0; j < newShape[1]; j++) {
            //creo una variabile locale per ogni thread in cui farò le somme parziali
            //cosi non devo ogni ciclo scrivere in ris->data[i * t2->shape[1] + j] ma scrivo in somma che è molto più veloce
            float somma = 0;
            for (int k = 0; k < n; k++) {
                //per accedere ad una certa cella con i e j (o k) devo moltiplicare i per il numero di colonne di quella matrice
                //per far sì che i rappresenti le righe
                somma += t1->data[i * t1->shape[1] + k] * t2->data[k * t2->shape[1] + j];
            }
            ris->data[i * t2->shape[1] + j] = somma;
        }
    }

    return ris;
}

struct tensor *tensorRelu(struct tensor * t){
    struct tensor * ris = createTensor(t->ndim, t->shape);

    #pragma omp parallel for
    for(int i = 0; i < t->numElementi; i++){
        //controllo se un elemento è minore di 0 e lo metto a 0
        ris->data[i] = (t->data[i] < 0.0) ? 0 : t->data[i];
    }
    return ris;
}

struct tensor *tensorMin(struct tensor * t1, struct tensor * t2){
    if(t1->ndim != t2->ndim){
        printf("L'operazione min è definita solo tra tensori con lo stesso numero di dimensioni\n");
        exit(1);
    }
    //controllo anche che tutte le singole dimensioni siano uguali
    for(int i = 0; i < t1->ndim; i++){
        if(t1->shape[i] != t2->shape[i]){
            printf("Non tutte le dimensioni sono uguali\n");
            exit(1);
        }
    }

    //creo il tensore risultato
    struct tensor * ris = createTensor(t1->ndim, t1->shape);

    //confronto elemento per elemento (parallelizzando)
    #pragma omp parallel for
    for(int i = 0; i < t1->numElementi; i++){
        ris->data[i] = (t1->data[i] <= t2->data[i]) ? t1->data[i] : t2->data[i];
    }
    return ris;

}

struct tensor *tensorMax(struct tensor * t1, struct tensor * t2){
    if(t1->ndim != t2->ndim){
        printf("L'operazione min è definita solo tra tensori con lo stesso numero di dimensioni\n");
        exit(1);
    }
    //controllo anche che tutte le singole dimensioni siano uguali
    for(int i = 0; i < t1->ndim; i++){
        if(t1->shape[i] != t2->shape[i]){
            printf("Non tutte le dimensioni sono uguali\n");
            exit(1);
        }
    }

    //creo il tensore risultato
    struct tensor * ris = createTensor(t1->ndim, t1->shape);

    //confronto elemento per elemento (parallelizzando)
    #pragma omp parallel for
    for(int i = 0; i < t1->numElementi; i++){
        ris->data[i] = (t1->data[i] >= t2->data[i]) ? t1->data[i] : t2->data[i];
    }
    return ris;

}

struct tensor *tensorReduction(struct tensor * t){
    //il nuovo tensore avrà un elemento
    int newShape[1] = {1};
    struct tensor * ris = createTensor(1, newShape);

    float somma = 0;
    #pragma omp parallel for reduction(+:somma)
    for(int i = 0; i < t->numElementi; i++){
        somma += t->data[i];
    }

    ris->data[0] = somma;
    return ris;
}

struct tensor *tensorFill(struct tensor * valori, struct tensor * newShape){
    int risNdim = newShape->numElementi;
    int risShape[MAX_DIM];
    //la shape del nuovo vettore sarà quello che leggo nel tensore newShape
    for(int i = 0; i < risNdim; i++){
        risShape[i] = (int)newShape->data[i];
    }

    struct tensor * ris = createTensor(risNdim, risShape);

    #pragma omp parallel for
    for(int i = 0; i < ris->numElementi; i++){
        ris->data[i] = valori->data[i%valori->numElementi];
    }
    return ris;
}

struct tensor *tensorRandom(struct tensor * newShape){
    //creo un nuovo tensore con la shape data dal tensore in input
    int risNdim = newShape->numElementi;
    int risShape[MAX_DIM];
    for(int i = 0; i < risNdim; i++){
        risShape[i] = (int)newShape->data[i];
    }

    struct tensor * ris = createTensor(risNdim, risShape);

    for(int i = 0; i < ris->numElementi; i++){
        ris->data[i] = (float)rand()/(float)RAND_MAX;//divido per il massimo possibile cosi saranno tutti tra 0 e 1
    }
    return ris;
}

struct tensor * tensorDotProduct(struct tensor * t1, struct tensor * t2){
    int newShape[1] = {1};
    struct tensor * ris = createTensor(1, newShape);

    float somma = 0;
    #pragma omp parallel for reduction(+:somma)
    //so già che i vettori hanno le stesse dimensioni quindi vado fino a t1->numElementi tanto è uguale a quello di t2
    for(int i = 0; i < t1->numElementi; i++){
        somma += t1->data[i]*t2->data[i];
    }

    ris->data[0] = somma;
    return ris;
}

struct tensor * tensorConvolution(struct tensor * kernel, struct tensor * matrice){
    //creo la matrice risultato che ha le stesse dimensioni di quella in input
    struct tensor * ris = createTensor(2, matrice->shape);

    int h = matrice->shape[0];
    int w = matrice->shape[1];

    //il raggio del kernel, inteso come distanza dal punto centrale, sarà la divisione intera per due
    //ad esempio se il kernel è 3x3 il raggio sarà 1, ovvero per la convoluzione guarderò tutti i vicini fino a distanza 1
    //dalla cella corrente. Se fosse 5x5 avrei che sarebbe 2
    int r = kernel->shape[0] / 2;

    int lato = kernel->shape[0];

    #pragma omp parallel for collapse(2)
    //itero su tutti gli elementi della matrice
    for(int i = 0; i < h; i++){
        for(int j = 0; j < w; j++){
            //qua devo iterare su tutti i vicini dell'elemento corrente
            float somma = 0;
            for(int yker = -r; yker <= r; yker++){
                for(int xker = -r; xker <= r; xker++){
                    //la posizione corrente nella matrice è m[i*w + j], però sto scorrendo quelli intorno, quindi
                    //a i e j devo sommare le yker e xker che stanno scorrendo su tutto il kernel, cosi sto scorrendo 
                    //nella matrice principale in un blocco di dimensione del kernel
                    //a questo moltiplico ogni volta la cella corrispondente del kernel, però non posso semplicemente usare
                    //xker e yker perche non vanno da 0 a lato ma da -r a r, quindi devo rinormalizzarle da 0 a lato (sommando r)

                    //devo però prendere in considerazione il padding, ma invece di farlo farò finta che ogni volta che 
                    //nella matrice principale dovrei sommare un valore fuori dai confini, non lo sommerò, o meglio
                    //farò la somma solo quando gli indici sono dentro
                    if((i + yker >= 0) && (i + yker < h) && (j + xker >= 0) && (j + xker < w)){
                        somma += matrice->data[(i + yker)*w + (j + xker)]*kernel->data[(yker + r)*lato + (xker + r)];
                    }
                }
            }
            ris->data[i*w + j] = somma;
        }
    }

    return ris;

}