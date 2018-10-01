#include "core.h"
#include "util.h"
#include "module_list.h"

#define BETWEEN_PTR(x, y, z) ( \
	((uintptr_t)x >= (uintptr_t)y) && \
	((uintptr_t)x < ((uintptr_t)y+(uintptr_t)z)) \
)

void find_hidden_modules(unsigned long addr){
	struct kset *mod_kset;
	struct kobject *cur, *tmp;
	struct module_kobject *kobj;

	mod_kset = (void *)kallsyms_lookup_name("module_kset");
	if (!mod_kset)
		return;

	list_for_each_entry_safe(cur, tmp, &mod_kset->list, entry){
		if (!kobject_name(tmp))
			break;

		kobj = container_of(tmp, struct module_kobject, kobj);
		if (!kobj || !kobj->mod)
			continue;

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,5,0)
		if (BETWEEN_PTR(addr, kobj->mod->core_layout.base, kobj->mod->core_layout.size)){
			printk(KERN_ALERT "[TYTON] Module [%s] controls a Netfilter hook.\n", kobj->mod->name);
		}
#else
		if (BETWEEN_PTR(addr, kobj->mod->module_core, kobj->mod->core_size)){
			printk(KERN_ALERT "[TYTON] Module [%s] controls a Netfilter hook.\n", kobj->mod->name);
		}
#endif
	}
}

void analyze_modules(void){
	struct kset *mod_kset;
	struct kobject *cur, *tmp;
	struct module_kobject *kobj;

	printk(KERN_INFO "[TYTON] Analyzing Module List\n");

	mod_kset = (void *)kallsyms_lookup_name("module_kset");
	if (!mod_kset)
		return;

	list_for_each_entry_safe(cur, tmp, &mod_kset->list, entry){
		if (!kobject_name(tmp))
			break;

		kobj = container_of(tmp, struct module_kobject, kobj);

		if (kobj && kobj->mod && kobj->mod->name){
			mutex_lock(&module_mutex);
			if(!find_module(kobj->mod->name))
				printk(KERN_ALERT "[TYTON] Module [%s] hidden.\n", kobj->mod->name);
			mutex_unlock(&module_mutex);
		}
	}
}
