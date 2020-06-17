#include <linux/kallsyms.h>

#include "core.h"
#include "util.h"

struct module *get_module_from_addr(unsigned long addr){
	return  __module_address(addr);
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5,7,0)

#include <linux/kprobes.h>

static struct kprobe kp;

unsigned long lookup_name(const char *name){
	kp.symbol_name = name;

	if (register_kprobe(&kp) < 0)
		return 0;

	unregister_kprobe(&kp);

	return (unsigned long)kp.addr;
}

#else

unsigned long lookup_name(const char *name){
	return kallsyms_lookup_name(name);
}

#endif
