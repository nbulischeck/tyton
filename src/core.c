#include <linux/workqueue.h>

#include "core.h"
#include "proc.h"
#include "module_list.h"
#include "syscall_hooks.h"
#include "netfilter_hooks.h"

static void work_func(struct work_struct *dummy);
static DECLARE_DELAYED_WORK(work, work_func);

static void execute_analysis(void){
	analyze_modules();
	analyze_syscalls();
	analyze_netfilter();
	analyze_processes();
}

static void work_func(struct work_struct *dummy){
	execute_analysis();
	schedule_delayed_work(&work,
		round_jiffies_relative(TIMEOUT*60*HZ));
}

void init_del_workqueue(void){
	schedule_delayed_work(&work, 0);
}

void exit_del_workqueue(void){
	cancel_delayed_work_sync(&work);
}

static int __init init_mod(void){
	printk(KERN_INFO "[TYTON] Inserting module\n");
	init_del_workqueue();
	return 0;
}

static void __exit exit_mod(void){
	printk(KERN_INFO "[TYTON] Exiting module\n");
	exit_del_workqueue();
}

module_init(init_mod);
module_exit(exit_mod);
