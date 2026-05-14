#include <stdio.h>
#include <stdlib.h>

int main(){
    int w = 3;
    int h = 3;
    int m[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    int cumsum[16];
    for (int i = 0; i < 16; i++)
    {
        cumsum[i] = 0;
    }
    for(int i = 1; i < h + 1; i++){
        for(int j = 1; j < w + 1; j++){
            cumsum[i*(w+1)+j] = cumsum[i*(w+1) + (j-1)] - cumsum[(i-1)*(w+1) + (j-1)] + cumsum[(i-1)*(w+1) + j] + m[(i-1)*w + (j-1)];
        }
    }
    for(int i = 0; i < h; i++){
        for(int j = 0; j < w; j++){
            printf("%d ", m[i*w+j]);
        }
        printf("\n");
    }
    printf("\n");
    for(int i = 0; i < h + 1; i++){
        for(int j = 0; j < w + 1; j++){
            printf("%d ", cumsum[i*(w+1)+j]);
        }
        printf("\n");
    }
    return 0;
}