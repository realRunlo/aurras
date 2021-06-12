#ifndef ___structs_h___  
#define ___structs_h___


typedef struct nodo * List;

List initList();

List push(List *l,void * new_value);

List pop(List l);

int get_sizel(List l);

List *get_l_next(List l);

void * getValue(List l,int pos);

List removeValue(List l,int pos);


#endif //___structs_h___