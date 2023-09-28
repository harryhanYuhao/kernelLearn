# Kernel Developping

## Kernel Modules

Kernel Modules is anything that can be loaded and unloaded from the kernel.

Useful commmands:

- `insmod <module-name>`: load the module 
- `rmmod <module-name>`: remove the module
- `lsmod`: show all modules 
- `modinfo <module-name>`: show module information
- `journalctl --since "x minute ago" | grep kernel`: 


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

