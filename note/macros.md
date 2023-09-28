# Kernel Macros

## `__init` 

Usage:

```c
static void __init init_hello(void);
```

This macro does not apply to loadable modules. For others, it causes the init function to be discarded and memory freed after the function is called.

## `__exit`

Usage: similar to `__init`. 

This macro does not apply to loadable modules. For others, it causesthe ommision of the function module when it is built into the Kernel

## `MODULE_LICENSE()`

Each module must have it for specifying the License.
