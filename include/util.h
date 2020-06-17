#ifndef UTIL_H
#define UTIL_H

struct module *get_module_from_addr(unsigned long);

extern unsigned long lookup_name(const char *);

#endif /* UTIL_H */
