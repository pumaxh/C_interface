#ifndef ATOM_H
#define ATOM_H

extern int Atom_length(const char *str);
extern const char *Atom_string(const char *str);
extern const char * Atom_int(long n);
extern void Atom_init(int hint);
extern void Atom_free(const char *str);
extern void Atom_reset(void);

#endif // ATOM_H
