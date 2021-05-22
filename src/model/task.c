#include "model/request.h"
#include "model/task.h"
#include <stdio.h>
#include <stdlib.h>

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

void show_task(Task t){
    printf("task #%d: %s\n",get_task_number(t),getCommand(get_task_request(t)));
}

void show_taskList(List l){
    int size = get_sizel(l);
    for(int i = 0;i<size;i++){
        Task t = (Task) getValue(l,i);
        show_task(t);
    }
}



