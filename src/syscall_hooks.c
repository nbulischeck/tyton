#include <linux/kallsyms.h> /* kallsyms_lookup_name */
#include <asm/asm-offsets.h> /* NR_syscalls */


static unsigned long *sct = NULL; /* Syscall Table */
static int (*ckt)(unsigned long addr); /* Core Kernel Text */

void analyze_syscalls(void){
	int i;

	sct = (void *)kallsyms_lookup_name("sys_call_table");
	ckt = (void *)kallsyms_lookup_name("core_kernel_text");

	for (i = 0; i < NR_syscalls; i++){
		if (!ckt(sct[i])){
			printk("Syscall number: %d hijacked!\n", i);
		}
	}

	return;
}
