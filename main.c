#include <stdio.h>
#include <assert.h>

#include "atom.h"

int atom_test()
{
    Atom_init(7);
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

int main()
{
    assert(atom_test() == 0);
    return 0;
}
