#ifndef __KERNEL__
#define __KERNEL__
#endif

#include <linux/module.h>
#include <linux/printk.h>
#include <linux/sched.h>

int init_module(void) 
{
  printk("<1>Hello, World!\nCurrent Process is \"%s\"; PID %i\n", current->comm, current->pid);
  return 0;
}

void cleanup_module(void)
{
  printk("<1>Goodbye, Cruel World.\n");
}

MODULE_LICENSE("GPL");


