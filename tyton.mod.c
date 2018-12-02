#include <linux/build-salt.h>
#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0xef168ca7, "module_layout" },
	{ 0x2d3385d3, "system_wq" },
	{ 0xd2b09ce5, "__kmalloc" },
	{ 0x1d5fc50e, "param_ops_int" },
	{ 0x754d539c, "strlen" },
	{ 0xffeedf6a, "delayed_work_timer_fn" },
	{ 0xf6c1256d, "cancel_delayed_work_sync" },
	{ 0xa6093a32, "mutex_unlock" },
	{ 0xe2d5255a, "strcmp" },
	{ 0xece784c2, "rb_first" },
	{ 0x7c32d0f0, "printk" },
	{ 0x5a921311, "strncmp" },
	{ 0x41aed6e7, "mutex_lock" },
	{ 0xe007de41, "kallsyms_lookup_name" },
	{ 0x11652772, "init_net" },
	{ 0x93fca811, "__get_free_pages" },
	{ 0x164e7c53, "queue_delayed_work_on" },
	{ 0xdb7305a1, "__stack_chk_fail" },
	{ 0x2ea2c95c, "__x86_indirect_thunk_rax" },
	{ 0xbdfb6dbb, "__fentry__" },
	{ 0x4302d0eb, "free_pages" },
	{ 0xdee6556a, "find_module" },
	{ 0x1eb9516e, "round_jiffies_relative" },
	{ 0x37a0cba, "kfree" },
	{ 0x69acdf38, "memcpy" },
	{ 0xe719502a, "module_mutex" },
	{ 0xc7c9873b, "__module_address" },
	{ 0xfee975d8, "iterate_dir" },
	{ 0xca9360b5, "rb_next" },
	{ 0x34224879, "filp_open" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "3BA24CDDC3FC3F1ABBEFF59");
