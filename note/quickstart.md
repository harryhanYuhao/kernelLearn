# Kernel Developping

## Kernel Modules

Kernel Modules is anything that can be loaded and unloaded from the kernel.

Useful commmands:

- `insmod <module-name>`: load the module 
- `rmmod <module-name>`: remove the module
- `lsmod`: show all modules 
- `modinfo <module-name>`: show module information
- `journalctl --since "x minute ago" | grep kernel`: 
- `strace <path-to-file>`: trace syscalls


## Build Modules

To build modules from c file named `hello-1.c`, consider the following makefile:

```makefile
obj-m += hello-1.o 
obj-m += hello-2.o

PWD := $(CURDIR)

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	EXTRA_CFLAGS='-save-temps'

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

EXTRA_CFLAGS='-save-temps'  # save the temporary files
```

To build the module, you may need to switch to root user with `sudo -Es`. `-E` flag is to preserve the bash variables.

To insert the module, `insmod <module-name>.ko`

### Compilation and Linking: some insights

The kernel module shall only invoke the symbols that are contained in the kernel (can be found in `/proc/kallsyms`. C library is not part of kernel. 

In the module file we invoked the function `pr_info()` without linking. 
This is because module are merely object files whose symbol was resolved when it is inserted into the kernel by `insmod`.

### Namespace pollution

To avoid namespace pollution:

- use static variable
- prefix each variable with lower case letter (kernel convention)
- register the variable in the symbol table.

### Memory

Each module shares the kernel's codespace memory. So module's segfault is also the kernel's. Most monolithic kernel behaves like this. There are also microkenrels like GNU Hurd and Zircon whose module may be in independent codespace.

## Device drivers 

### Device files

On unix, each hardware is represented by a file located in `/dev`. So a device driver may be linked to `/dev/sound`, and a user space program can access `/dev/sound` without knowing the driver.

The output of `lsblk` is:

```
NAME        MAJ:MIN RM   SIZE RO TYPE MOUNTPOINTS
nvme0n1     259:0    0 238.5G  0 disk 
├─nvme0n1p1 259:1    0     4G  0 part /boot
├─nvme0n1p2 259:2    0     8G  0 part [SWAP]
└─nvme0n1p3 259:3    0 226.5G  0 part /
```

The output of `ls \dev\` is something like:

```
crw-------  10,123 root 30 Sep 18:03 acpi_thermal_rel
crw-r--r--  10,235 root 30 Sep 18:03 autofs
drwxr-xr-x       - root 30 Sep 18:03 block
crw-------  10,234 root 30 Sep 18:03 btrfs-control
drwxr-xr-x       - root 30 Sep 18:03 bus
drwxr-xr-x       - root 30 Sep 18:03 char
crw-------     5,1 root 30 Sep 18:03 console
lrwxrwxrwx    141T root 30 Sep 18:03 core -> /proc/kcore
drwxr-xr-x       - root 30 Sep 18:03 cpu
crw-------  10,124 root 30 Sep 18:03 cpu_dma_latency
crw-------  10,203 root 30 Sep 18:03 cuse
drwxr-xr-x       - root 30 Sep 18:03 disk
drwxr-xr-x       - root 30 Sep 18:03 dma_heap
drwxr-xr-x       - root 30 Sep 18:03 dri
crw-------   238,0 root 30 Sep 18:03 drm_dp_aux0
crw-rw----    29,0 root 30 Sep 18:03 fb0
lrwxrwxrwx       - root 30 Sep 18:03 fd -> /proc/self/fd
crw-rw-rw-     1,7 root 30 Sep 18:03 full
crw-rw-rw-     1,3 root 30 Sep 18:03 null
crw-------   237,0 root 30 Sep 18:03 nvme0
brw-rw----     3,0 root 30 Sep 18:03 nvme0n1
brw-rw----     3,1 root 30 Sep 18:03 nvme0n1p1
brw-rw----     3,2 root 30 Sep 18:03 nvme0n1p2
brw-rw----     3,3 root 30 Sep 18:03 nvme0n1p3
crw-------  10,144 root 30 Sep 18:03 nvram
crw-r-----     1,4 root 30 Sep 18:03 port
crw-------   108,0 root 30 Sep 18:03 ppp
crw-------    10,1 root 30 Sep 18:03 psaux
crw-rw-rw-     5,2 root 30 Sep 19:04 ptmx
drwxr-xr-x       - root 30 Sep 18:03 pts
crw-rw-rw-     1,8 root 30 Sep 18:03 random
crw-rw-r--@ 10,242 root 30 Sep 18:03 rfkill
lrwxrwxrwx   248,0 root 30 Sep 18:03 rtc -> rtc0
crw-------   248,0 root 30 Sep 18:03 rtc0
drwxrwxrwt       - root 30 Sep 18:03 shm
crw-------  10,231 root 30 Sep 18:03 snapshot
drwxr-xr-x       - root 30 Sep 18:03 snd
lrwxrwxrwx   136,0 root 30 Sep 18:03 stderr -> /proc/self/fd/2
lrwxrwxrwx   136,0 root 30 Sep 18:03 stdin -> /proc/self/fd/0
lrwxrwxrwx       0 root 30 Sep 18:03 stdout -> /proc/self/fd/1
crw-rw----  10,224 tss  30 Sep 18:03 tpm0
crw-rw----   253,0 root 30 Sep 18:03 tpmrm0
crw-rw-rw-     5,0 root 30 Sep 18:32 tty
crw--w----     4,0 root 30 Sep 18:03 tty0
crw-------     4,1 hhyh 30 Sep 18:56 tty1
crw--w----     4,2 root 30 Sep 18:03 tty2
crw--w----     4,3 root 30 Sep 18:03 tty3
...
rw-rw----    4,87 root 30 Sep 18:03 ttyS23
crw-rw----    4,88 root 30 Sep 18:03 ttyS24
crw-rw----    4,89 root 30 Sep 18:03 ttyS25
crw-rw----    4,90 root 30 Sep 18:03 ttyS26
crw-rw----    4,91 root 30 Sep 18:03 ttyS27
crw-rw----    4,92 root 30 Sep 18:03 ttyS28
```

The number separated by comma is the device major number and minor number. Each driver is assigned to a unique major number, which is shown here before the comma, denoting the associated driver. The minor number is merely for the driver to distinguish the various hardware. 

When a device file is accessed, the kernel use the major number to determine which driver to use. The kernel cares not about the minor number, which is only used by the driver to differentiate the device.

Device file can be created by command like `mknod /dev/coffee c 12 2`. Device file can be placed in any directory.

#### `file_operation` structure

The `file_operaton` structure is defined in `include/linux/fs.h`. It holds pointer to function that perform various operation on the device. 

It is defined thus: 

```c 
struct file_operations {
    struct module *owner;
    loff_t (*llseek) (struct file *, loff_t, int);
    ssize_t (*read) (struct file *, char __user *, size_t, loff_t *);
    ssize_t (*write) (struct file *, const char __user *, size_t, loff_t *);
    ssize_t (*read_iter) (struct kiocb *, struct iov_iter *);
    ssize_t (*write_iter) (struct kiocb *, struct iov_iter *);
    int (*iopoll)(struct kiocb *kiocb, struct io_comp_batch *,
            unsigned int flags);
    /* many omitted 
     * ...
     */
    int (*fadvise)(struct file *, loff_t, loff_t, int);
    int (*uring_cmd)(struct io_uring_cmd *ioucmd, unsigned int issue_flags);
    int (*uring_cmd_iopoll)(struct io_uring_cmd *, struct io_comp_batch *,
            unsigned int poll_flags);
} __randomize_layout;

```

#### Registering a device

Char device is accessed through device file. 
Adding a driver to a system means registering it to the kernel, which means assigning a major number during the module's initialisation. 

### Unregistering a device 

Each device has a counter that tracks how many process are using it. `rmmod` only remove the module if the conter is 0.
This counter shall not be used directly. Instead, use the function:

```c
try_module_get(THIS_MODULE); // increase reference count by 1
module_put(THIS_MODULE); // decrease by 1 
module_refcount(THIS_MODULE); // return refcount
```

You shall call `module_put` at exit function of the module.

If the reference count was wrongly manipulated, the module can not be removed and the only resort is to reboot the computer.

### Concurrency

Kernel code shall be written with attention of concurrency. 


## `\proc` file system

`\proc` file system does not reside on disk. They live on memory. 

Create a `proc` file is similar to creating a device file. A structure holding all information and function pointers needs to be created.

## Memory Segment

Linux memory, at least on Intel Architecture, is segmented. 
A pointer by itself thus does not contain enough information to locate a memory address.

There is one segment for the kernel (and its modules) , and one for each processes. Each process can only access its own segment. 

A kernel module many times only nees to access the kernel's memory segment. The content of process's memory segment may be accessed by passing it as a buffer to the kernel's segment, or vice versa, using macros `put_user; get_user; copy_to_user

