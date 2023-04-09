    lw 0 1 arg1 Load reg1 with 2 
    lw 0 2 arg2 Load reg2 with -5
    nor 1 2 3   reg3 = nor reg1 and reg2 
    add 3 1 4   reg4 = reg3 + reg1
    beq 5 4 done    if reg5 = reg4 , go to end of program
    jalr 1 5    reg5 =6, pc=2
done    halt        end of program
arg1    .fill 2
arg2    .fill -5
