	lw 0 1 wrong    check duplicate labels
	lw 0 2 test
	add 1 1 3
wrong	add 2 1 2
	add 3 2 4
	nor 3 4 5
	noop
	halt
wrong	.fill 4
test    .fill 4
