#include <stdio.h>
#include <assert.h>

#include "atom.h"
#include "arena.h"
#include "list.h"

Arena_T *g_arena;

int atom_test()
{
    Atom_initial(7);
    const char *a1 = Atom_int(7);
    const char *a2 = Atom_string("test");
    const char *a3 = Atom_int(999999);

    assert(Atom_length(a1) == 1);
    assert(Atom_length(a2) == 4);
    assert(Atom_length(a3) == 6);

    Atom_free(a1);
    assert(Atom_length(a1) == -1);
    Atom_reset();
    assert(Atom_length(a2) == -1);
    assert(Atom_length(a3) == -1);
    printf("Atom test pass!");
    return 0;
}

int arena_test()
{
    Arena_T *g_arena = Arena_new();
    char *str1 = (char *)Arena_alloc(g_arena, 1024, __FILE__, __LINE__);
    snprintf(str1, 1023, "str1: !!!!!!!!!!!!" );
    Arena_free(g_arena);

    char *str2 = (char *)Arena_alloc(g_arena, 1024, __FILE__, __LINE__);
    snprintf(str2, 1023, "str2: !!!!!!!!!!!!" );

    Arena_dispose(&g_arena);
    assert(g_arena == NULL);
    printf("Arena test pass!");
    return 0;
}

void list_show_element(void **elem, void *cl)
{
    assert(elem != NULL);
    printf("element[%s] ", (char *)(*elem));
}

int list_test()
{
    char *rst;
    int isLast;
    List_T_p list1 = List_list("first");
    List_map(list1, list_show_element, NULL);
    list1 = List_push(list1, "second");
    List_T_p list2 = List_list("1");
    list1 = List_append(list1, list2);
    printf("after append\n");
    List_map(list1, list_show_element, NULL);
    list1 = List_revers(list1);
    printf("after reverse\n");
    List_map(list1, list_show_element, NULL);
    List_pop(list1, (void **)&rst);
    printf("pop elem[%s]\n", rst);
    return 0;
}

int main()
{
    assert(list_test() == 0);
    return 0;
}
