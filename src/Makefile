#
# Makefile for GPIO testing remote proc PRU firmware.
#
# It is assumed that the PRU compiler environment has been set.
# This means the bin directory under pru_2.0.0B2 should be in
# PATH, and the environment variable PRU_C_DIR should be defined
# to point at the lib and include directories under pru_2.0.0B2,
# separated with a semi-colon, e.g.:
#
# export PRU_C_DIR="/foo/pru_2.0.0B2/lib;/foo/pru_2.0.0B2/include"
#

.PHONY: all clean

all: iotest-pru0.stripped iotest-pru0.lst \
	iotest-pru1.stripped iotest-pru1.lst

CC := clpru
LD := lnkpru
AR := arpru
OBJDUMP := dispru
STRIP := strippru

# -v3				PRU version 3
# --c99 			C99 support
# --gcc 			Enable GCC extensions
# -O3				Optimization level maximum
# --printf_support=minimal 	Minimal printf
# -ppd				Generate dependencies *.pp
# -ppa				Continue after generating deps
# -DDEBUG			Enable debug
# CFLAGS= -v3 --c99 --gcc -O3 --printf_support=minimal -ppd -ppa -DDEBUG 
CFLAGS := -v3 --c99 --gcc -O3 -I ./include --printf_support=minimal -ppd -ppa --symdebug:none

# -cr 				Link using RAM auto init model (loader assisted)
# -x				Reread libs until no unresolved symbols found
LDFLAGS := -cr --diag_warning=225 -llnk-am33xx.cmd -x

STRIPFLAGS := -p

OBJS0 := iotest-pru0.obj syscall0.obj debug.obj pru_vring.obj
OBJS1 := iotest-pru1.obj iotest-inc.obj asmutil.obj syscall1.obj debug.obj

%.obj: %.c
	$(CC) $(CFLAGS) -c $<

%.obj: %.asm
	$(CC) $(CFLAGS) -c $<

iotest-pru0: $(OBJS0)
	$(CC) $(CFLAGS) $^ -z $(LDFLAGS) -o $@

iotest-pru1: $(OBJS1)
	$(CC) $(CFLAGS) $^ -z $(LDFLAGS) -o $@

%.stripped: %
	$(STRIP) $(STRIPFLAGS) $< -o $@

%.lst: %
	$(OBJDUMP) -1 $< > $@

clean:
	rm -f *.obj *.lst *.out *.stripped \
		iotest-pru0 iotest-pru1 tags

distclean: clean
	rm -f *.pp

# include any generated deps
-include $(patsubst %.obj,%.pp,$(OBJS))
