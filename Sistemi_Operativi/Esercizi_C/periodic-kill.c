#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

int main (int argc, char *argv[]) {
    int segnale = atoi(argv[1]);
    int interval = atoi(argv[2]);
    int nproc = argc - 3;
    int * pids = (int *)malloc(sizeof(int)*nproc);
    for (int i = 0; i < nproc; i++)
    {
        pids[i] = atoi(argv[i+3]);
    }

    while(1){
        sleep(interval);
        for (int i = 0; i < nproc; i++)
        {
            kill(pids[i], segnale);
            printf("Inviato %s a %d\n", argv[1], pids[i]);
        }
        
    }
    
    free(pids);
    return 0;
}