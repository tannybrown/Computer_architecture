   lw 0 1 data
   beq 2 3 end
   noop
   noop
   add 1 3 4 //branch  hazard(if we want avoid hazard, one more noop instruction is  needed)
end   halt
data .fill 10
