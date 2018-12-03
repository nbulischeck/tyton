# Tyton

[![Build Status](https://travis-ci.com/nbulischeck/tyton.svg?branch=master)](https://travis-ci.com/nbulischeck/tyton)
[![license](https://img.shields.io/badge/license-GPL-brightgreen.svg)](https://github.com/nbulischeck/tyton/blob/master/LICENSE)
[![version](https://img.shields.io/badge/linux-4.4.0.31+-blue.svg?style=flat)](https://github.com/nbulischeck/tyton)

<img align="left" src="https://i.imgur.com/enDjxat.jpg" style="padding: 25px">

Linux Kernel-Mode Rootkit Hunter for 4.4.0-31+.

For more information, visit [Tyton's website](https://nbulischeck.github.io/tyton/).

## Detected Attacks

**Hidden Modules**

**Syscall Table Hooking**
  
**Network Protocol Hooking**

**Netfilter Hooking**

**Zeroed Process Inodes**

**Process Fops Hooking**

**Interrupt Descriptor Table Hooking**

&nbsp;

## Additional Features

**Notifications**: Users (including myself) do not actively monitor their journald logs, so a userland notification daemon has been included to monitor journald logs and display them to the user using libnotify. Notifications are enabled after install by XDG autorun, so if your DM does not have `/etc/xdg/autostart` it will fail.

**DKMS**: Dynamic Kernel Module Support has been added for Arch and Fedora/CentOS (looking to expand in the near future). DKMS allows the (near) seamless upgrading of Kernel modules during kernel upgrades. This is mainly important for distributions that provide rolling releases or upgrade their kernel frequently.

## Installing

### Dependencies

* Linux Kernel 4.4.0-31 or greater
* Corresponding Linux Kernel Headers
* GCC
* Make
* Libnotify
* Libsystemd
* Package Config
* GTK3

### From Source

#### Ubuntu/Debian/Kali

1. `sudo apt install linux-headers-$(uname -r) gcc make libnotify-dev pkg-config libgtk-3-dev libsystemd-dev`
2. `git clone https://github.com/nbulischeck/tyton.git`
3. `cd tyton`
4. `make`
5. `sudo insmod tyton.ko`

> **Note**: For Ubuntu 14.04, libsystemd-dev is named libsystemd-journal-dev.

#### Arch

1. `sudo pacman -S linux-headers gcc make libnotify libsystemd pkgconfig gtk3`
2. `git clone https://github.com/nbulischeck/tyton.git`
3. `cd tyton`
4. `make`
5. `sudo insmod tyton.ko`

> **Note**: It's recommended to install Tyton through the AUR so you can benefit from DKMS.

#### Fedora/CentOS

1. `dnf install kernel-devel gcc make libnotify libnotify-devel systemd-devel gtk3-devel gtk3`
2. `git clone https://github.com/nbulischeck/tyton.git`
3. `cd tyton`
4. `make`
5. `sudo insmod tyton.ko`

##### Kernel Module Arguments

The kernel module can be passed a specific timeout argument on insertion through the command line.

To do this, run the command `sudo insmod tyton.ko timeout=X` where X is the number of minutes you would like the kernel module to wait before executing its scan again.

### AUR

Tyton is available on the AUR [here](https://aur.archlinux.org/packages/tyton-dkms-git/).

You can install it using the AUR helper of your choice:

* `yaourt -S tyton-dkms-git`
* `yay -S tyton-dkms-git`
* `pakku -S tyton-dkms-git`
