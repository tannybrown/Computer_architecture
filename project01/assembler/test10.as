    lw 0 1 8   load reg1 with 7
    lw 0 2 one  load reg2 1
again add 1 0 3 reg3 = reg1 + reg0
    beq 1 3 1  if reg1 == reg3, go to end of program
    noop
    halt            end of program
one     .fill 1
two     .fill 2
test    .fill 7
