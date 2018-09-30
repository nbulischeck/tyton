#include <linux/kernel.h>
#include <linux/module.h>

#include "syscall_hooks.h"
#include "netfilter_hooks.h"

static int __init init_mod(void){
	printk(KERN_INFO "[TYTON] Inserting module\n");
	analyze_syscalls();
	analyze_netfilter();
	return 0;
}

static void __exit exit_mod(void){
	printk(KERN_INFO "[TYTON] Exiting module\n");
}

MODULE_AUTHOR("Nick Bulischeck");
MODULE_DESCRIPTION("Rootkit Hunter");
MODULE_LICENSE("GPL");

module_init(init_mod);
module_exit(exit_mod);
