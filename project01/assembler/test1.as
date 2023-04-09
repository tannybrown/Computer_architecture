	lw 0 1 eight Load reg1 with 8(symbolic)
	lw 0 2 nine  Load reg2 with 9(symbolic)
    lw 0 6 one   Load reg6 with 1(symbolic)
loop	add 1 6 1 reg1 = reg1 + reg6
    beq 1 2 loop  if reg1=reg2, go loop
done halt           end of program
one     .fill 1
eight   .fill 8
nine    .fill 9
