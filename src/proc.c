#include "core.h"
#include "proc.h"
#include "util.h"

LIST_HEAD(tyton_log);
DECLARE_RWSEM(tyton_sem);

static void *t_start(struct seq_file *m, loff_t *pos){
	down_read(&tyton_sem);
	return seq_list_start(&tyton_log, *pos);
}

static void *t_next(struct seq_file *m, void *p, loff_t *pos){
	return seq_list_next(p, &tyton_log, pos);
}

static void t_stop(struct seq_file *m, void *p){
	up_read(&tyton_sem);
}

static int t_show(struct seq_file *m, void *p){
	struct node *entry = list_entry(p, struct node, list);
	seq_printf(m, "%s\n", entry->data);
	return 0;
}

static const struct seq_operations tyton_seq_ops = {
	.start		= t_start,
	.next		= t_next,
	.stop		= t_stop,
	.show		= t_show
};

static int tyton_info_open(struct inode *inode, struct file *file){
	return seq_open(file, &tyton_seq_ops);
}

static const struct file_operations proc_tyton_ops = {
	.open		= tyton_info_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= seq_release
};

void tyton_init_proc(void){
	proc_create("tyton", 0, NULL, &proc_tyton_ops);
}

void tyton_exit_proc(void){
	remove_proc_entry("tyton", NULL);
	clear_list(&tyton_log);
}
