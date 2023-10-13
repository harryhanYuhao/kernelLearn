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
static unsigned long procfs_buffer_size = 0;

/* this session is called when proc is read */
static ssize_t procfile_read(struct file *file_pointer, char __user *buffer,
                             size_t buffer_length, loff_t *offset)
{
        char s[15] = "Hello, World!\n";
        int len = sizeof(s);
        ssize_t ret = len;

        if (*offset >= len || copy_to_user(buffer, s, len)) {
                pr_info("Copy to User failed!\n");
                ret = 0;
        } else {
                pr_info("procfile read %s\n",
                        file_pointer->f_path.dentry->d_name.name);
                *offset += len;
        }
        return ret;
}

/* This function is called when /proc file is written */
static ssize_t procfile_write(struct file *file, const char __user *buff,
                              size_t len, loff_t *off)
{
        procfs_buffer_size = len;
        if (procfs_buffer_size > PROCFS_MAX_SIZE)
                procfs_buffer_size = PROCFS_MAX_SIZE;

        if (copy_from_user(procfs_buffer, buff, procfs_buffer_size))
                return -EFAULT;

        procfs_buffer[procfs_buffer_size & (PROCFS_MAX_SIZE - 1)] ='\0';
        *off += procfs_buffer_size;
        pr_info("procfile write %s \n", procfs_buffer);

        return procfs_buffer_size;
}

#ifdef HAVE_PROC_OPS
static const struct proc_ops proc_file_fops = {
        .proc_read = procfile_read,
        .proc_write = procfile_write,
};
#else 
static const struct file_operations proc_file_fops = {
        .read = procfile_read,
        .write = procfile_write,
};
#endif

static int __init procfs2_init(void)
{
        our_proc_file = proc_create(PROCFS_NAME, 0644, NULL, &proc_file_fops);
        if (our_proc_file == NULL){
                pr_alert("Error: Could not generate /proc/%s \n", PROCFS_NAME);
                return -ENOMEM;
        }

        pr_info("/proc/%s created!", PROCFS_NAME);
        return 0;
}

static void __exit procfs2_exit(void) {
        proc_remove(our_proc_file);
        pr_info("/proc/%s removed!", PROCFS_NAME);
}

module_init(procfs2_init);
module_exit(procfs2_exit);

MODULE_LICENSE("GPL");
