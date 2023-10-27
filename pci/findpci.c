#include <linux/module.h>
#include <linux/init.h>
#include <linux/pci.h>
#include <linux/gpio.h>

/* can be checked with lspci -nn */
#define MY_VENDER_ID 0x8086
#define MY_DEVICE_ID 0x9d18

/* Meta Information */
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Example PCI Driver");

// static struct pci_device_id pcittl32io_ids[] = {
// 	{ PCI_DEVICE(PCITTL32IO_VENDOR_ID, PCITTL32IO_DEVICE_ID) },
// 	{ }
// };
//
// MODULE_DEVICE_TABLE(pci, pcittl32io_ids);

static int __init my_init(void) {
        u16 vid;
	pr_info("findpci init\n");
        struct pci_dev *dev = NULL;
        dev = pci_get_device(MY_VENDER_ID, MY_DEVICE_ID, dev);
        if (0 == pci_read_config_word(dev, 0x0, &vid))
                pr_info("vid is %u", vid);

        return 0;
}

/**
 * @brief This function is called, when the module is removed from the kernel
 */
static void __exit my_exit(void) {
	pr_info("findpci exit \n");
}

module_init(my_init);
module_exit(my_exit);
