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

default: all

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	make -C $(PWD)/notify

install: install-notify install-module

install-notify: 
	make -C $(PWD)/notify install

install-module:
	sudo insmod tyton.ko

uninstall: uninstall-notify uninstall-module

uninstall-notify:
	make -C $(PWD)/notify uninstall

uninstall-module:
	sudo rmmod tyton.ko

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	make -C $(PWD)/notify clean
