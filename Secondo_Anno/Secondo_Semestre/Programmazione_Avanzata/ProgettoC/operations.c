#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>//per mmap
#include <sys/stat.h>//per stat

#include <fcntl.h> //per la open
#include <unistd.h>//per la close

#include "stack.h"
#include "tensor.h"
#include "operations.h"

void printOperation(struct stack * s){
    struct stackElement e = pop(s);
    if(e.tipo == STRING_T){
        printf("Si possono stampare solo i tensori, errore\n");
        exit(1);
    }
    //chiamo la funzione di stampa in tensor.c
    //passandogli il tensore (contenuto nella union)
    printTensor(e.data.t);

    deleteTensor(e.data.t);
}

void dupOperation(struct stack * s){
    struct stackElement el = pop(s);
    if(el.tipo == STRING_T){
        //faccio due push della stringa che ho appena estratto, cosi rimetto la scorsa e ne aggiungo un'altra
        pushString(el.data.str, s);
        pushString(el.data.str, s);
    } else if(el.tipo == TENSOR_T){
        //anche nel caso tensore faccio la stessa cosa di prima
        //ricordandomi di incrementare ref_count perche non faccio una copia
        el.data.t->ref_count++;
        pushTensor(el.data.t, s);
        pushTensor(el.data.t, s);
    } else{
        printf("Errore nella funzione di duplicazione\n");
    }
}

void dropOperation(struct stack * s){
    struct stackElement el = pop(s);
    if(el.tipo == TENSOR_T){
        deleteTensor(el.data.t);
    }
}

void swapOperation(struct stack * s){
    //tolgo i primi due elementi dallo stack
    struct stackElement el1 = pop(s);
    struct stackElement el2 = pop(s);
    
    //li rimetto dentro ma contrari

    if(el1.tipo == TENSOR_T){
        pushTensor(el1.data.t, s);
    } else if(el1.tipo == STRING_T){
        pushString(el1.data.str, s);
    }

    if(el2.tipo == TENSOR_T){
        pushTensor(el2.data.t, s);
    } else if(el2.tipo == STRING_T){
        pushString(el2.data.str, s);
    }

}

void overOperation(struct stack * s){
    struct stackElement el1 = pop(s);
    struct stackElement el2 = pop(s);
    //rimetto il secondo dall alto
    if(el2.tipo == TENSOR_T){
        pushTensor(el2.data.t, s);
    } else if(el2.tipo == STRING_T){
        pushString(el2.data.str, s);
    }

    //rimetto il primo dall'alto
    if(el1.tipo == TENSOR_T){
        pushTensor(el1.data.t, s);
    } else if(el1.tipo == STRING_T){
        pushString(el1.data.str, s);
    }

    if(el2.tipo == TENSOR_T){
        //se l'elemento che ho duplicato è un tensore aumento il ref_count perche non lo sto copiando (come dup)
        el2.data.t->ref_count++;
        pushTensor(el2.data.t, s);
    } else if(el2.tipo == STRING_T){
        pushString(el2.data.str, s);
    }
}

void sommaOperation(struct stack * s){
    struct stackElement el1 = pop(s);
    struct stackElement el2 = pop(s);

    if(el1.tipo != TENSOR_T || el2.tipo != TENSOR_T){
        printf("La somma si può fare solo tra due tensori\n");
        exit(1);
    }
    //se sono qua sicuro ho due tensori
    struct tensor *ris = tensorSum(el1.data.t, el2.data.t);

    pushTensor(ris, s);
    deleteTensor(el1.data.t);
    deleteTensor(el2.data.t);
}

void sottrazioneOperation(struct stack * s){
    struct stackElement el1 = pop(s);
    struct stackElement el2 = pop(s);

    if(el1.tipo != TENSOR_T || el2.tipo != TENSOR_T){
        printf("La sottrazione si può fare solo tra due tensori\n");
        exit(1);
    }
    //se sono qua sicuro ho due tensori

    //el1 è quello che era in cima allo stack, ma sarà il secondo operando della sottrazione
    //es. [ 10 ] [ 3 ] - voglio che faccia 10 - 3 ma 10 è el2
    //quindi nella chiamata a tensorSubtraction li scambio
    struct tensor *ris = tensorSubtraction(el2.data.t, el1.data.t);

    pushTensor(ris, s);
    deleteTensor(el1.data.t);
    deleteTensor(el2.data.t);
}

