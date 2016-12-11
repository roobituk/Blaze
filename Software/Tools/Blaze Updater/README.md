=========================
Blaze Updater
=========================

Blaze Updater is a utility which allows the downloading of a Blaze application via Blaze Bootloader.

To start Blaze in bootloader mode, connect Blaze to USB and tap the screen while the Blaze logo shows. Doing this will force
bootloader mode and in turn, will allow Blaze to enumerate as a USB serial port device. Once the device has enumerated and
has been given a COM port number, select the correct port from within the Blaze Updater utility and then click the "Program
Firmware" button to choose a binary image.

*** NOTE ***
It is vital, that any application being loaded to Blaze via the bootloader has the correct linker configuration settings. The
bootloader resides in the lower 64kB of flash so all vector tables, etc must be relocated accordingly. Failure to do this will
prevent any loaded application from starting correctly.

