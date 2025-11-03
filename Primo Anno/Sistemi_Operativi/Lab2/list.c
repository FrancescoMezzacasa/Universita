#include <stdio.h>
#include <sys/stat.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <string.h>

int lista(char* path, int primaVoltaCheChiamo){
    //devo trasformare il path nel path completo
    DIR * cartella;
    cartella = opendir(path);
    if(cartella == NULL){
        printf("Errore nell'apertura della cartella\n");
        return 1;
    }
    struct dirent *elemento;
    struct stat info;
    int gia_visto_punto = 0;//cosi stampo il punto solo all inizio
    while(((elemento = readdir(cartella)) != NULL)){
        //printf("ciao sono entrato nel while\n");
        if((strcmp(elemento ->d_name, "..") != 0)){
            
            char path_completo[1024];
            if((strcmp(elemento ->d_name, ".") == 0) && (primaVoltaCheChiamo == 1)){
                if(gia_visto_punto == 1){//se ho gia visto punto lo skippo
                    continue;
                }else{
                    gia_visto_punto = 1;//se non l avevo visto metto a 1 il flag ma continuo normale
                    snprintf(path_completo, sizeof(path_completo), "%s", path);//se sono nella cartella corrente il path e solo il nome
                    printf("path completo1: %s\n", path_completo);
                }
            }

            if((strcmp(elemento ->d_name, ".") == 0) && (primaVoltaCheChiamo == 0)){
                continue;
            }


            if(strcmp(elemento ->d_name, ".") != 0){
                snprintf(path_completo, sizeof(path_completo), "%s/%s", path, elemento->d_name);
            }
            //printf("ciao sono entrato nell'if\n");
            //printf("nome: %s\n", elemento->d_name);
            if(stat(path_completo, &info) == -1){
                printf("Path completo: %s\n", path_completo);
                perror("Errore nella stat");
                return 1;
            }
            

            //devo ottenere il nome dell'utente
            struct passwd *utente;
            utente = getpwuid(info.st_uid);//qua ci trovo il nome
            char *nome = utente ->pw_name;

            //ora faccio la stessa cosa per il gruppo
            struct group *gruppo;
            gruppo = getgrgid(info.st_gid);
            char *nome_gruppo = gruppo ->gr_name;

            char *tipo;
            if((info.st_mode & __S_IFMT) == __S_IFREG){
                tipo = "file";
            } else if((info.st_mode & __S_IFMT) == __S_IFDIR){
                tipo = "directory";
            } else if((info.st_mode & __S_IFMT) == __S_IFLNK){
                tipo = "symbolic link";
            } else if((info.st_mode & __S_IFMT) == __S_IFIFO){
                tipo = "FIFO";
            } else{
                tipo = "other";
            }

            printf("Node: %s\n", path_completo);
            printf("\tInode: %lu\n", info.st_ino);
            printf("\tType: %s\n", tipo);
            printf("\tSize: %ld\n", info.st_size);
            printf("\tOwner: %d %s\n", info.st_uid, nome);
            printf("\tGroup: %d %s\n", info.st_gid, nome_gruppo);

            //ora devo fare la ricorsione
            if(strcmp(tipo, "directory") == 0 && strcmp(elemento ->d_name, ".") != 0){
                lista(path_completo, 0);
            }
        }   
    }
    return 0;
}

int main (int argc, char *argv[]){
    if(argc != 2){
        printf("Inserire il path nel terminale\n");
        return 1;
    }
    char *path = argv[1];
    
    lista(path, 1);
    return 0;
}