# Tyton

<img align="left" src="https://raw.githubusercontent.com/nbulischeck/tyton/master/images/tyton-large.jpg?token=ARN9vXUA3y4sqHCkZj_tM1s9Zlb4eW-vks5bu8I4wA%3D%3D" style="padding: 25px">

Linux Kernel-Mode Rootkit Hunter for 4.15+

## Detection Methods

**Syscall Table Hooking**: Search through the syscall table to see if any functions point outside the core kernel text section.
  
**Netfilter Hooking**: Search through all possible Netfilter hook combinations and report modules that have active Netfilter hooks.

**Hidden Modules**: Retrieve the kset `module_kset` from `sysfs` which holds references to all loaded kernel modules and traverse it to resolve the kobj back to its containing object.

**Zeroed /proc inodes**: Search through `/proc` for all `linux_dirent` structures and examine the inodes to find any set to 0.
