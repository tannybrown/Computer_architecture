	lw 0 1 three
	noop
	noop
	noop
	sw 0 1 2// good we can avoid hazard
	halt
three	.fill 3
