Thunderbolt NVM updates for Linux
=====
This repository contains payloads that for updating the Thunderbolt NVM on select Dell systems.

Why
---
Although tools are available in the kernel and [fwupd](https://github.com/hughsie/fwupd), payloads 
aren't yet released onto LVFS in .CAB form.

How
---
Kernel 4.13 and later are needed for doing updates. They contain a native "Thunderbolt" driver.
Fwupd 0.9.7 or later are needed to safely process updates from userspace.

Kernel 4.15 and later include a special driver for waking up sleeping thunderbolt controllers.
Fwupd 1.0.0 and later include the userspace support necessarily for using this driver.

Usage
---
Devices can be enumerated using ```#fwupdmgr get-devices```

If the controller does not enumerate, then it is most likely asleep. Plug in a thunderbolt
device to wake it up and try again.

If you don't have a Thunderbolt device, it's also possible to force the controller into
flash mode to wake it up using a utility that is on this repository or a newer kernel/fwupd.

To build a CAB file from these payloads, refer to the firmware packager available here:
https://github.com/hughsie/fwupd/tree/master/contrib/firmware-packager

It's important that the controller stay awake during this time. If you woke it using
another device, leave that device plugged in until the update is done.
If you woke it using the Dell force tool or kernel driver then you don't need to do anything.

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
