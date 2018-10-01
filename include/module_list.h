#ifndef MODULE_LIST_H
#define MODULE_LIST_H

#include <linux/kallsyms.h>

void analyze_modules(void);
void find_hidden_modules(unsigned long addr);

#endif /* MODULE_LIST_H */
