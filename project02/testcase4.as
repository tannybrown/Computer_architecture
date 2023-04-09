	lw 0 1 error
	noop
	noop
	noop
	add 1 2 3
	add 3 1 4//data hazard , if we want void data hazard we should avoid, we shoud add noop instruction between add instruction
	halt
error	.fill 4
