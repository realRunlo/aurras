#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../includes/filter.h"
#include "../includes/reading.h"

int read_char(int fd,char *c){

   return read(fd,c,sizeof(char));

}

ssize_t readln(int fd, char *line, size_t size){
  
    ssize_t res= 0;
    int i=0;

    while(i<size && (res=read_char(fd,&line[i]) >0)){

        if(line[i]=='\n'){
            i += res;
            return i;
         }
        i += res;
        

    }

    return i;

}

List load_server_configs(char * filename){

    int fd = open(filename,O_RDONLY,0666);
    if(fd==-1){
        perror("configs");
        _exit(1);
    }

    List configs = initList();
    char buffer[1024];
    int size_read;
    while((size_read = readln(fd,&buffer,1024) ) > 0){
       Filter f = initF(buffer);
       configs = push(configs,f);
    }

    close(fd);
    return configs;

}


