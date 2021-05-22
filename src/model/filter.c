#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "model/filter.h"

struct filter{
    char * name;
    char * exec_name;
    int f_runing;
    int max;
};

void inc_runing(Filter f){
    f->f_runing++;
}

void dec_runing(Filter f){
    f->f_runing--;
}

void set_name(Filter f,char * name){
    if(f != NULL && name != NULL)
        f->name = strdup(name);
}

void set_exec_name(Filter f,char * execName){
    if(f != NULL && execName != NULL) 
        f->exec_name = strdup(execName);
}

void set_f_runing(Filter f ,int runing){
    if(f != NULL)
        f->f_runing = runing;
}

void set_max(Filter f,int max){
    if(f != NULL)
        f->max = max;
}

char * get_name(Filter f){
    return f != NULL ? strdup(f->name) : NULL;
}

char * get_exec_name(Filter f){
    return f != NULL ? strdup(f->exec_name) : NULL;
}

int get_f_runing(Filter f){
    return f != NULL ? f->f_runing : -1;
}

int get_max(Filter f){
    return f != NULL ? f->max : -1;
}

Filter initFilter(char * name, char *exec_name, int max) {
    Filter f = NULL;
    
    if(name != NULL && exec_name != NULL) {
        f = malloc(sizeof(struct filter));
        set_name(f, name);
        set_exec_name(f, exec_name);
        set_f_runing(f, 0);
        set_max(f, max);
    }

    return f;
}

Filter initFFromLine(char * line){
    Filter f = NULL;
    char *linecpy = NULL;
    char *tmp = NULL;
    char *name = NULL;
    char *exec_name = NULL;
    int max = 0;

    if(line != NULL) {
        linecpy = strdup(line);
        tmp = linecpy;

        name = strsep(&linecpy," ");
        exec_name = strsep(&linecpy," ");
        max = atoi(strsep(&linecpy,"\n"));
        f = initFilter(name, exec_name, max);

        if(tmp != NULL)
            free(tmp);
    }
    return f;
}

void update_runingFilters(List filters,Task t,int op){ // op -> 0 add runing | 1 dec running
    Request req = get_task_request(t);
    char * line = getCommand(req);
    strsep(&line,";"); //transform
    strsep(&line,";"); //input
    strsep(&line,";"); //output
    char * seped;
    while((seped = strsep(&line,";"))){
        for(int i = 0;i<get_sizel(filters);i++){
            Filter f = (Filter) getValue(filters,i);
            if(strcmp(f->name,seped)==0){
                if(op==0){
                    inc_runing(f);
                }else{
                    dec_runing(f);
                }
                
            }
        }

    }
}

int canUse_filter(List filters,char * f_name){
    for(int i = 0;i<get_sizel(filters);i++){
        Filter f = getValue(filters,i);
        if(strcmp(f->name,f_name) == 0 && f->f_runing == f->max){
            return 0;
        }
    }
    return 1;
}

void show_filter(Filter f){
    printf("filter %s: %d/%d (running/max)\n",get_name(f),get_f_runing(f),get_max(f));
}

void show_filterList(List f_list){
    for(int i = 0;i<get_sizel(f_list);i++){
        Filter f = getValue(f_list,i);
        show_filter(f);
    }
}


