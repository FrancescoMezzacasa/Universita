#include <stdio.h>

int main (int argc, char *argv[]){
    int a, b, maxa, maxb = 0;
    FILE * file = fopen(argv[1], "rb");
    char buffer[1024];
    do
    {
        printf("Inserire il carattere di partenza: ");
        scanf("%d", &a);
        printf("Inserire la lunghezza da leggere: ");
        scanf("%d", &b);
        if(b > maxb){
            maxa = a;
            maxb = b;
        }
        //sposto il cursore di a caratteri cosi salto i primi
        fseek(file, a, SEEK_SET);

        int quantiletti = fread(buffer, sizeof(char), b, file);
        printf("Ho letto %d byte\n", quantiletti);
        
        for (int i = 0; i < b; i++)
        {
            printf("%x ", buffer[i]);
        }
        printf("\n");
        
    } while (b != 0);

    printf("La coppia con b maggiore è: (%d, %d)\n", maxa, maxb);
    
    return 0;
}