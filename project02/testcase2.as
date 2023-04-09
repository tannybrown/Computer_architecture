	lw 0 1 data
	noop
	noop
	noop
	nor 1 2 3//good we can avoid hazard
end	halt
data .fill 10
