#!/bin/bash
qemu-system-x86_64 -hda Image/x64BareBonesImage.qcow2 -m 1024 -soundhw pcspk  -chardev stdio,id=seabios -device isa-debugcon,iobase=0x402,chardev=seabios
