# Some Utils for Kernel Development

- `logger <message>`: write to syslog
    -`logger -p local0.notice -t ${0##*/}[$$] Hello world`: take note the calling program
- `systemd-cat`: similar to logger. 
    - `cat afile | systemd-cat`
