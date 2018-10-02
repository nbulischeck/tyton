#include "core.h"

#define MESSAGE(level, tag, ...) printk(level tag __VA_ARGS__);
#define GENERIC(...) MESSAGE(KERN_INFO,    "[TYTON] ", __VA_ARGS__)
#define SUCCESS(...) MESSAGE(KERN_ALERT,   "[TYTON] ", __VA_ARGS__)
#define WARNING(...) MESSAGE(KERN_WARNING, "[TYTON] ", __VA_ARGS__)
#define FAILURE(...) MESSAGE(KERN_ERR,     "[TYTON] ", __VA_ARGS__)
