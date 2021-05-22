#ifndef ___task_h___  
#define ___task_h___

#include "model/structs.h"


typedef struct task * Task;

Task create_task(int nTask,Request req);

Request getRequest(Task t);

int getTask_numb(Task t);

void show_task(Task t);

void show_taskList(List l);




#endif 