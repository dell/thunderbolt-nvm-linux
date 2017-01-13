Thunderbolt NVM updates for Linux
=====
This repository contains payloads that for updating the Thunderbolt NVM on select Dell systems.

Why
---
Intel is working on releasing a full NVM update solution for Linux users that will integrate
with [fwupd](http://github.com/hughsie/fwupd).

As of now (12/20/16) this full solution is not yet ready, but Intel has released tools
that advanced users can use to provide feedback to Intel and to perform NVM updates.

When the fwupd solution is complete upstream, the payloads will be moved to LVFS in .CAB files
and this page will be updated to point how to use the full upstream solution.

How
---
Intel's solution requires a series of kernel patches that have not yet been included upstream.
These patches are used for Thunderbolt networking, but also happen to be pre-requisites for
NVM query and NVM flash.

First integrate these [patches](https://github.com/01org/thunderbolt-software-kernel-tree/tree/networking)
 into locally.  

The `thunderbolt-icm` kernel module can be ran out of tree via [DKMS](https://github.com/dell/dkms) if you would prefer to not
compile a local kernel build.  
Dell has made available a DKMS deb package [here](https://github.com/dell/thunderbolt-icm-dkms).

Next, fetch the [thunderbolt user space stack](https://github.com/01org/thunderbolt-software-user-space/tree/fwupdate)

3 separate components will need to be built and installed in this order:

1. The Thunderbolt Daemon `thunderboltd`
2. The thunderbolt FW flash library `libtbtfwu`
3. The thunderbolt FW flash command line application `tbtfwucli`


Usage
---
After those components are all built and installed, you will be able to enumerate devices
using this command:
```
# tbtfwucli EnumControllers
```

If the controller does not enumerate, then it is most likely asleep. Plug in a thunderbolt
device to wake it up and try again.

If you don't have a Thunderbolt device, it's also possible to force the controller into
flash mode to wake it up using a utility that is on this repository.

Once you have succesfully enumerated the controller ID, you can query the current NVM version 
using this command:
```
# tbtfwucli GetCurrentNVMVersion $CONTROLLERID
```

You can query the model using this command:
```
# tbtfwucli GetModelID $CONTROLLERID
```

Find the firmware payload in this repository that corresponds to that model and you can
validate it using this command:
```
# tbtfwucli ValidateFWImage $CONTROLLERID $PAYLOAD
```

Finally you can flash the payload using this command:
```
# tbtfwucli FWUpdate $CONTROLLERID $PAYLOAD
```

It's important that the controller stay awake during this time. If you woke it using
another device, leave that device plugged in until the update is done.
If you woke it using the Dell force tool then you don't need to do anything.

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

Feedback
---
Kernel feedback can be provided on the most recent kernel submission.
As of 12/20 that's [version 9](http://www.spinics.net/lists/linux-pci/msg55813.html)

Issues with the userspace tools should be filed with [project on github](https://github.com/01org/thunderbolt-software-user-space/tree/fwupdate)

To provide other general feedback:
 - thunderbolt-software@lists.01.org 
 - Register at: https://lists.01.org/mailman/listinfo/thunderbolt-software 
 - Archives at: https://lists.01.org/pipermail/thunderbolt-software/ 

