#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../includes/request.h"


struct request{

    char * id; //pid of the client
    char * command;
};

Request create_request(char *  id,char ** command){

    Request rq = malloc(sizeof(struct request));
    rq->id = strdup(id);
    int size = 0;
    for(int i = 0;command[i] != '\0';i++){
        size += strlen(command[i]);
        size++;
    }
    size--;
    char * str = malloc(sizeof(char) * size);
    for(int i =0;command[i] != '\0';i++){
        strcat(str,command[i]);
        if(command[i+1] != '\0')
            strcat(str,";");
    }
    rq->command = strdup(str);
    free(str);
    return rq;
}

char * getId(Request req){
    return strdup(req->id);
}

char * getCommand(Request req){
    return strdup(req->command);
}

char * req_toString(Request req){
    int size = strlen(getId(req)) + strlen(getCommand(req)) + 2;
    char * str;  
    snprintf(str,size,"%s;%s",getId(req),getCommand(req));
    
    return  str;
}

Request toReq(char * str){
    Request req = malloc(sizeof(struct request));
    req->id = strsep(&str,";");
    req->command = strsep(&str," ");

    return req;
}
