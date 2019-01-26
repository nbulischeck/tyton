#ifndef PROC_H
#define PROC_H

#include <linux/atomic.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>

void tyton_init_proc(void);
void tyton_exit_proc(void);

#endif /* PROC_H */