void prodottoOperation(struct stack * s){
    struct stackElement el1 = pop(s);
    struct stackElement el2 = pop(s);

    if(el1.tipo != TENSOR_T || el2.tipo != TENSOR_T){
        printf("Il prodotto si può fare solo tra due tensori\n");
        exit(1);
    }
    //se sono qua sicuro ho due tensori
    struct tensor *ris = tensorProduct(el1.data.t, el2.data.t);

    pushTensor(ris, s);
    deleteTensor(el1.data.t);
    deleteTensor(el2.data.t);
}

void maggioreOperation(struct stack * s){
    struct stackElement el1 = pop(s);
    struct stackElement el2 = pop(s);

    if(el1.tipo != TENSOR_T || el2.tipo != TENSOR_T){
        printf("Il confronto si può fare solo tra due tensori\n");
        exit(1);
    }
    //se sono qua sicuro ho due tensori
    struct tensor *ris = tensorMaggiore(el1.data.t, el2.data.t);

    pushTensor(ris, s);
    deleteTensor(el1.data.t);
    deleteTensor(el2.data.t);
}

void minoreOperation(struct stack * s){
    struct stackElement el1 = pop(s);
    struct stackElement el2 = pop(s);

    if(el1.tipo != TENSOR_T || el2.tipo != TENSOR_T){
        printf("Il confronto si può fare solo tra due tensori\n");
        exit(1);
    }
    //se sono qua sicuro ho due tensori
    struct tensor *ris = tensorMinore(el1.data.t, el2.data.t);

    pushTensor(ris, s);
    deleteTensor(el1.data.t);
    deleteTensor(el2.data.t);
}

void ugualeOperation(struct stack * s){
    struct stackElement el1 = pop(s);
    struct stackElement el2 = pop(s);

    if(el1.tipo != TENSOR_T || el2.tipo != TENSOR_T){
        printf("Il confronto si può fare solo tra due tensori\n");
        exit(1);
    }
    //se sono qua sicuro ho due tensori
    struct tensor *ris = tensorUguale(el1.data.t, el2.data.t);

    pushTensor(ris, s);
    deleteTensor(el1.data.t);
    deleteTensor(el2.data.t);
}

void andOperation(struct stack * s){
    struct stackElement el1 = pop(s);
    struct stackElement el2 = pop(s);

    if(el1.tipo != TENSOR_T || el2.tipo != TENSOR_T){
        printf("Le operazioni logiche si possono fare solo tra due tensori\n");
        exit(1);
    }
    //se sono qua sicuro ho due tensori
    struct tensor *ris = tensorAndLogico(el1.data.t, el2.data.t);

    pushTensor(ris, s);
    deleteTensor(el1.data.t);
    deleteTensor(el2.data.t);
}

void orOperation(struct stack * s){
    struct stackElement el1 = pop(s);
    struct stackElement el2 = pop(s);

    if(el1.tipo != TENSOR_T || el2.tipo != TENSOR_T){
        printf("Le operazioni logiche si possono fare solo tra due tensori\n");
        exit(1);
    }
    //se sono qua sicuro ho due tensori
    struct tensor *ris = tensorOrLogico(el1.data.t, el2.data.t);

    pushTensor(ris, s);
    deleteTensor(el1.data.t);
    deleteTensor(el2.data.t);
}

void notOperation(struct stack * s){
    struct stackElement el = pop(s);

    if(el.tipo != TENSOR_T){
        printf("La negazione logica si può fare solo su un tensore\n");
        exit(1);
    }

    struct tensor *ris = tensorNotLogico(el.data.t);

    pushTensor(ris, s);
    deleteTensor(el.data.t);
}

void selezioneOperation(struct stack * s){
    struct stackElement mask = pop(s);//il primo che estraggo è la maschera
    struct stackElement el1 = pop(s);
    struct stackElement el2 = pop(s);

    if(el1.tipo != TENSOR_T || el2.tipo != TENSOR_T || mask.tipo != TENSOR_T){
        printf("La selezione si può fare solo tra tensori\n");
        exit(1);
    }
    //se sono qua sicuro ho due tensori
    struct tensor *ris = tensorSelection(el1.data.t, el2.data.t, mask.data.t);

    pushTensor(ris, s);
    deleteTensor(el1.data.t);
    deleteTensor(el2.data.t);
    deleteTensor(mask.data.t);
}

