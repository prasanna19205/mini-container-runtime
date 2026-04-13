#include <linux/module.h>
#include <linux/export-internal.h>
#include <linux/compiler.h>

MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};



static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0x8ffd462b, "send_sig" },
	{ 0xc13edbcb, "find_vpid" },
	{ 0x848a0d8d, "pid_task" },
	{ 0x67628f51, "msleep" },
	{ 0x37031a65, "__register_chrdev" },
	{ 0x7f79e79a, "kthread_create_on_node" },
	{ 0x630dad60, "wake_up_process" },
	{ 0x0571dc46, "kthread_stop" },
	{ 0xcb8b6ec6, "kfree" },
	{ 0x52b15b3b, "__unregister_chrdev" },
	{ 0xbd03ed67, "random_kmalloc_seed" },
	{ 0xfaabfe5e, "kmalloc_caches" },
	{ 0xc064623f, "__kmalloc_cache_noprof" },
	{ 0xbd03ed67, "__ref_stack_chk_guard" },
	{ 0x092a35a2, "_copy_from_user" },
	{ 0xd272d446, "__stack_chk_fail" },
	{ 0xd272d446, "__fentry__" },
	{ 0xe8213e80, "_printk" },
	{ 0xd272d446, "__x86_return_thunk" },
	{ 0x5e505530, "kthread_should_stop" },
	{ 0xbebe66ff, "module_layout" },
};

static const u32 ____version_ext_crcs[]
__used __section("__version_ext_crcs") = {
	0x8ffd462b,
	0xc13edbcb,
	0x848a0d8d,
	0x67628f51,
	0x37031a65,
	0x7f79e79a,
	0x630dad60,
	0x0571dc46,
	0xcb8b6ec6,
	0x52b15b3b,
	0xbd03ed67,
	0xfaabfe5e,
	0xc064623f,
	0xbd03ed67,
	0x092a35a2,
	0xd272d446,
	0xd272d446,
	0xe8213e80,
	0xd272d446,
	0x5e505530,
	0xbebe66ff,
};
static const char ____version_ext_names[]
__used __section("__version_ext_names") =
	"send_sig\0"
	"find_vpid\0"
	"pid_task\0"
	"msleep\0"
	"__register_chrdev\0"
	"kthread_create_on_node\0"
	"wake_up_process\0"
	"kthread_stop\0"
	"kfree\0"
	"__unregister_chrdev\0"
	"random_kmalloc_seed\0"
	"kmalloc_caches\0"
	"__kmalloc_cache_noprof\0"
	"__ref_stack_chk_guard\0"
	"_copy_from_user\0"
	"__stack_chk_fail\0"
	"__fentry__\0"
	"_printk\0"
	"__x86_return_thunk\0"
	"kthread_should_stop\0"
	"module_layout\0"
;

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "B99F5A826B9E46D00BCEEDC");
