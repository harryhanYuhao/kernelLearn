# udev:  dynamic device management

udev, a user-space program,  supplies system software with permission management, device event, creation of device file in `\dev`, among others. 

THe udev daemon, `systemd-udev.service(8)`, received all device uevents from the kernel, whenever the state of the device changed (addition, removal, etc). It then matches its configured set of rules to identify the devuce.

Device information are stored in udev database which can be accessed through the library `libudev`.

## Rules Files

The udev read the rule files from 
>the files located in the system
       rules directories /usr/lib/udev/rules.d and
       /usr/local/lib/udev/rules.d, the volatile runtime directory
       /run/udev/rules.d and the local administration directory
       /etc/udev/rules.d.
Importantly:
>All rules files are collectively sorted and
       processed in lexical order, regardless of the directories in
       which they live. However, files with identical filenames
       replace each other. Files in /etc/ has highest priority, files in /run/ takes precedence over file of the same name under /usr/.
Thus, to override a system-supplied rules, create file of the same name in /etc/ or /run/. 

*Moreover*,
A symlink in /etc/ pointing /dev/null disable the rule file of the same name entirely.

### Reference

- `man udev`
- udev on [archlinux.org](https://wiki.archlinux.org/title/udev)

## udevadm - udev management tool

To query the information of a device (such as /dev/video2), use 
```
udevadm info --attribute-walk --path=$(udevadm info --query=path --name=/dev/video2)
```
