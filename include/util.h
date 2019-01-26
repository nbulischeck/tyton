#ifndef UTIL_H
#define UTIL_H

#include <linux/slab.h>

struct node {
	char *data;
	struct list_head list;
};

/* Module Detection */

struct module *get_module_from_addr(unsigned long);

/* List Management */

void clear_list(struct list_head *list);
void append_data(struct list_head *list, char *fmt, ...);

#endif /* UTIL_H */
