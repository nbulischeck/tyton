#include <linux/kallsyms.h> /* kallsyms_lookup_name */
#include <asm/asm-offsets.h> /* NR_syscalls */

#include "core.h"
#include "util.h"
#include "module_list.h"

static unsigned long *sct = NULL; /* Syscall Table */
static int (*ckt)(unsigned long addr); /* Core Kernel Text */

void analyze_syscalls(void){
	int i;
	const char *mod_name;
	unsigned long addr;
	struct module *mod;

	printk(KERN_INFO "[TYTON] Analyzing Syscall Hooks\n");

	sct = (void *)kallsyms_lookup_name("sys_call_table");
	ckt = (void *)kallsyms_lookup_name("core_kernel_text");

	if (!sct || !ckt)
		return;

	for (i = 0; i < NR_syscalls; i++){
		addr = sct[i];
		if (!ckt(addr)){
			mutex_lock(&module_mutex);
			mod = get_module_from_addr(addr);
			if (mod){
				printk(KERN_ALERT "[TYTON] Module [%s] hooked syscall [%d].\n", mod->name, i);
			} else {
				mod_name = find_hidden_module(addr);
				printk(KERN_ALERT "[TYTON] Hidden module [%s] hooked syscall [%d].\n", mod_name, i);
			}
			mutex_unlock(&module_mutex);
		}
	}
}
