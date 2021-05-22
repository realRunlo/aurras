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
    t->task_number = nTask;
    t->req = req;

    return t;
}

Request getRequest(Task t){
    return t->req;
}

int getTask_numb(Task t){
    return t->task_number;
}

void show_task(Task t){
    printf("task #%d: %s\n",t->task_number,getCommand(t->req));
}

void show_taskList(List l){
    
    for(int i = 0;i<get_sizel(l);i++){
        Task t = (Task) getValue(l,i);
        show_task(t);
    }
}



