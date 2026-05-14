#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <omp.h>

#include "netpbm.h"
#include "image_transform.h"

int blur_image(netpbm_ptr src, char *path, netpbm_ptr dst) {
    if (src == NULL || dst == NULL || path == NULL) {
        return -1;
    }
    //creo l'immagine di destinazione
    if(create_output_image(path, src, dst, RADIUS_BLUR) != 0){
        printf("Errore nella create_output_image\n");
        return -1;
    }
    int r = RADIUS_BLUR;
    #pragma omp parallel for collapse(2)
    //itero su tutti i pixel della sorgente
    for(int x = r; x < src->width - r; x++){
        for(int y = r; y < src->height - r; y++){
            //ora per ognuno guardo quelli intorno
            unsigned long long sum = 0;
            for(int i = -r; i <= r; i++){
                for(int j = -r; j <= r; j++){
                    sum += (unsigned char)*pixel_at(src, x+i, y+j);
                }
            }
            //printf("%lld\n", sum);
            *pixel_at(dst, x-r, y-r) = (char)(sum/((2*r + 1)*(2*r + 1)));
        }
    }
    printf("Ho finito il blur\n");
    return 0;
}

unsigned long long *build_prefix_sum(const unsigned char *src_data, int w, int h) {

    unsigned long long *prefix = calloc((size_t)(h + 1) * (w + 1), sizeof(unsigned long long));

    if (prefix == NULL) {
        return NULL;
    }
    //calloc fornisce gia memoria inizializzata a 0
    for(int i = 1; i < h + 1; i++){
        for(int j = 1; j < w + 1; j++){
            //sommo quello a sinistra e quello in alto, ma togliendo il pezzo contato due volte (quello in comune)
            //e aggiungendo il valore mancante
            prefix[i*(w+1)+j] = prefix[i*(w+1) + (j-1)] - prefix[(i-1)*(w+1) + (j-1)] + prefix[(i-1)*(w+1) + j] + src_data[(i-1)*w + (j-1)];
        }
    }
    printf("Ho costruito la matrice\n");
    return prefix;
}


int edge_highlight(netpbm_ptr src, char *path, netpbm_ptr dst) {
    printf("Sono entrato in edge_highlights\n");
    if (src == NULL || dst == NULL || path == NULL) {
        return -1;
    }
    //creo l'immagine di destinazione
    if(create_output_image(path, src, dst, RADIUS_BORDER_HIGHLIGHT) != 0){
        printf("Errore nella create_output_image\n");
        return -1;
    }
    int w = src->width;
    int h = src->height;
    unsigned long long *prefix = build_prefix_sum((const unsigned char*)src->data, w, h);
    if(prefix == NULL){
        printf("Errore nell'allocazione\n");
    }
    int r = RADIUS_BORDER_HIGHLIGHT;
    printf("Sono prima del pragma\n");
    //itero su tutti i pixel della destinazione
    #pragma omp parallel for collapse(2)
    for(int y = 0; y < dst->height; y++){
        for(int x = 0; x < dst->width; x++){
            //i for fatti cosi perche cosi uso coordinate x e y come gli assi cartesiani
            unsigned long long sum = 0;
            long double media = 0.0;

            //creo le coordinate della sorgente (shiftate)
            int src_x = x+r;
            int src_y = y+r;

            unsigned long long A = prefix[(src_y - r) * (w + 1) + (src_x - r)];
            unsigned long long B = prefix[(src_y - r) * (w + 1) + (src_x + r + 1)]; 
            unsigned long long C = prefix[(src_y + r + 1) * (w + 1) + (src_x - r)]; 
            unsigned long long D = prefix[(src_y + r + 1) * (w + 1) + (src_x + r + 1)];
            //ci sono dei +1 perche in basso e a destra ho gli zeri da tenere da conto
            sum = D - C - B + A;
            media = sum/((2*r + 1)*(2*r + 1));
            if(media >= (unsigned char)*pixel_at(src, x+r, y+r)){
                *pixel_at(dst, x, y) = (unsigned char)255;
            }else{
                *pixel_at(dst, x, y) = (unsigned char)0;
            }
        }
    }
    printf("HO finito l edge\n");
    free(prefix);
    return 0;
}