#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <wait.h>

int fattoriale(int n){
    if(n == 0 || n == 1){
        return 1;
    }
    return(n*fattoriale(n-1));
}

int main (int argc, char *argv[]) {
    FILE * file = fopen(argv[1], "r");
    int numeri[1024];
    int conto = 0;
    while(fscanf(file, "%d", &numeri[conto]) != EOF){
        conto++;
    }
    //quando sono qua ho la lista con i numeri e il conto di quanti sono
    for (int i = 0; i < conto; i++)
    {
        if(fork() == 0){//figlio
            printf("Il fattoriale di %d è %d\n", numeri[i], fattoriale(numeri[i]));
            exit(0);
        }
        //il padre nel for non serve che faccia niente
    }
    //quando arrivo qua i figli hanno stampato tutti i fattoriali
    //devo aspettarli
    for (int i = 0; i < conto; i++)
    {
        wait(NULL);
    }
    
    //ora devo stampare numero massimo e riga
    int maxindex = 0;
    int maxnum = numeri[0];
    for (int i = 0; i < conto; i++)
    {
        if(numeri[i] > maxnum){
            maxnum = numeri[i];
            maxindex = i;
        }
    }
    
    printf("Il numero massimo è %d e si trova alla riga %d\n", maxnum, maxindex + 1);
    return 0;
}