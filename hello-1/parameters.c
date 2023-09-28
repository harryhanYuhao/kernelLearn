#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>

static int hello3_data __initdata = 3;

// Customized init function
static int __init hello_2_init(void)
{
  pr_info("Second Init Method\n");
  return 0;
}

static void __exit hello_2_exit(void)
{
  pr_info("Second Exit\n");
}

module_init(hello_2_init);
module_exit(hello_2_exit);

MODULE_LICENSE("GPL");
