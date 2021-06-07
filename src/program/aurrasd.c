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
#include <signal.h>


int canRun(Task t,List filters){
    Request req = get_task_request(t);
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
    List waiting_queque = NULL;
    List runing_queque = NULL;
    List filters_toExec = NULL;
    int save_1;
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

    if(mkfifo("client2server",0666) == -1){ // create pipe for READING clients requests
        perror("client2server");
    }

    printf("Server online\n");

    c2s_pipe = open("client2server", O_RDONLY); // open pipe for READING clients requests


    open("client2server", O_WRONLY);//keeps the server open without clients
                                    //better then using >= in while cicle for ressons explained in class


    char buffer[1024];

    while((r_size = read(c2s_pipe,&buffer,1024)) > 0){

        Request req = toReq(buffer); //read request from the pipe
        char * req_command = getCommand(req); 
        if(strcmp(req_command,"status") == 0 ){
            s2c_pipe = open(getId(req),O_WRONLY);
            save_1 = dup(1);
            dup2(s2c_pipe,1);
            show_taskList(runing_queque);
            show_filterList(filters);
            dup2(save_1,1);
            close(s2c_pipe);
            kill(atoi(getId(req)),SIGINT); //closes client after printing status
        }else if (strcmp(strsep(&req_command,";"),"transform")==0) {
            Task t = create_task(task_counter,req);
            task_counter++;

            s2c_pipe = open(getId(req),O_WRONLY);
            waiting_queque = push(&waiting_queque,t);
            write(s2c_pipe,"pending...\n",12);
            
            if(canRun(t,filters)){
                t = (Task) getValue(waiting_queque,0);
                write(s2c_pipe,"processing...\n",15);
                waiting_queque = pop(waiting_queque);// remove from waiting queque
                filters_toExec = creatExecsQueque(filters,t); //creats list of filters to aply
                runing_queque = push(&runing_queque,t); // add to the runing queque
                update_runingFilters(filters,t,0);
                
                if(fork()){
                    int n_filtersToExec = get_sizel(filters_toExec);
                    int pids[n_filtersToExec];
                    int pid;
                    for(int i=0;i<n_filtersToExec;i++){//launch all de execs
                        Filter exF = (Filter) getValue(filters_toExec,0);
                        char * execName = get_exec_name(exF);
                        char * inFile = getInFile(t);
                        char * outFile = getOutFile(t);
                        filters_toExec = pop(filters_toExec);
                    
                        if((pid = fork())==0){
                            execlp(execName,execName,inFile,outFile,NULL);
                        }
                        pids[i] = pid; //store pid of the processes launched to exec
                    }

                    //wait for all the execs
                    for (int i = 0; i < n_filtersToExec; ++i) {
                        int status;
                        while (-1 == waitpid(pids[i], &status, 0)); 
                    }

                    /*
                        NEED to find a way for updating status in the father process
                    */
                    update_runingFilters(filters,t,1);
                }
                //limpar a lista de filtros para execs
                
            } 
            
        }
    }
    return 0;
}