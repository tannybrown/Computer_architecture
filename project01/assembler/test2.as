    lw 0 1 a1   Load reg1 with 1(symbolic)
    lw 0 2 a2   Load reg2 with 5(symbolic)
    beq 5 2 last    if reg5= reg2 , go end of program
    add 1 1 3       reg3 = reg1 + reg1
    jalr 1 5        reg5 = 5, pc = 1
last    halt    end of program
a1  .fill 1
a2  .fill 5
