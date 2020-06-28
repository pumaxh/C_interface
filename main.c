#include <stdio.h>
#include "atom.h"

int main()
{
    Atom_init(7);
    const char *a1 = Atom_int(7);
    const char *a2 = Atom_string("test");

    printf("length of a1 is %d",Atom_length(a1));
    printf("length of a2 is %d",Atom_length(a2));

    Atom_free(a1);
    Atom_free(a2);
}
