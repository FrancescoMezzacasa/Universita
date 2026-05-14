#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>

#include "netpbm.h"

int open_image(char * path, netpbm_ptr img)
{
  //devo riempire tutti i campi
  FILE * file = fopen(path, "r+b");
  if(file == NULL){
    printf("Errore nell'apertura del file\n");
    return -1;
  }
  //riempio campo fd
  img->fd = file;

  struct stat buf;
  if(stat(path, &buf) < 0){
    printf("Impossibile ottenere informazioni sul file\n");
    return -1;
  }
  //riempio il campo size
  int size = buf.st_size;
  img->size = size;

  char formato[3];
  int width;
  int height;
  int maxpixelvalue;

  //riempio width e height
  fscanf(file, "%s %d %d %d", formato, &width, &height, &maxpixelvalue);
  //printf("%d %d\n", width, height);
  img->width = width;
  img->height = height;

  //ora il cursore è subito dopo il 255 ma come detto dal testo c'è un ritorno a capo che devo togliere
  fgetc(file);
  //ora il cursore è nella posizione giusta quindi vedo dov'è e quello è l'offset
  //riempio il campo offset
  img->offset = ftell(file);

  //ora devo riempire data con mmap
  int fd = fileno(file);
  img->data = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if(img->data == MAP_FAILED){
    printf("Fallita la mmap\n");
    return -1;
  }

  return 0;
}

int empty_image(char * path, netpbm_ptr img, int width, int height)
{
  FILE * fd = fopen(path, "w+");
  if (fd == NULL) {
    return -1;
  }
  int written = fprintf(fd, "P5\n%d %d\n255\n", width, height);
  ftruncate(fileno(fd), written + width * height);
  fclose(fd);
  return open_image(path, img);
}

char * pixel_at(netpbm_ptr img, int x, int y)
{
  if(x < 0 || x >= img->width){
    return NULL;
  }
  if(y < 0 || y >= img->height){
    return NULL;
  }
  
  return &(img->data[y * img->width + x + img->offset]);
}

int close_image(netpbm_ptr img)
{
  munmap(img->data, img->size);
  fclose(img->fd);
  //svuoto anche la struct
  img->data = NULL;
  img->fd = NULL;
  img->width = 0;
  img->height = 0;
  img->offset = 0;
  img->size = 0;
  return 0;
}
