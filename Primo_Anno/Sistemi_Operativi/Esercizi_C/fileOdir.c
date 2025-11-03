#include <stdio.h>
#include <sys/stat.h>

int main (int argc, char *argv[]) {
    if(argc != 2){
        printf("Inserire il path nel terminale\n");
        return 1;
    }
    char *path = argv[1]; //non char path[] perché servirebbe una dimensione fissa ma non la sa
    struct stat info;

    if(stat(path, &info) == -1){
        perror("Errore");
        return 1;
    }

    if(S_ISREG(info.st_mode))
        printf("%s è un file\n", path);
    else if(S_ISDIR(info.st_mode))
        printf("%s è una directory\n", path);
    else if(S_ISLNK(info.st_mode))
        printf("%s è un link\n", path);
    else
        printf("%s non è ne un file ne una directory ne un link\n", path);

    return 0;
}