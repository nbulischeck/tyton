#include "core.h"
#include "util.h"

/* Module Detection */

struct module *get_module_from_addr(unsigned long addr){
	return  __module_address(addr);
}

/* String Functions */

char *build_string(const char *fmt, va_list args){
	char *line;

	line = kmalloc(512, GFP_KERNEL);
	if (!line)
		return NULL;

	vsnprintf(line, 512, fmt, args);

	return line;
}

/* List Management */

void clear_list(struct list_head *list){
	struct node *ptr, *tmp;

	list_for_each_entry_safe(ptr, tmp, list, list){
		list_del(&ptr->list);
		kfree(ptr);
	}
}

void append_data(struct list_head *list, char *fmt, ...){
	va_list args;
	struct node *entry;

	entry = kmalloc(sizeof *entry, GFP_KERNEL);
	if (!entry)
		goto error;

	va_start(args, fmt);
	entry->data = build_string(fmt, args);
	va_end(args);

	if (!entry->data)
		goto error;

	INIT_LIST_HEAD(&entry->list);
	list_add_tail(&entry->list, list);

	return;

error:
	if (entry)
		kfree(entry);

	return;
}
