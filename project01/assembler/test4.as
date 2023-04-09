    lw 0 1 four Load reg1 with 4(symbolic)
    lw 1 2 2    Load reg2 with -1(numeric)
way nor 1 2 2   nor reg1 and reg2 -> 0
    noop        do nothing
    halt        end of program
four    .fill 4
minus   .fill -1
sybol   .fill way   contain address of way (2)
