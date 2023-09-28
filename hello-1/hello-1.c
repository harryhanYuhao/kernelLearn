#include <linux/module.h>
#include <linux/printk.h>

// this function is called when the module is loaded into the kernel
int init_module(void)
{
	pr_info("Hello, World! 1.\n");
	return 0;
}

// this function is called when the module is removed from the kernel.
void cleanup_module(void)
{
	pr_info("Goodbye! 1. \n");
}

MODULE_LICENSE("GPL");

