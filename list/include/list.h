#ifndef LIST_H
#define LIST_H

typedef struct List_T * List_T_p;

struct List_T {
    List_T_p rest;
    void *first;
};

#define T List_T
#define PT List_T_p

extern PT   List_append(PT list, PT tail);
extern PT   List_copy(PT list);
extern PT   List_list(void *x);
extern PT   List_pop(PT list, void **x);
extern PT   List_push(PT list, void *x);
extern PT   List_revers(PT list);
extern int  List_length(PT list);
extern void List_free(PT *list);
extern void List_map(PT list,
                     void apply(void **x, void*cl),
                     void* cl);
extern void **List_toArray(PT list, void *end);
#undef T
#undef PT

#endif // LIST_H
