#include <stdlib.h>
#include "model/structs.h"
#include "model/filter.h"

/* ------private------ */
struct nodo{

    void * value;
    List next;

};

/* ------public------ */

List initList(){
    List new_list =  malloc(sizeof(struct nodo));
    new_list->next = NULL;
    return new_list;
}

/**
 * @brief Add element in the end of the list
 * 
 * @param l 
 * @param new_value 
 */
List push(List l,void * new_value){
    List head = l;
    if(l==NULL){
        l = malloc(sizeof(struct nodo));
        l->value = new_value;
        l->next = NULL;
    }else{
        List new_nodo = malloc(sizeof(struct nodo));
        new_nodo->value = new_value;
        new_nodo->next = NULL;
        for(;l->next;l=l->next);
        l->next = new_nodo;
    }

    return head;
}

List pop(List l){
    if(l!=NULL){
        List new_list = l->next;
        free(l);
        return new_list;
    }
    return l;
}

int get_sizel(List l){
    int i;
    for(i=0;l->next;l=l->next,i++);

    return i;
}

void * getValue(List l,int pos){
    for(int i=0;l->next && i<=pos;l = l->next,i++);

    return l->value;
}