void reshapeOperation(struct stack * s){
    struct stackElement newShape = pop(s);
    struct stackElement el = pop(s);

    if(el.tipo != TENSOR_T || newShape.tipo != TENSOR_T){
        printf("Il reshape si può fare solo su un tensore e anche il vettore con le nuove dimensioni deve essere un tensore\n");
        exit(1);
    }

    //devo controllare che il vettore newShape sia 1D e che il prodotto dei suoi elementi sia uguale al numero di elementi di el
    if(newShape.data.t->ndim > 1){
        printf("Il tensore con le nuove dimensioni deve essere 1D\n");
        exit(1);
    }
    //controllo che newShape non superi il limite di dimensioni impostato
    if(newShape.data.t->numElementi > MAX_DIM){
        printf("Impossibile effettuare il reshape, richiesto un numero di dimensioni maggiori del limite\n");
        exit(1);
    }

    for(int i = 0; i < newShape.data.t->numElementi; i++){
        if(newShape.data.t->data[i] < 0){
            printf("Non posso avere dimensioni negative\n");
            exit(1);
        }
    }

    int prod = 1;
    for(int i = 0; i < newShape.data.t->numElementi; i++){//scorro tutto newShape (tanto se sono qua so che è 1D)
        prod *= (int)newShape.data.t->data[i];//non posso avere un numero di elementi decimale
    }
    if(prod != el.data.t->numElementi){
        printf("Impossibile effettuare il reshape, dimensioni richieste non compatibili\n");
        exit(1);
    }
    //se sono qua vuol dire che posso farlo
    el.data.t->ndim = newShape.data.t->numElementi;//se newShape ha due elementi avrò due dimensioni
    for(int i = 0; i < el.data.t->ndim; i++){
        el.data.t->shape[i] = (int)newShape.data.t->data[i];//non posso avere valori di shape decimali
    }
    deleteTensor(newShape.data.t);
    pushTensor(el.data.t, s);
}

void matmulOperation(struct stack * s){
    struct stackElement el1 = pop(s);
    struct stackElement el2 = pop(s);

    if(el1.tipo != TENSOR_T || el2.tipo != TENSOR_T){
        printf("Il prodotto tra matrici si può fare solo tra tensori\n");
        exit(1);
    }

    if(el1.data.t->ndim != 2 || el2.data.t->ndim != 2){
        printf("I tensori per la moltiplicazione tra matrici devono essere a due dimensioni\n");
        exit(1);
    }

    //devo controllare che le dimensioni siano compatibili cioe che il numero di colonne di a sia uguale al numero di righe di b
    if(el1.data.t->shape[1] != el2.data.t->shape[0]){
        printf("Dimensioni delle matrici non compatibili per il prodotto\n");
        exit(1);
    }

    //se sono qua posso farlo
    struct tensor *ris = tensorMatmul(el1.data.t, el2.data.t);

    pushTensor(ris, s);
    deleteTensor(el1.data.t);
    deleteTensor(el2.data.t);
}

void appiattisciOperation(struct stack * s){
    struct stackElement el = pop(s);

    if(el.tipo != TENSOR_T){
        printf("L'operazione di ravel può essere effettuata solo su un tensore\n");
        exit(1);
    }
    //tengo il caso in cui il vettore abbia gia una dimensione tanto se è cosi non lo cambia
    el.data.t->ndim = 1;
    el.data.t->shape[0] = el.data.t->numElementi;
    pushTensor(el.data.t, s);
}

void shapeOperation(struct stack * s){
    struct stackElement el = pop(s);

    if(el.tipo != TENSOR_T){
        printf("E' possibile ottenere la forma solo di un tensore\n");
        exit(1);
    }
    //creo un tensore a una dimensione e che ha come shape il numero di dimensioni dell'originale
    //perche shape funziona che se il tensore originale è una matrice allora la sua shape sarà lunga 2
    int newShape[1] = {el.data.t->ndim};
    struct tensor * shapeTensor = createTensor(1, newShape);

    for(int i = 0; i < el.data.t->ndim; i++){
        shapeTensor->data[i] = (float)el.data.t->shape[i];
    }
    pushTensor(shapeTensor, s);
    deleteTensor(el.data.t);
}

void reluOperation(struct stack * s){
    struct stackElement el = pop(s);

    if(el.tipo != TENSOR_T){
        printf("L'operazione di relu si può fare solo su un tensore\n");
        exit(1);
    }

    struct tensor *ris = tensorRelu(el.data.t);

    pushTensor(ris, s);
    deleteTensor(el.data.t);
}

