	lw 0 1 error
	lw 0 2 1231415  offsetFields don't fit in 16bits
	lw 0 3 set
	lw 0 4 test
	noop
wow	beq 2 4 done
	add 1 2 3
    noop
	beq 0 0 wow
done	halt
	noop
error	.fill 5
off     .fill 2
set     .fill 1
test    .fill 4
