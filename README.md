Thunderbolt NVM updates for Linux
=====
This repository contains information on updating Thunderbolt NVM on applicable Dell systems.

Why
---
Tools are available in the kernel and [fwupd](https://github.com/hughsie/fwupd).
Some payloads are released onto LVFS in .CAB form.

How
---
Kernel 4.13 and later are needed for doing updates. They contain a native "Thunderbolt" driver.
Fwupd 0.9.7 or later are needed to safely process updates from userspace.

Kernel 4.15 and later include a special driver for waking up sleeping thunderbolt controllers.
Fwupd 1.0.0 and later include the userspace support necessarily for using this driver.

Currently payloads are posted to LVFS testing. To use them, you will need to activate the
LVFS testing remote.
To do this, modify:
```
/etc/lvfs/remotes.d/lvfs-testing.conf
```
Set `Enabled` to `true`.

Usage
---
Devices can be enumerated using ```#fwupdmgr get-devices```

If the controller does not enumerate, then it is most likely asleep. Plug in a thunderbolt
device to wake it up and try again.

If you don't have a Thunderbolt device, it's also possible to force the controller into
flash mode to wake it up using a utility that is on this repository or a newer kernel/fwupd.

It's important that the controller stay awake during this time. If you woke it using
another device, leave that device plugged in until the update is done.
If you woke it using the Dell force tool or kernel driver then you don't need to do anything.

Bugs
---
If an NVM is missing or you run into a problem, you can report it here or with the fwupd
project.

Dell Force Tool
---
A tool is provided in this repository that can force a Thunderbolt controller on a Dell
system into flashing mode. When in flashing mode the controller won't go to sleep, so be
sure to take it out of flashing mode when you are done with interactions.

Pre-requisites:

1. [libsmbios](https://github.com/dell/libsmbios) development libraries
2. [efivar](https://github.com/rhinstaller/efivar) development libraries

To compile the tool:
```
# gcc -o force_dell_tbt force_dell_tbt.c -I /usr/include/efivar/ -lsmbios_c -lefivar
```

To force flashing mode:
```
# sudo ./force_dell_tbt 1
```

To force user mode:
```
# sudo ./force_dell_tbt 0
```
