;*
;* Various assembly utils
;*

        .text

	;* Save on call: R0-R1, R14-R29
	;* Save on entry: R3.w2-R13

        .global delay_cycles

delay_cycles:
        .asg r14, nr

	SUB R14, R14, 5		;* function entry + overhead
	LSR R14, R14, 1		;* loop is by two
$L1:	SUB R14, R14, 1
	QBNE $L1, R14, 0
        JMP R3.w0

	.global delay_cycles_accurate
delay_cycles_accurate:
	SUB R14, R14, 3
	QBEQ $L10, R14.w2, 0
	LDI R0, 0		;* R0 = 0
$L12:	SUB R0.w0, R0.w0, 1	;* 0 - 1 = 0xffffffff
$L11:	LOOP $L11, R0.w0	;* loop 65535 times + 1 65536
	SUB R14.w2, R14.w2, 1
	QBNE $L12, R14.w2, 0
$L10:	LOOP $L10, R14.w0
	JMP R3.w0

	.global delay_cycles_accurate2
delay_cycles_accurate2:
	SUB R14, R14, 5		;* subtract overhead
	LSR R0, R14, 1		;* divide by two
	QBBC $L20, R14, 0	;* test low bit
	NOP			;* add one cycle for odd
$L20:	SUB R0, R0, 1		;* loop high
	QBNE $L20, R0, 0
	JMP R3.w0
