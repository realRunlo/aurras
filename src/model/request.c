#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "model/request.h"

/**
 * @brief Stores the resquest sent from client
 * 
 */
struct request{

    char * id; //pid of the client
    char * command;
};

void setId(Request req, char *id) {
    if(req != NULL && id != NULL) {
            req->id = strdup(id);
    }
}

char * getId(Request req){
    return strdup(req->id);
}

void setCommand(Request req, char *command) {
    if(req != NULL && command != NULL)
        req->command = strdup(command);
}

char * getCommand(Request req) {
    return strdup(req->command);
}

Request init_request(char *id, char *command) {
    Request req = NULL;

    if(id != NULL && command != NULL) {
        req = malloc(sizeof(struct request));
        setId(req, id);
        setCommand(req, command);
    }
    return req;
}

Request create_request(char *  id,char ** command){
    Request rq = NULL;
    int size = 0;

    for(int i = 0;command[i] != NULL;i++){
        size += strlen(command[i]);
        size++;
    }
    size--;
    char * str = malloc(sizeof(char) * size);
    for(int i =0;command[i] != NULL;i++){
        strcat(str,command[i]);
        if(command[i+1] != NULL)
            strcat(str,";");
    }
    
    rq = init_request(id, str);
    free(str);
    return rq;
}

char * req_toString(Request req){
    int size = strlen(getId(req)) + strlen(getCommand(req)) + 2;
    char *str = malloc(sizeof(char) * size);  

    snprintf(str,size,"%s;%s\n",getId(req),getCommand(req));
    
    return  str;
}

Request toReq(char * str){
    char *id = strdup(strsep(&str,";"));
    char *command = strdup(strsep(&str,"\n"));
    Request req = init_request(id, command);

    return req;
}
