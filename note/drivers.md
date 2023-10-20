# Linux Device Driver

## lspci 

Here is an output of lspci:

```shell
00:1d.0 PCI bridge: Intel Corporation Sunrise Point-LP PCI Express Root Port #9 (rev f1)
```

- `00:1d.0` is bus number, device number, and function number
- `PCI bridge` is device class
- `Intel Corporation` is Device vendor
- `Sunrise Point-LP PCI Express Root Port` is Device name
- `(rev f1)` is revision number

### Useful Options

- `lspci -k`: check which driver is in use for device
- `lspci -nn`: check vendor and device id

## Unbind driver to device

Before binding device to new driver, existing driver must be unbinded.

It is possible to unbind driver of the device in user space as well as kernel space:

In user space, first find the existing driver by `lspci -k`. Or, look for the driver symlink in the device directory `/sys/**/device/bus_id`. (Something like `/sys/devices/pci0000:00/00:1c.5`

In sysfs tree, each driver has a `bind` and `unbind` files associated with it, which can be found in the directory `/sys/bus/**/drivers/drivername`.

Then simply type the command:

```
 echo -n "00:1d.0" > /sys/bus/pci/drivers/pcieport/unbind 
```

To bind a driver in user space is similar:

```
 echo -n "00:1d.0" > /sys/bus/pci/drivers/pcieport/bind
```
