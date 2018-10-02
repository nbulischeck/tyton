#include "core.h"

#define RESET  "\x1B[0m"
#define BOLD   "\x1B[1m"
#define WHITE  "\x1B[89m"
#define RED    "\x1B[91m"
#define GREEN  "\x1B[92m"
#define YELLOW "\x1B[93m"

#define MESSAGE(level, color, tag, ...) printk(level color tag RESET __VA_ARGS__);
#define GENERIC(...) MESSAGE(KERN_INFO, BOLD WHITE , "[TYTON] ", __VA_ARGS__)
#define SUCCESS(...) MESSAGE(KERN_ALERT, BOLD GREEN , "[TYTON] ", __VA_ARGS__)
#define WARNING(...) MESSAGE(KERN_WARNING, BOLD YELLOW, "[TYTON] ", __VA_ARGS__)
#define FAILURE(...) MESSAGE(KERN_ERR, BOLD RED   , "[TYTON] ", __VA_ARGS__)
