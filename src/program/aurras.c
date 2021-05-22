#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "model/request.h"

#define BUFFSIZE 1024


/* Client, to run client ./aurras -> show how to use, ./aurras status -> show status , ./aurras transform [....] -> submit request */
int main(int argc,char * argv[]){
    int size_read;
    char pid[50];
    char * request_message;
    char buff[BUFFSIZE];

    sprintf(pid,"%d",getpid());

    if(argc==1){
        printf("Executing modes\n");
        printf("./aurras status\n");
        printf("./aurras transform input-filename output-filename filter-id-1 filter-id-2 ...\n");
        _exit(1);
    }


    if(strcmp(argv[1],"status") != 0 && strcmp(argv[1],"transform") !=0 ){
        printf("Invalid mode\n");
        _exit(1);
    }

    if(strcmp(argv[1],"transform") == 0 && argc < 5){
        printf("Not enougth arguments\n");
    }

    if(mkfifo(pid,0666) == -1){ // create pipe for READING anwser from the server
        perror(pid);
    }

    int c2s_pipe = open("client2server",O_WRONLY); // open pipe for WRITING requests to de server
    if(c2s_pipe == -1){
        printf("Server offline...");
        _exit(-1);
    }

    Request req = create_request(pid,argv+1);
    request_message = strdup(req_toString(req));
    
    write(c2s_pipe,request_message,strlen(request_message)+1); // send the request message to the server

    int s2c_pipe = open(pid,O_RDONLY); // opens pipe for READING anwser from the server

    
    while((size_read = read(s2c_pipe,&buff,BUFFSIZE)) != EOF){
       write(1,buff,size_read);
    }

    return 0;
}