#ifndef ___filter_h___  
#define ___filter_h___

#include "structs.h"
#include "../includes/request.h"
#include "../includes/task.h"

typedef struct filter * Filter;

void set_name(Filter f,char * name);

void set_exec_name(Filter f,char * execName);

void set_f_runing(Filter f ,int runing);

void set_max(Filter f,int max);

char * get_name(Filter f);

char * get_exec_name(Filter f);

int get_f_runing(Filter f);

int get_max(Filter f);

void update_runingFilters(List filters,Task t,int op);

int canUse_filter(List filters,char * f_name);

void show_filter(Filter f);

void show_filterList(List f);

Filter initF(char * line);

#endif //___filter_h___