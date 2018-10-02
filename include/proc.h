#ifndef PROC_H
#define PROC_H

#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/version.h>

struct linux_dirent {
	unsigned long 	d_ino;
	unsigned long 	d_off;
	unsigned short 	d_namlen;
	unsigned long 	d_type;
	char 			d_name[];
};

struct readdir_data {
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,11,0)
	struct dir_context 	ctx;
#endif
	char 				*dirent;
	size_t 				used;
	int 				full;
};

struct proc_list {
	char 				*name;
	unsigned int 		length;
	struct list_head 	list;
};

void analyze_processes(void);

#endif /* PROC_H */
