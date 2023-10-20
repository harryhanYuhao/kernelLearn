obj-m += pci.o

PWD := $(CURDIR)

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	EXTRA_CFLAGS='-save-temps'

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
