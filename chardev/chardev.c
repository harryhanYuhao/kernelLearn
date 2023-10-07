#include <linux/atomic.h>
#include <linux/cdev.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/types.h> #include <linux/uaccess.h>
#include <linux/version.h>
#include <asm/errno.h>

/* Prototypes -- Ususally placed in header files */
/* file in linux is not file 
 * Each device is represented in kernel by file structure, defined in fs.h 
 * FILE is defined by glibc, represented by inode structure in kernel
 */
static int device_open(struct inode *, struct file *); 
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char __user *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char __user *, size_t, loff_t *);

#define SUCCESS 0
#define DEVICE_NAME "chardev" /* DEVICE_NAME appears in /proc/devices */
#define BUF_LEN 80  /* max length of message from the device */

static int major; /* major number assigned to our device driver */

enum {
	CDEV_NOT_USED = 0,
	CDEV_EXCLUSIVE_OPEN = 1, 
};

static atomic_t already_open = ATOMIC_INIT(CDEV_NOT_USED);

static char msg[BUF_LEN + 1];

static struct class *cls;

static struct file_operations chardev_fops = {
	.read = device_read,
	.write = device_write,
	.open = device_open,
	.release = device_release,
};

static int __init chardev_init(void)
{
        /* register_chrdev defined in fs.h 
         * input: unsigned int major, const char * name, struct file_operations * fop
         * if major = 0, the return value will be dynamically allocated major number
         */
	major = register_chrdev(0, DEVICE_NAME, &chardev_fops);

	if (major < 0){
		pr_alert("Registering cahr device failed with %d\n", major);
		return major;
	}

	pr_info("I was assigned major number %d", major);

#if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 4, 0)
	cls = class_create(DEVICE_NAME);
#else 
	cls = class_create(THIS_MODULE, DEVICE_NAME);
#endif
        /* create device file */
	device_create(cls, NULL, MKDEV(major, 0), NULL, DEVICE_NAME);

	pr_info("Device created on /dev/%s\n", DEVICE_NAME);
	return SUCCESS;
}

static void __exit chardev_exit(void)
{
	device_destroy(cls, MKDEV(major, 0));
	class_destroy(cls);

	unregister_chrdev(major, DEVICE_NAME);
	pr_info("chardev removed!");
}

/* methods 
 * called when a process tries to open the device file, like "cat /dev/mycharfile" 
 * */
static int device_open(struct inode *inode, struct file *file)
{
	static int counter = 0;

	if (atomic_cmpxchg(&already_open, CDEV_NOT_USED, CDEV_EXCLUSIVE_OPEN))
		return -EBUSY;

	sprintf(msg, "I already told you %d times Hello, World! \n", counter++);
        /* increment the reference count of the current module */
	try_module_get(THIS_MODULE);

	return SUCCESS;
}

/* called when a process closes the device file */
static int device_release(struct inode *inode, struct file *file)
{
	/* we are ready for the next caller */
	atomic_set(&already_open, CDEV_NOT_USED);

        /* decrement the reference count of the current module */
	module_put(THIS_MODULE);
	return SUCCESS;
}

/* called when a process, which already opened the dev file, attempts to read from it */
static ssize_t device_read(struct file *filp,
			   char __user *buffer, /* buffer to fill with data */
			   size_t length,
			   loff_t *offset)
{
	/* number of bytes written to the buffer */
	int bytes_read = 0;
	const char *msg_ptr = msg;

	if (!*(msg_ptr + *offset)) {
		*offset = 0;
		return 0;
	}

	msg_ptr += *offset;

	while (length && *msg_ptr){
                /* the buffer is in user space 
                 * not kernel space
                 * put_user copies the date from kernel to user space
                 */
		put_user(*(msg_ptr++), buffer++);

		length--;
		bytes_read++;
	}

	*offset += bytes_read;

	return bytes_read;
}

/* called when a process writes to dev file, eg 
 * echo "hi" > /dev/hello
 * */
static ssize_t device_write(struct file *filp, const char __user *buff,
			    size_t len, loff_t *off)
{
	pr_alert("Sorry, this operation is not supported.\n");
	return -EINVAL;
}

module_init(chardev_init);
module_exit(chardev_exit);

MODULE_LICENSE("GPL");
