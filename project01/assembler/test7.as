	lw 0 1 wrong    
	lv 0 2 inst     unrecognized opcodes
	add 2 1 3
	add 5 1 2
    beq 1 2 2
	nor 3 4 5
	noop
	halt
wrong	.fill 10
inst   .fill 20
