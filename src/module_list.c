#include "core.h"
#include "module_list.h"

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