void minOperation(struct stack * s){
    struct stackElement el1 = pop(s);
    struct stackElement el2 = pop(s);

    if(el1.tipo != TENSOR_T || el2.tipo != TENSOR_T){
        printf("Il confronto si può fare solo tra due tensori\n");
        exit(1);
    }

    struct tensor *ris = tensorMin(el1.data.t, el2.data.t);

    pushTensor(ris, s);
    deleteTensor(el1.data.t);
    deleteTensor(el2.data.t);
}

void maxOperation(struct stack * s){
    struct stackElement el1 = pop(s);
    struct stackElement el2 = pop(s);

    if(el1.tipo != TENSOR_T || el2.tipo != TENSOR_T){
        printf("Il confronto si può fare solo tra due tensori\n");
        exit(1);
    }

    struct tensor *ris = tensorMax(el1.data.t, el2.data.t);

    pushTensor(ris, s);
    deleteTensor(el1.data.t);
    deleteTensor(el2.data.t);
}

void riduzioneOperation(struct stack * s){
    struct stackElement el = pop(s);

    if(el.tipo != TENSOR_T){
        printf("L'operazione di riduzione con somma si può fare solo su un tensore\n");
        exit(1);
    }
    
    struct tensor * ris = tensorReduction(el.data.t);

    pushTensor(ris, s);
    deleteTensor(el.data.t);
}

void fillOperation(struct stack * s){
    struct stackElement valori = pop(s);
    struct stackElement newShape = pop(s);
    //per rispettare la scrittura detta nelle istruzioni voglio che il primo estratto sia
    //il tensore che mi dice con che valori riempire il nuovo tensore
    //perchè così se io scrivo [2 3] [1 2] f il primo che estraggo sara [1 2] 

    if(valori.tipo != TENSOR_T || newShape.tipo != TENSOR_T){
        printf("Il fill richiede due tensori come parametri\n");
        exit(1);
    }

    if(newShape.data.t->ndim != 1){
        printf("Il tensore che dice quale sarà la nuova forma deve essere 1D\n");
        exit(1);
    }
    if(newShape.data.t->numElementi > MAX_DIM){
        printf("Non può essere creato un tensore con più dimensioni del massimo consentito\n");
        exit(1);
    }

    if(newShape.data.t->numElementi == 0){
        printf("Il tensore con le nuove dimensioni non può essere vuoto\n");
        exit(1);
    }

    if(valori.data.t->numElementi == 0){
        printf("Deve esserci almeno un valore con cui riempire\n");
        exit(1);
    }

    for(int i = 0; i < newShape.data.t->numElementi; i++){
        if(newShape.data.t->data[i] < 0){
            printf("Non posso avere dimensioni negative\n");
            exit(1);
        }
    }
    struct tensor *ris = tensorFill(valori.data.t, newShape.data.t);

    pushTensor(ris, s);
    deleteTensor(valori.data.t);
    deleteTensor(newShape.data.t);
}


void randomOperation(struct stack * s){
    struct stackElement newShape = pop(s);

    if(newShape.tipo != TENSOR_T){
        printf("La generazione di tensori random richiede un tensore che dia la forma\n");
        exit(1);
    }

    if(newShape.data.t->ndim != 1){
        printf("Il tensore che dice quale sarà la nuova forma deve essere 1D\n");
        exit(1);
    }
    if(newShape.data.t->numElementi > MAX_DIM){
        printf("Non può essere creato un tensore con più dimensioni del massimo consentito\n");
        exit(1);
    }

    if(newShape.data.t->numElementi == 0){
        printf("Il tensore con le nuove dimensioni non può essere vuoto\n");
        exit(1);
    }

    for(int i = 0; i < newShape.data.t->numElementi; i++){
        if(newShape.data.t->data[i] < 0){
            printf("Non posso avere dimensioni negative\n");
            exit(1);
        }
    }

    struct tensor *ris = tensorRandom(newShape.data.t);

    pushTensor(ris, s);
    deleteTensor(newShape.data.t);
}

