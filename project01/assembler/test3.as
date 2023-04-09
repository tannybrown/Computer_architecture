    lw 0 1 new Load reg1 with 5(symbolic)
    sw 0 2 old Store reg2 into mem[7]
    lw 0 3  old Load reg3 with 0(symbolic)
    beq 2 3 ro if reg2 = reg3,  go to ro
en  halt    end of program
ro  beq 4 5 en if reg4 = reg5, go to end of program
new .fill 5
old .fill 7
