;*
;* Various assembly utils
;*

	;*
	;* Constants
	;*

	.asg C0, CONST_PRUSSINTC
	.asg C2, CONST_PRUCFG
	.asg 36, SICR_OFFSET

	.asg 0x02000, CONST_OTHERPRU_MEM
	.asg 0x10000, CONST_SHARED_MEM

	.asg 17, PRU0_PRU1_INTERRUPT
	.asg 18, PRU1_PRU0_INTERRUPT

	;* Constants specifying the bits in the R30 GPIO register to use for
	;* controlling the zero and one lines.
	.asg 0, R30_ZERO_BIT
	.asg 1, R30_ONE_BIT
	
	.asg R1, WIDTH
	.asg R2, INTERVAL
	.asg R3, EOT
	.asg R4, BITS
	.asg R5.b0, DATA
	.asg R5.b1, BITS_OUT
	.asg R5.b2, BIT
	.asg R6, OFFSET

	;*
	;* Spin around and kill time. 5 nanoseconds per cycle.
	;* 

DELAY_CYCLES .macro REG
	MOV R14, REG
	SUB R14, R14, 3		;* function entry + overhead
	LSR R14, R14, 1		;* loop is by two
$1:
	SUB R14, R14, 1
	QBNE $1, R14, 0

	.endm