void dotProductOperation(struct stack * s){
    struct stackElement el1 = pop(s);
    struct stackElement el2 = pop(s);

    if(el1.tipo != TENSOR_T || el2.tipo != TENSOR_T){
        printf("Il prodotto interno si può fare solo tra due tensori\n");
        exit(1);
    }

    if(el1.data.t->ndim > 1 || el2.data.t->ndim > 1){
        printf("Il prodotto interno può essere effettuato solo su vettori\n");
        exit(1);
    }

    if(el1.data.t->numElementi != el2.data.t->numElementi){
        printf("Il prodotto interno può essere effettuato solo tra vettori con le stesse dimensioni \n");
        exit(1);
    }

    struct tensor * ris = tensorDotProduct(el1.data.t, el2.data.t);

    pushTensor(ris, s);
    deleteTensor(el1.data.t);
    deleteTensor(el2.data.t);
}

void convoluzioneOperation(struct stack * s){
    struct stackElement kernel = pop(s);
    struct stackElement el = pop(s);

    if(kernel.tipo != TENSOR_T || el.tipo != TENSOR_T){
        printf("La convoluzione richiede una matrice (tensore) e un kernel (tensore)\n");
        exit(1);
    }

    if(kernel.data.t->ndim != 2 || el.data.t->ndim != 2){
        printf("La convoluzione richiede due matrici (due dimensioni)\n");
        exit(1);
    }

    //l'algoritmo che implementerò (scorrere il centro del kernel su tutta la matrice) funziona se
    //il kernel è una matrice quadrata e di lato dispari, quindi farò qui questo controllo
    if(kernel.data.t->shape[0] != kernel.data.t->shape[1]){
        printf("Si richiede che il kernel sia quadrato\n");
        exit(1);
    }

    if(kernel.data.t->shape[0] % 2 == 0){
        printf("Si richiede che il kernel abbia dimensioni dispari\n");
        exit(1);
    }

    struct tensor * ris = tensorConvolution(kernel.data.t, el.data.t);

    pushTensor(ris, s);
    deleteTensor(el.data.t);
    deleteTensor(kernel.data.t);
}

void leggiImmagineOperation(struct stack * s){
    struct stackElement filename = pop(s);

    if(filename.tipo != STRING_T){
        printf("L'operazione di lettura immagini richiede un path dato come stringa\n");
        exit(1);
    }

    FILE * imageFile = fopen(filename.data.str, "rb");
    if(imageFile == NULL){
        printf("Impossibile aprire l'immagine passata\n");
        exit(1);
    }
    char format[3];//uno spazio anche per il terminatore
    int width, height, maxcolor;

    //leggo l'header con fscanf
    if(fscanf(imageFile, "%s\n%d %d\n%d\n", format, &width, &height, &maxcolor) != 4){
        printf("Impossibile leggere l'header dell'immagine\n");
        exit(1);
    }
    //ora ho i valori che voglio nelle variabili e ho anche il cursore giusto dove iniziano i dati

    int newShape[2] = {height, width};//prima height perche rappresenta il numero di righe
    //creo il tensore con la shape presa dall'header del file 
    struct tensor * immagine = createTensor(2, newShape);
    int pixel;
    for(int i = 0; i < immagine->numElementi; i++){
        pixel = fgetc(imageFile);
        if(pixel == EOF){
            printf("Errore nella lettura dei pixel dell'immagine\n");
            exit(1);
        }
        immagine->data[i] = (float)pixel/maxcolor;
    }

    pushTensor(immagine, s);
    fclose(imageFile);
}

void scriviImmagineOperation(struct stack * s){
    struct stackElement filename = pop(s);
    struct stackElement el = pop(s);

    if(filename.tipo != STRING_T || el.tipo != TENSOR_T){
        printf("La funzione di scrittura di un'immagine richiede il nome di un file e un tensore\n");
        exit(1);
    }

    if(el.data.t->ndim != 2){
        printf("Il tensore da scrivere come immagine deve essere 2D\n");
        exit(2);
    }

    FILE * imageFile = fopen(filename.data.str, "wb");
    if(imageFile == NULL){
        printf("Impossibile aprire il file su cui scrivere\n");
        exit(1);
    }

    char *format = "P5";
    int height = el.data.t->shape[0];
    int width = el.data.t->shape[1];
    int maxval = 255;

    if(fprintf(imageFile, "%s\n%d %d\n%d\n", format, width, height, maxval) < 0){
        printf("Errore nella scrittura dell'header dell'immagine\n");
        exit(1);
    }
    //ora ho il cursore giusto dove devo iniziare a scrivere i pixel
    float pixel;
    for(int i = 0; i < el.data.t->numElementi; i++){
        pixel = el.data.t->data[i]*maxval;

        if(pixel < 0.0){
            pixel = 0;
        }
        if(pixel > maxval){
            pixel = maxval;
        }
        fputc((int)pixel, imageFile);
    }
    deleteTensor(el.data.t);
    fclose(imageFile);
}

