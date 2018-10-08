#include <asm/asm-offsets.h> /* NR_syscalls */

#include "core.h"
#include "util.h"
#include "logger.h"
#include "module_list.h"

extern unsigned long *sct; /* Syscall Table */
extern int (*ckt)(unsigned long addr); /* Core Kernel Text */

void analyze_syscalls(void){
	int i;
	const char *mod_name;
	unsigned long addr;
	struct module *mod;

	INFO("Analyzing Syscall Hooks\n");

	if (!sct || !ckt)
		return;

	for (i = 0; i < NR_syscalls; i++){
		addr = sct[i];
		if (!ckt(addr)){
			mutex_lock(&module_mutex);
			mod = get_module_from_addr(addr);
			if (mod){
				ALERT("Module [%s] hooked syscall [%d].\n", mod->name, i);
			} else {
				mod_name = find_hidden_module(addr);
				if (mod_name)
					ALERT("Hidden module [%s] hooked syscall [%d].\n", mod_name, i);
			}
			mutex_unlock(&module_mutex);
		}
	}
}
