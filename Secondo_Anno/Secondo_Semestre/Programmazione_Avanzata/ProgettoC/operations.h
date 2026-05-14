#ifndef OPERATIONS_H
#define OPERATIONS_H

#define MAX_STRING 1024

struct on_disk_tensor {
    int32_t shape[MAX_DIM];
    int32_t ndim;
    off_t data_offset;
};

void printOperation(struct stack * s);

//operazioni su stack
void dupOperation(struct stack * s);
void dropOperation(struct stack * s);
void swapOperation(struct stack * s);
void overOperation(struct stack *s);

//operazioni aritmetiche
void sommaOperation(struct stack * s);
void sottrazioneOperation(struct stack * s);
void prodottoOperation(struct stack * s);

//operazioni di comparazione
void maggioreOperation(struct stack * s);
void minoreOperation(struct stack * s);
void ugualeOperation(struct stack * s);

//operazioni logiche
void andOperation(struct stack * s);
void orOperation(struct stack * s);
void notOperation(struct stack * s);

//selezione
void selezioneOperation(struct stack * s);

//operazioni su forma dei tensori
void reshapeOperation(struct stack * s);
void appiattisciOperation(struct stack * s);
void shapeOperation(struct stack * s);

//operazioni specifiche per tensori
void matmulOperation(struct stack * s);
void dotProductOperation(struct stack * s);
void convoluzioneOperation(struct stack * s);

//operazioni elemento per elemento
void reluOperation(struct stack * s);
void minOperation(struct stack * s);
void maxOperation(struct stack * s);

//riduzione
void riduzioneOperation(struct stack * s);

//fill
void fillOperation(struct stack * s);

//random
void randomOperation(struct stack * s);

//operazioni IO
void leggiImmagineOperation(struct stack * s);
void scriviImmagineOperation(struct stack * s);
void leggiTensoredaFileOperation(struct stack * s);
void scriviTensoresuFileOperation(struct stack * s);

#endif