void leggiTensoredaFileOperation(struct stack * s){
    struct stackElement filename = pop(s);

    if(filename.tipo != STRING_T){
        printf("L'operazione di lettura di un tensore da file richiede il path in formato di stringa\n");
        exit(1);
    }
    int fd = open(filename.data.str, O_RDONLY);

    //ora devo fare la mmap del file, mi serve la sua size
    struct stat buf;
    if(stat(filename.data.str, &buf) < 0){
        printf("Impossibile ottenere informazioni sul file\n");
        exit(1);
    }
    int size = buf.st_size;
    //lo assegno direttamente alla struct on_disk_tensor cosi potrò leggere direttamente i campi della struct
    //in pratica assegno il puntatore all'inizio del file al tipo struct on_disk_tensor cosi leggo gia i vari campi
    struct on_disk_tensor * header = (struct on_disk_tensor *)mmap(NULL, size, PROT_READ, MAP_SHARED, fd, 0);
    if(header == MAP_FAILED){
        printf("Errore nella mmap\n");
        close(fd);
        exit(1);
    }

    //ora leggendo dall'on_disk_tensor creo il tensore
    struct tensor * t = createTensor(header->ndim, header->shape);

    //ora so già la lunghezza dei vari campi e quindi so da dove iniziano i dati (leggendo offset)
    float *dati = (float *)((char *)header + header->data_offset);//uso l'aritmetica dei puntatori, header è l'inizio e vado avanti di offset
    //devo castarlo a char * che ha lunghezza 1 byte cosi mi fa il salto del numero giusto di byte

    //ora io dovrei mettere i dati nel campo data del tensore t, ma non voglio copiarli quindi semplicemente 
    //faccio puntare t->data all'inizio dei dati (prima libero la memoria che aveva allocato createTensor)

    free(t->data);
    t->data = dati;

    t->on_disk = true;
    t->fd = fd;
    t->offset = header->data_offset;

    pushTensor(t, s);
}

void scriviTensoresuFileOperation(struct stack * s){
    struct stackElement filename = pop(s);
    struct stackElement el = pop(s);
    if(filename.tipo != STRING_T || el.tipo != TENSOR_T){
        printf("L'operazione di scrittura di un tensore su file richiede un path e un tensore nello stack\n");
        exit(1);
    }
    //qua posso aprirlo con fopen perche tanto ci scrivo dentro e poi lo chiudo, penserà chi lo apre (come nella funzoine sopra)
    //ad aprirlo con open per avere l'fd
    FILE * file = fopen(filename.data.str, "wb");
    if(file == NULL){
        printf("Errore nell'apertura del file\n");
        exit(1);
    }

    struct on_disk_tensor header;
    //riempio i campi dell'header
    header.ndim = el.data.t->ndim;
    //ho riempito shape con i valori che ho quindi fino a ndim, la riempio anche di 0 fino a MAX_DIM
    //cosi poi nella fwrite scrivo tutto e in caso ho 0
    for(int i = 0; i < header.ndim; i++){
        header.shape[i] = el.data.t->shape[i];
    }
    for(int i = header.ndim; i < MAX_DIM; i++){
        header.shape[i] = 0;
    }
    header.data_offset = 64;
    //ora devo scrivere l'header sul file, lo scrivo direttamente tutto
    if(fwrite(&header, sizeof(struct on_disk_tensor), 1, file) != 1){
        printf("Errore nella scrittura dell'header\n");
        fclose(file);
        exit(1);
    }
    //dovrei fare il padding fino a data_offset, ma tanto data_offset mi dice solo dove iniziano i dati
    //quindi se io faccio fseek sposto solo il cursore a data_offset e inizio a scrivere da lì ed è equivalente
    if(fseek(file, header.data_offset, SEEK_SET) != 0){
        printf("Errore nella scrittura del padding\n");
        fclose(file);
        exit(1);
    }

    //ora posso scrivere i numeri
    if(fwrite(el.data.t->data, sizeof(float), el.data.t->numElementi, file) != el.data.t->numElementi){
        printf("Errore nella scrittura dei numeri nel tensore su file\n");
        fclose(file);
        exit(1);
    }
    fclose(file);
    deleteTensor(el.data.t);
}