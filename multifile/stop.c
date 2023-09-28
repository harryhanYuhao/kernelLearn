#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/printk.h>

void cleanup_module(void)
{
  pr_info("cleanup multifile module\n");
}

MODULE_LICENSE("GPL");
