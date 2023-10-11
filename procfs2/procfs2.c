#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h> /* providing user access */
#include <linux/version.h>

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 6, 0)
#define HAVE_PROC_OPS
#endif

#define PROCFS_MAX_SIZE 1024
#define PROCFS_NAME "buffer1k"

/* holding information on the buffer */
static struct proc_dir_entry *our_proc_file;

/* storing charactor of the module */
static char procfs_buffer[PROCFS_MAX_SIZE];

/*the size of the buffer */

