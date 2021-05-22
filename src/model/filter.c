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
    
    f->name = strdup(name);
}

void set_exec_name(Filter f,char * execName){
    f->exec_name = strdup(execName);
}

void set_f_runing(Filter f ,int runing){
    f->f_runing = runing;
}

void set_max(Filter f,int max){
    f->max = max;
}

char * get_name(Filter f){
    return strdup(f->name);
}

char * get_exec_name(Filter f){
    return strdup(f->exec_name);
}

int get_f_runing(Filter f){
    return f->f_runing;
}

int get_max(Filter f){
    return f->max;
}

Filter initF(char * line){

    char * linecpy = strdup(line);
    Filter f = malloc(sizeof(struct filter));
    f->name = strdup(strsep(&linecpy," "));
    f->exec_name = strdup(strsep(&linecpy," "));
    f->f_runing = 0;
    f->max = atoi(strsep(&linecpy,"\n"));

    return f;
}
void update_runingFilters(List filters,Task t,int op){ // op -> 0 add runing | 1 dec running
    Request req = getRequest(t);
    char * line = getCommand(req);
    strsep(&line,";"); //transform
    strsep(&line,";"); //input
    strsep(&line,";"); //output
    char * seped;
    while((seped = strsep(&line,";"))){
        for(int i = 0;i<get_sizel(filters);i++){
            Filter f = getValue(filters,i);
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
    printf("filter %s: %d/%d (running/max)\n\0",get_name(f),get_f_runing(f),get_max(f));
}

void show_filterList(List f_list){
    for(int i = 0;i<get_sizel(f_list);i++){
        Filter f = getValue(f_list,i);
        show_filter(f);
    }
}


