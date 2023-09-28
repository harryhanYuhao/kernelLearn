#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/moduleparam.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Harry Han");
MODULE_DESCRIPTION("A sample driver");

static int myint = 3;
static int myarray[2] = {1,2};
static char *mystring = "Hello, World!";
static int myarray_count = 0;

module_param(myint, int, 0); /* name, type, permission */

/* 
 * pass argument like:
 * insmod hello-2.ko myint=5
 */

// static int hello3_data __initdata = 3;

/* passing string */

module_param(mystring, charp, 0000);
MODULE_PARM_DESC(mystring, "hello string");

/* passing array 
 * array name (pointer name)
 * type of array element 
 * pointer to a array counter 
 * permission
 */
module_param_array(myarray, int, &myarray_count, 0000);

// Customized init function
static int __init hello_2_init(void)
{
  int i;

  pr_info("Second Init Method\n");
  pr_info("%s: %d\n", "myint", myint);
  pr_info("mystring is %s\n", mystring);

  for (i=0; i<ARRAY_SIZE(myarray); ++i){
    pr_info("The %d element of array is %d", myarray_count, myarray[i]);
  }
  pr_info("%d", myarray_count);
  return 0;
}

static void __exit hello_2_exit(void)
{
  pr_info("Second Exit\n");
}

module_init(hello_2_init);
module_exit(hello_2_exit);

