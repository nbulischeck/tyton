#include <linux/workqueue.h>

#include "core.h"
#include "proc.h"
#include "logger.h"
#include "module_list.h"
#include "syscall_hooks.h"
#include "network_hooks.h"
#include "netfilter_hooks.h"
#include "interrupt_hooks.h"

static int timeout = 5;
unsigned long *idt = NULL; /* IDT Table */
unsigned long *sct = NULL; /* Syscall Table */
int (*ckt)(unsigned long addr) = NULL; /* Core Kernel Text */

static void work_func(struct work_struct *dummy);
static DECLARE_DELAYED_WORK(work, work_func);

static void execute_analysis(void){
	analyze_modules();
	analyze_syscalls();
	analyze_networks();
	analyze_netfilter();
	analyze_processes();
	analyze_interrupts();
}

static void work_func(struct work_struct *dummy){
	execute_analysis();
	schedule_delayed_work(&work,
		round_jiffies_relative(timeout*60*HZ));
}

void init_del_workqueue(void){
	schedule_delayed_work(&work, 0);
}

void exit_del_workqueue(void){
	cancel_delayed_work_sync(&work);
}

static void init_kernel_syms(void){
	idt = (void *)kallsyms_lookup_name("idt_table");
	sct = (void *)kallsyms_lookup_name("sys_call_table");
	ckt = (void *)kallsyms_lookup_name("core_kernel_text");
}

static int __init init_mod(void){
	INFO("Inserting Module\n");
	init_kernel_syms();
	init_del_workqueue();
	return 0;
}

static void __exit exit_mod(void){
	INFO("Exiting Module\n");
	exit_del_workqueue();
}

MODULE_AUTHOR("Nick Bulischeck <nbulisc@clemson.edu>");
MODULE_DESCRIPTION("Linux Kernel-Mode Rootkit Hunter for 4.4.0-31+.");
MODULE_LICENSE("GPL");

module_param(timeout, int, 0);
module_init(init_mod);
module_exit(exit_mod);
