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

int updatePipe[2];
List filters; 
List waiting_queque;
List runing_queque;
List filters_toExec;
pid_t pid_father;
int c2s_pipe;


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



void update_handler(int signum){
    printf("updating status...\n");
    
    char buff[100];
    read(updatePipe[0],&buff,100);
    
    int pos = get_tasks_pos(runing_queque,atoi(buff));
    Task t = removeValue(runing_queque,pos); //remove task from ruining queque
    update_runingFilters(filters,t,1); //update filters

}

void close_handler(int signum){
    close(c2s_pipe);
    printf("\nServer offline.\n");
}

/* Server, to run server ./aurrasd config-filename filter-folder*/
int main(int argc,char * argv[]){
    signal(SIGUSR1,update_handler);
    signal(SIGINT,close_handler);
    pipe(updatePipe);
    filters = NULL;
    waiting_queque = NULL;
    runing_queque = NULL;
    filters_toExec = NULL;

    pid_father = getpid();
    printf("%d\n",pid_father);
    int save_1;
    int task_counter = 1;
    int r_size;
    char * filters_folder;
    int s2c_pipe;


    if(argc<2){
        printf("Not enougth arguments\n");
        _exit(1);
    }
    
    /* init the server */
    filters = load_server_configs(argv[1]); // load the the filters in the configs file to a list
    filters_folder = strdup(argv[2]); // save the execs folders name

    if(mkfifo("tmp/client2server",0666) == -1){ // create pipe for READING clients requests
        perror("client2server");
    }

    printf("Server online\n");

    c2s_pipe = open("tmp/client2server", O_RDONLY); // open pipe for READING clients requests


    open("tmp/client2server", O_WRONLY);//keeps the server open without clients
                                    //better then using >= in while cicle for ressons explained in class


    char buffer[1024];

    while((r_size = read(c2s_pipe,&buffer,1024)) > 0){

        Request req = toReq(buffer); //read request from the pipe
        char * req_command = getCommand(req); 
        if(strcmp(req_command,"status") == 0 ){
            char pipeName[20];
            sprintf(pipeName,"tmp/%s",getId(req));
            s2c_pipe = open(pipeName,O_WRONLY);
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
            char pipeName[100];
            sprintf(pipeName,"tmp/%s",getId(req));
            s2c_pipe = open(pipeName,O_WRONLY);
            waiting_queque = push(&waiting_queque,t);
            write(s2c_pipe,"pending...\n",12);

            if(canRun(t,filters)){
                t = (Task) getValue(waiting_queque,0);
                write(s2c_pipe,"processing...\n",15);
                waiting_queque = pop(waiting_queque);// remove from waiting queque
                filters_toExec = creatExecsQueque(filters,t); //creats list of filters to aply
                runing_queque = push(&runing_queque,t); // add to the runing queque
                update_runingFilters(filters,t,0);
 

                int inFp = open(getInFile(t),O_RDONLY);
                int outFp = open(getOutFile(t),O_CREAT | O_WRONLY);

                if(fork()==0){

                    close(updatePipe[0]); //close updatePipe for reading
                    
                    int n_filtersToExec = get_sizel(filters_toExec);
                    int pids[n_filtersToExec];
                    int pid,res_pipe=0;
                    int arrayPipes[n_filtersToExec-1][2];
                    printf("%d\n",n_filtersToExec);
                    for(int i=0;i<n_filtersToExec;i++){//launch all de execs
                        
                        Filter exF = (Filter) getValue(filters_toExec,i);
                        char * execName = get_exec_name(exF);
                        printf("%s\n",execName);
                        char folder[100];
                        sprintf(folder,"%s/%s",filters_folder,execName);

                        if(n_filtersToExec==1){
                            switch (pid = fork())
                            {
                            case -1:
                                perror("fork");
                                _exit(-1);
                            case 0:

                                dup2(inFp,0); //exec will receive the input from stdin
                                dup2(outFp,1);
                                execlp(folder,execName,NULL);
                                _exit(-1);
                            default:
                                break;
                                
                            }


                        }
                        /*else{
                             if(i==0){ // first exec
                            pipe(arrayPipes[i]);
                            if(res_pipe==-1){
                             printf("Error pipe %d:",i);
                            }

                            switch (pid = fork())
                            {
                            case -1:
                                perror("fork");
                                _exit(-1);
                            case 0:
                                close(arrayPipes[i][0]);
                                dup2(arrayPipes[i][1],1);
                                close(arrayPipes[i][1]);
                                dup2(inFp,0); //exec will receive the input from stdin

                                execlp(folder,execName,NULL);
                                _exit(-1);
                            default:
                                close(arrayPipes[i][1]);
                                
                            }

                        }else if(i==n_filtersToExec-1){ // last exec

                            switch (pid = fork())
                            {
                            case -1:
                                perror("fork");
                                _exit(-1);
                            case 0:
                                dup2(arrayPipes[i-1][0],0);
                                close(arrayPipes[i-1][0]);
                                dup2(outFp,1);// prints de output in the output file
                                //execlp(folder,execName,NULL);
                                _exit(-1);
                            default:
                                close(arrayPipes[i-1][1]);
                                
                            }
                        }else{ // mid execs
                            switch (pid = fork())
                            {
                            case -1:
                                perror("fork");
                                _exit(-1);
                            case 0:
                                close(arrayPipes[i][0]);
                                dup2(arrayPipes[i-1][0],0);
                                close(arrayPipes[i-1][0]);     
                                dup2(arrayPipes[i][1],1);
                                close(arrayPipes[i][1]);
                                execlp(folder,execName,NULL);
                                _exit(-1);
                            default:
                                close(arrayPipes[i][1]);
                                close(arrayPipes[i-1][0]);
                                
                            }

                        }

                       
                        pids[i] = pid; //store pid of the processes launched to exec
                        
                    }*/

                        }
            
                       

                    //wait for all the execs
                    /*
                    for (int i = 0; i < n_filtersToExec; ++i) {
                        int status;
                        while (-1 == waitpid(pids[i], &status, 0)); 
                    }


                    /*
                        NEED to find a way for updating status in the father process
                        THIS UPDATE DOESNT WORK BECAUSE THE MEMORY IS NOT SHARED BETWeNN PROCESSES
                    */
                    
                    char taskNumb[100];
                    sprintf(taskNumb,"%d",get_task_number(t));
                    close(updatePipe[0]);
                    write(updatePipe[1],&taskNumb,strlen(taskNumb));
                    kill(pid_father,SIGUSR1);
   
                }
                    close(inFp);
                    close(outFp);
                
        

                /* NEDDS TO CLEAR THE FILTERS TO EXEC QUEQUE */
                
            } 
            
        }
    }


    return 0;
}