#include "core.h"

#define MESSAGE(level, tag, ...) printk(level tag __VA_ARGS__);
#define INFO(...) MESSAGE(KERN_INFO,    "tyton: INFO: ", __VA_ARGS__)
#define ALERT(...) MESSAGE(KERN_ALERT,   "tyton: ALERT: ", __VA_ARGS__)
#define WARNING(...) MESSAGE(KERN_WARNING, "tyton: WARNING: ", __VA_ARGS__)
#define ERROR(...) MESSAGE(KERN_ERR,     "tyton: ERROR: ", __VA_ARGS__)
