/*
 *
 * PRU Remoteproc GPIO test firmware
 *
 * Based on an original work of Pantelis Antoniou
 *
 * Copyright 2014, Scott Murray. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are
 * permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 *    conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 *    of conditions and the following disclaimer in the documentation and/or other materials
 *    provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#define PRU1

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <strings.h>

#include "debug.h"
#include "pru_defs.h"
#include "prucomm.h"

#include "syscall.h"

// NOTE: Extra pinmux manipulation required to safely use bits 4 & 6
#define PRU0_R30_BITS	( \
	BIT( 0)|BIT( 1)|BIT( 2)|BIT( 3)|BIT( 4)|BIT( 5)|BIT( 6)|BIT( 7)| \
	BIT(14)|BIT(15) \
	)

#define PRU1_R30_BITS	( \
	BIT( 0)|BIT( 1)|BIT( 2)|BIT( 3)|BIT( 4)|BIT( 5)|BIT( 6)|BIT( 7)| \
	BIT( 8)|BIT( 9)|BIT(10)|BIT(11)|BIT(12)|BIT(13)	\
	)

extern void delay_cycles(u32 delay);
extern void delay_cycles_accurate(u32 delay);
extern void delay_cycles_accurate2(u32 delay);

int main(int argc, char *argv[])
{
	/* enable OCP master port */
	PRUCFG_SYSCFG &= ~SYSCFG_STANDBY_INIT;
	sc_printf("PRU1: Starting GPIO test firmware");

	// Start in off state
	__R30 &= ~PRU1_R30_BITS;
	pru_other_and_or_reg(30, ~PRU0_R30_BITS, 0);

	IO_CTRL->pru1_r31 = __R31;

	sc_puts("PRU1: go");

	for (;;) {
		// Signalled interrupt from either PRU0 or host
		if (pru_signal()) {
			if (PINTC_SRSR0 & (1 << SYSEV_OTHER_PRU_TO_THIS_PRU)) {
				PINTC_SICR = SYSEV_OTHER_PRU_TO_THIS_PRU;

				sc_printf("PRU signal");
			}
			if (PINTC_SRSR0 & (1 << SYSEV_ARM_TO_THIS_PRU)) {
				PINTC_SICR = SYSEV_ARM_TO_THIS_PRU;
				sc_puts("ARM signal");
			}
		}
		if(IO_CTRL->state == IO_STOP)
			continue;

		IO_CTRL->pru1_r31 = __R31;

		// Turn on
		__R30 |= PRU1_R30_BITS;
		pru_other_and_or_reg(30, 0xffffffff, PRU0_R30_BITS);

		// Delay
		delay_cycles(PRU_200MHz_ms(500));

		// Turn off
		__R30 &= ~PRU1_R30_BITS;
		pru_other_and_or_reg(30, ~PRU0_R30_BITS, 0);

		// Delay
		delay_cycles(PRU_200MHz_ms(500));
	}
}
