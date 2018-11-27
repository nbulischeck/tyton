TYTON := tyton
SRCDIR := src

obj-m := $(TYTON).o
$(TYTON)-y += $(SRCDIR)/core.o
$(TYTON)-y += $(SRCDIR)/util.o
$(TYTON)-y += $(SRCDIR)/proc.o
$(TYTON)-y += $(SRCDIR)/module_list.o
$(TYTON)-y += $(SRCDIR)/syscall_hooks.o
$(TYTON)-y += $(SRCDIR)/network_hooks.o
$(TYTON)-y += $(SRCDIR)/netfilter_hooks.o
$(TYTON)-y += $(SRCDIR)/interrupt_hooks.o

HEADERS := $(PWD)/include
ccflags-y += -I$(HEADERS)

default: module notify

module:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

notify:
	make -C $(PWD)/notify

install:
	cp tyton.ko $(DESTDIR)/

clean: clean-module clean-notify

clean-module:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

clean-notify:
	make -C $(PWD)/notify clean