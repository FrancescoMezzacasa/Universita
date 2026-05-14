#include <stdio.h>
#include <stdlib.h>
#include "tensor.h"
#include "stack.h"

int testCreateTensor() {
    printf("--- INIZIO TEST TENSORI ---\n");

    // 1. Prepariamo le misure: vogliamo una matrice 2x3 (ndim = 2)
    int mia_shape[2] = {2, 3};

    // 2. Chiamiamo la nostra fabbrica
    printf("Chiamo createTensor...\n");
    struct tensor *t = createTensor(2, mia_shape);

    // 3. Verifichiamo che la struttura sia nata bene stampando i valori
    printf("\n=== VERIFICA STRUTTURA ===\n");
    printf("Numero dimensioni (ndim): %d\n", t->ndim);
    printf("Shape: [%d, %d]\n", t->shape[0], t->shape[1]);
    printf("Ref count iniziale: %d\n", t->ref_count);

    // 4. Proviamo a scrivere nella memoria allocata per i dati (il nostro array 1D)
    // Se non avessimo fatto la malloc dei dati, qui avremmo un bel Segmentation Fault!
    printf("\n=== VERIFICA MEMORIA DATI ===\n");
    t->data[0] = 3.14;       // Scriviamo nel primo cassetto (riga 0, col 0)
    t->data[5] = 42.0;       // Scriviamo nell'ultimo cassetto (riga 1, col 2)
                             // Ricorda la formula: i*C + j -> 1*3 + 2 = 5
    
    printf("Valore in data[0]: %f\n", t->data[0]);
    printf("Valore in data[5]: %f\n", t->data[5]);

    printf("\n--- TEST SUPERATO (Se non è esploso nulla) ---\n");

    // ATTENZIONE: per ora stiamo lasciando spazzatura in memoria.
    // Il prossimo step sarà testare la tua funzione tensor_decref(t) qui sotto!
    deleteTensor(t);

    return 0;
}

void testStack() {
    printf("\n--- INIZIO TEST STACK ---\n");

    // 1. Creiamo e inizializziamo lo stack
    struct stack mio_stack;
    createStack(&mio_stack);
    printf("Stack creato. Indice top iniziale: %d\n", mio_stack.top);

    // 2. Creiamo un tensore finto usando la tua fabbrica
    int shape_finta[2] = {2, 3}; // Matrice 2x3
    struct tensor *mio_tensore = createTensor(2, shape_finta);

    // 3. Facciamo i push
    printf("\nFaccio push di un tensore...\n");
    pushTensor(mio_tensore, &mio_stack);
    
    printf("Faccio push di una stringa...\n");
    pushString("immagine_test.pgm", &mio_stack);

    printf("Indice top dopo gli inserimenti: %d\n", mio_stack.top);

    // 4. Testiamo la POP 1 (Deve essere la stringa)
    printf("\n=== ESTRAZIONE 1 ===\n");
    struct stackElement estratto1 = pop(&mio_stack);
    
    if (estratto1.tipo == STRING_T) {
        printf("[OK] Letta etichetta STRINGA. Valore: %s\n", estratto1.data.str);
    } else {
        printf("[ERRORE] Mi aspettavo una stringa ma l'etichetta dice altro!\n");
    }

    // 5. Testiamo la POP 2 (Deve essere il tensore)
    printf("\n=== ESTRAZIONE 2 ===\n");
    struct stackElement estratto2 = pop(&mio_stack);
    
    if (estratto2.tipo == TENSOR_T) {
        printf("[OK] Letta etichetta TENSORE. Ndim: %d\n", estratto2.data.t->ndim);
    } else {
        printf("[ERRORE] Mi aspettavo un tensore ma l'etichetta dice altro!\n");
    }
    printf("\n=== ESTRAZIONE 3 ===\n");
    struct stackElement estratto3 = pop(&mio_stack);
    // 6. Testiamo la gestione della memoria del tensore
    // Ora che l'abbiamo tolto dallo stack e non ci serve più, lo eliminiamo
    deleteTensor(mio_tensore);

    printf("--- FINE TEST STACK ---\n");
}

int main(){
    testCreateTensor();
    testStack();
}