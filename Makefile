TYTON := tyton
SRCDIR := src

obj-m := $(TYTON).o
$(TYTON)-y += $(SRCDIR)/core.o
$(TYTON)-y += $(SRCDIR)/syscall_hooks.o
$(TYTON)-y += $(SRCDIR)/netfilter_hooks.o

default: all

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
