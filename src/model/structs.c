#include <stdlib.h>
#include "model/structs.h"
#include "model/filter.h"

/* ------private------ */
struct nodo{

    void * value;
    List next;

};

static void set_l_value(List l, void *value) {
    if(l != NULL && value != NULL)
        l->value = value;
}

static void set_l_next(List l, List next) {
    if(l != NULL)
        l->next = next;
}

/* ------public------ */
void *get_l_value(List l) {
    return l != NULL ? l->value : NULL;
}

List *get_l_next(List l) {
    return l != NULL ? &(l->next) : NULL;
}

List initList(void *value) {
    List l = NULL;

    if(value != NULL) {
        l = malloc(sizeof(struct nodo));
        set_l_value(l, value);
        set_l_next(l, NULL);
    }
    return l;
}

/**
 * @brief Add element in the end of the list
 * 
 * @param l 
 * @param new_value 
 */
List push(List *l,void * new_value){
    List new_nodo = NULL;
    List *tmp = l;

    new_nodo = initList(new_value);

    for(;*tmp != NULL;tmp = get_l_next(*tmp));
    *tmp = new_nodo;

    return *l;
}

List pop(List l){
    if(l!=NULL){
        List new_list = *get_l_next(l);
        free(l);
        return new_list;
    }
    return l;
}

int get_sizel(List l){
    int i;
    for(i=0;l != NULL;l= *get_l_next(l),i++);

    return i;
}

void * getValue(List l,int pos){
    for(int i=0;l != NULL && i<pos;l = *get_l_next(l),i++);

    return get_l_value(l);
}


