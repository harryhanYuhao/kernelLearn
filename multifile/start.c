#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/printk.h>

int init_module(void)
{
  pr_info("Multifile Module\n");
  return 0;
}

MODULE_LICENSE("GPL");
