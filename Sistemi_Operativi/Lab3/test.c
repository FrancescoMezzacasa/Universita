#include <stdio.h>
#include <string.h>

char *sostituisciComando(char * daSostituire,char * cosaSostituire,char * simboloDaTogliere, char * risultato){
    char padre[256];
    char figlio[256];
    char pattern[256];

    strcpy(padre, daSostituire);
    strcpy(figlio, cosaSostituire);
    strcpy(pattern, simboloDaTogliere);

    char pezzo_finale[256];
    char pezzo_iniziale[256];

    strcpy(pezzo_finale, strstr(padre, pattern));
    
    int lunghezza_pezzo_iniziale = strlen(padre)-strlen(pezzo_finale);
    strncpy(pezzo_iniziale, padre, lunghezza_pezzo_iniziale);
    pezzo_iniziale[lunghezza_pezzo_iniziale] = '\0';

    strcpy(risultato, pezzo_iniziale);
    strcat(risultato, figlio);

    int l = strlen(risultato);
    int j = 0;
    for (int i = lunghezza_pezzo_iniziale; i < strlen(padre); i++)
    {   
        if(padre[i] != '%'){
            risultato[l + j] = padre[i];
            j++;
        }
    }
    risultato[l+j] = '\0';
    
    return(risultato);
}

int main (int argc, char *argv[]) {
    char risultato[256];
    sostituisciComando(argv[1], argv[2], argv[3], risultato);
    puts(risultato);
    return 0;
}

