#include "core.h"
#include "util.h"

struct module *get_module_from_addr(unsigned long addr){
	return  __module_address(addr);
}
