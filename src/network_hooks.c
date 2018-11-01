#include <net/net_namespace.h> /* init_net */

#include "core.h"
#include "util.h"
#include "logger.h"
#include "module_list.h"
#include "network_hooks.h"

#define NUM_NET_ENTRIES 6

extern int (*ckt)(unsigned long addr); /* Core Kernel Text */

struct net_entry net[NUM_NET_ENTRIES] = {
	{"tcp",			NULL},
	{"tcp6",		NULL},
	{"udp",			NULL},
	{"udp6",		NULL},
	{"udplite",		NULL},
	{"udplite6",	NULL}
};

struct proc_dir_entry *find_subdir(struct rb_root *tree, const char *str){
	struct rb_node *node = rb_first(tree);
	struct proc_dir_entry *e = NULL;

	while (node){
		e = rb_entry(node, struct proc_dir_entry, subdir_node);
		if (strcmp(e->name, str) == 0)
			return e;
		node = rb_next(node);
	}

	return NULL;
}

void analyze_networks(void){
	int i, j;
	unsigned long op_addr[4];
	const struct module *mod;
	const struct seq_operations *seq_ops;
	const struct file_operations *seq_fops;
	const char *mod_name, *op_string[4] = {
		"llseek", "read", "release", "show"
	};

	for (i = 0; i < NUM_NET_ENTRIES; i++){
		net[i].entry = find_subdir(&init_net.proc_net->subdir, net[i].name);
		if (!net[i].entry)
			continue;

		seq_ops = net[i].entry->seq_ops;
		seq_fops = net[i].entry->proc_fops;
		
		op_addr[0] = *(unsigned long *)seq_fops->llseek;
		op_addr[1] = *(unsigned long *)seq_fops->read;
		op_addr[2] = *(unsigned long *)seq_fops->release;
		op_addr[3] = *(unsigned long *)seq_ops->show;

		for (j = 0; j < 4; j++){
			if (!ckt(op_addr[j])){
				mutex_lock(&module_mutex);
				mod = get_module_from_addr(op_addr[j]);
				if (mod){
					ALERT("Module [%s] hooked %s function %s.\n",
						mod->name, net[i].entry->name, op_string[j]);
				} else {
					mod_name = find_hidden_module(op_addr[j]);
					if (mod_name)
						ALERT("Module [%s] hooked %s function %s.\n",
							mod_name, net[i].entry->name, op_string[j]);
				}
				mutex_unlock(&module_mutex);
			}
		}
	}
}
