#include "model/request.h"
#include "model/task.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct task{

    int task_number;
    Request req;

};

Task create_task(int nTask,Request req){
    Task t = malloc(sizeof(struct task));
    set_task_number(t, nTask);
    set_task_request(t, req);

    return t;
}

void set_task_number(Task t, int nTask) {
    if(t != NULL) 
        t->task_number = nTask;
}

void set_task_request(Task t, Request req) {
    if(t != NULL)
        t->req = req;
}

Request get_task_request(Task t){
    return t->req;
}

int get_task_number(Task t){
    return t->task_number;
}
char * getInFile(Task t){
    char * str = getCommand(get_task_request(t));
    char * seped;
    strsep(&str,";");
    seped = strsep(&str,";"); //inputfile

    return strdup(seped);
}

int get_tasks_pos(List tasks,int tNumb){
   
    List tmp = tasks;
    for(int i = 0 ;tmp!=NULL; tmp = *get_l_next(tmp),i++){

        Task t = getValue(tasks,i);
        if(get_task_number(t) == tNumb){
            return i;
        }

    }
    return -1;
}

char * getOutFile(Task t){
    char * str = getCommand(get_task_request(t));
    char * seped;
    strsep(&str,";");
    strsep(&str,";");
    seped = strsep(&str,";"); //outputfile

    return strdup(seped);

}
void show_task(Task t){
    char * comand = getCommand(get_task_request(t));
    printf("task #%d: ",get_task_number(t));
    char * seped;
    while((seped=strsep(&comand,";"))){
        printf("%s ",seped);
    }
    printf("\n");
}

void show_taskList(List l){

    int size = get_sizel(l);
    List tmp = l;
    for(int i = 0;i<size && tmp;i++){
        Task t = (Task) getValue(tmp,i);
        show_task(t);
    }
   
}



