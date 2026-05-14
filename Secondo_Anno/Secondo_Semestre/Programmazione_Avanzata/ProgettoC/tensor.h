//FRANCESCO MEZZACASA SM3201587
#ifndef TENSOR_H
#define TENSOR_H

#define MAX_DIM 2

#include <stdbool.h>
#include <sys/types.h>

struct tensor{
    int shape[MAX_DIM]; //array con la forma
    int ndim; //quante dimensioni ha

    int numElementi;//da quanti elementi totali è formato (es. matrice 2 x 3 = 6 elementi)

    float * data; //numeri che ci sono dentro
    int ref_count;//da quanti puntatori è puntato

    bool on_disk; //flag per dire se è fatto su disco con mmap
    int fd;
    off_t offset; //offset da cui inizia nel file fd
};

struct tensor * createTensor(int ndim, int * shape);
void deleteTensor(struct tensor * t);

void printTensor(struct tensor * t);

struct tensor *tensorSum(struct tensor * t1, struct tensor * t2);
struct tensor *tensorSubtraction(struct tensor * t1, struct tensor * t2);
struct tensor *tensorProduct(struct tensor * t1, struct tensor * t2);

struct tensor *tensorMaggiore(struct tensor * t1, struct tensor * t2);
struct tensor *tensorMinore(struct tensor * t1, struct tensor * t2);
struct tensor *tensorUguale(struct tensor * t1, struct tensor * t2);

struct tensor *tensorAndLogico(struct tensor * t1, struct tensor * t2);
struct tensor *tensorOrLogico(struct tensor * t1, struct tensor * t2);
struct tensor *tensorNotLogico(struct tensor * t);

struct tensor *tensorSelection(struct tensor * t1, struct tensor * t2, struct tensor * mask);

struct tensor *tensorMatmul(struct tensor * t1, struct tensor * t2);

struct tensor *tensorRelu(struct tensor * t);
struct tensor *tensorMin(struct tensor * t1, struct tensor * t2);
struct tensor *tensorMax(struct tensor * t1, struct tensor * t2);

struct tensor *tensorReduction(struct tensor * t);

struct tensor *tensorFill(struct tensor * valori, struct tensor * newShape);

struct tensor *tensorRandom(struct tensor * newShape);

struct tensor * tensorDotProduct(struct tensor * t1, struct tensor * t2);
struct tensor * tensorConvolution(struct tensor * kernel, struct tensor * matrice);
#endif