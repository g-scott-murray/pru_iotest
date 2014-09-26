/*
 * prucomm.h - structure definitions for communication
 *
 */
#ifndef PRUCOMM_H
#define PRUCOMM_H

#include "pru_defs.h"

struct io_ctrl {
	u32 state;
	u32 pru1_r31;
};

/* The control structure is at the start of shared DPRAM */
#define IO_CTRL		((struct io_ctrl *)((void *)DPRAM_SHARED))

#define IO_STOP		0
#define IO_RUN		1

#endif
