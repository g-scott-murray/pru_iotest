pru_iotest
==========

BeagleBone Black PRU I/O Test Firmware

Overview:
--------

This repository contains source code to build I/O testing firmware for the
programmable realtime units (PRUs) on the BeagleBone Black's TI AM335x SoC.
It can be used to test the available outputs and inputs from the PRUs.  The
output pins can be set to toggle at 1 Hz and the state of input only pins
can be read.  The driver uses pins:

  P8.11 - P8.12, P8.20 - P8.21, P8.27 - P8.30, P8.39 - P8.46, P9.24,
  P9.27 - P9.31, P9.41 - P9.42

as outputs, and pins:

  P8.15 - P8.16, P9.25 - P9.26

as inputs.  Note that HDMI and eMMC must be disabled for all of these pins
to be available for use.

Building:
--------

Compiling requires the Beta TI PRU code generation tools located at:

http://software-dl.ti.com/codegen/non-esd/downloads/beta.htm

The required installer may be downloaded by selecting “Linux” under the
“PRU Code Generation Tools” section.  Note that creating a ti.com login and
filling in a request form are required before the installer can be downloaded.

The Makefile in the src directory assumes that the PRU compiler environment
has been set up.  This means the bin directory under pru_2.0.0B2 should be in
PATH, and the environment variable PRU_C_DIR should be defined to point at the
lib and include directories under pru_2.0.0B2, separated with a semi-colon,
e.g.:

  export PRU_C_DIR="/foo/pru_2.0.0B2/lib;/foo/pru_2.0.0B2/include"

To build, run "make" in the src directory.

Usage:
-----

To use the driver firmware, copy the files iotest-pru0 and iotest-pru1 to
/lib/firmware.  Also copy the BB-BONE-PRU-07-00A0.dts file from the dts
directory to /lib/firmware.  Compile the devicetree overlay with:

  dtc -I dts -O dtb -o BB-BONE-PRU-06-00A0.dtbo -@ 0 BB-BONE-PRU-06-00A0.dts

The firmware may then be loaded by running the command:

  echo BB-BONE-PRU-07 > /sys/devices/bone_capemgr.9/slots
 
To verify that the driver firmware loaded correctly, look for the message:

  pru-rproc 4a300000.prurproc: P0 PUTS 'PRU0: Starting GPIO test firmware'

on either the serial console or in the output of the “dmesg” command.
Another check is to verify the presence of the “/dev/vport0p0” device node.

Once loaded, output pin toggling can be started by writing "r" (for run) to
the virtual serial port device, and stopped by writing "s".  The state of
the input only pins can be read by writing "i", and the state will be logged
via the kernel message log (readable via "dmesg").
