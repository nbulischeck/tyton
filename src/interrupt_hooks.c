#include "core.h"
#include "util.h"
#include "logger.h"
#include "module_list.h"

extern unsigned long *idt; /* IDT Table */
extern int (*ckt)(unsigned long addr); /* Core Kernel Text */

void analyze_interrupts(void){
	int i;
	const char *mod_name;
	unsigned long addr;
	struct module *mod;

	GENERIC("Analyzing Interrupt Hooks\n");

	if (!idt || !ckt)
		return;

	for (i = 0; i < 256; i++){
		addr = idt[i];
		if (!ckt(addr)){
			mutex_lock(&module_mutex);
			mod = get_module_from_addr(addr);
			if (mod){
				SUCCESS("Module [%s] hooked syscall [%d].\n", mod->name, i);
			} else {
				mod_name = find_hidden_module(addr);
				SUCCESS("Hidden module [%s] hooked syscall [%d].\n", mod_name, i);
			}
			mutex_unlock(&module_mutex);
		}
	}
}
