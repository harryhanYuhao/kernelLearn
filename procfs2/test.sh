#!/bin/sh 

sudo rmmod procfs2
sudo -E make
sudo -E insmod procfs2.ko
echo "A message!" > /proc/buffer1k
