#ifndef ___task_h___  
#define ___task_h___

#include "model/structs.h"


typedef struct task * Task;

Task create_task(int nTask,Request req);

Request get_task_request(Task t);

void set_task_number(Task t, int nTask);
int get_task_number(Task t);

void set_task_request(Task t, Request req);
Request get_task_request(Task t);

void show_task(Task t);

void show_taskList(List l);




#endif 