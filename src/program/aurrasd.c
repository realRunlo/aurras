#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "model/reading.h"
#include "model/structs.h"
#include "model/request.h"
#include "model/filter.h"
#include "model/task.h"

int canRun(Task t,List filters){
    Request req = getRequest(t);
    char * line = getCommand(req);
    strsep(&line," "); //transform
    strsep(&line," "); //input
    strsep(&line," "); //output
    char * seped;
    while((seped = strsep(&line," "))){
        if(!canUse_filter(filters,seped)){ // if can not use filter return false = 0
            return 0;
        }
    }

    return 1;
}

/* Server, to run server ./aurrasd config-filename filter-folder*/
int main(int argc,char * argv[]){
    List filters; 
    List waiting_queque;
    List runing_queque;
    int task_counter = 1;
    int r_size;
    char * filters_folder;
    int c2s_pipe,s2c_pipe;

    if(argc<2){
        printf("Not enougth arguments\n");
        _exit(1);
    }
    
    /* init the server */
    filters = load_server_configs(argv[1]); // load the the filters in the configs file to a list
    filters_folder = strdup(argv[2]); // save the execs folders name
    
    waiting_queque = initList(); // queque

    runing_queque = initList(); 

    printf("Server online\n");

    if(mkfifo("client2server",0666) == -1){ // create pipe for READING clients requests
        perror("client2server");
    }


    c2s_pipe = open("client2server", O_RDONLY); // open pipe for READING clients requests

    char buffer[1024];

    while((r_size = read(c2s_pipe,&buffer,1024)) >= 0){

        Request req = toReq(buffer);
        if(strcmp(getCommand(req),"status") == 0 ){
            s2c_pipe = open(getId(req),O_WRONLY);
            dup2(s2c_pipe,1);
            show_taskList(runing_queque);
            show_filterList(filters);
            close(s2c_pipe);
        }else{
            Task t = create_task(task_counter,req);
            task_counter++;

            s2c_pipe = open(getId(req),O_WRONLY);
            waiting_queque = push(waiting_queque,t);
            write(s2c_pipe,"pending\n",12);

            if(canRun(t,filters)){
                t = (Task) getValue(waiting_queque,0);
                runing_queque = push(runing_queque,t); // add to the runing queque
                waiting_queque = pop(waiting_queque); // remove from waiting
                update_runingFilters(filters,t,0);
                
                switch (fork())
                {
                case -1:
                    perror("fork");
                    break;
                case 0:
                    req = getRequest(t);
                    write(s2c_pipe,"processing...",14);
                    //execs etc....

                    update_runingFilters(filters,t,1);
                    //runing_queque = pop(runing_queque);
                default:
                    // children will exec so doesnt need waiting
                    break;
                }
            }
            
     
        }


    }


    return 0;
}