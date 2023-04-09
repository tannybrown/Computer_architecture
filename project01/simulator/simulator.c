/* LC-2K Instruction-level simulator */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define NUMMEMORY 65536 /* maximum number of words in memory */
#define NUMREGS 8 /* number of machine registers */
#define MAXLINELENGTH 1000

typedef struct stateStruct {
 int pc;
 int mem[NUMMEMORY];
 int reg[NUMREGS];
 int numMemory;
} stateType;

void printState(stateType *);

int convertNum(int num)
{
    /* convert a 16-bit number into a 32-bit Linux integer */
    if (num & (1<<15) ) {
    num -= (1<<16);
    }
    return(num);
}

int 
main(int argc, char *argv[])
{ 
    char line[MAXLINELENGTH];
    stateType state;
    FILE *filePtr;

    if (argc != 2) {
        printf("error: usage: %s <machine-code file>\n", argv[0]);
        exit(1);
    }
    filePtr = fopen(argv[1], "r");
    if (filePtr == NULL) {
        printf("error: can't open file %s", argv[1]);
        perror("fopen");
        exit(1);
    }

    /* read in the entire machine-code file into memory */
    for (state.numMemory = 0; fgets(line, MAXLINELENGTH, filePtr) != NULL;state.numMemory++) {
        if (sscanf(line, "%d", state.mem+state.numMemory) != 1) {
        printf("error in reading address %d\n", state.numMemory);
        exit(1);
        }
        printf("memory[%d]=%d\n", state.numMemory, state.mem[state.numMemory]);
    }
    memset(state.reg, 0, (sizeof(int) * NUMREGS));
    state.pc = 0;
    int inst_count = 0;

    while(1)
    {

        printState(&state);

        int machinecode = state.mem[state.pc];
        int opcode = machinecode >> 22;
        //add
        if(opcode == 0){
            int regA = (machinecode >>19) & 0x00000007;
            int regB = (machinecode >>16) & 0x00000007;
            int destReg = (machinecode & 0x00000007);
            state.reg[destReg] = state.reg[regA] + state.reg[regB];
        }
        //nor
        else if(opcode == 1){
            int regA = (machinecode >>19) & 0x00000007;
            int regB = (machinecode >>16) & 0x00000007;
            int destReg = (machinecode & 0x00000007);
            state.reg[destReg] = ~(state.reg[regA] | state.reg[regB]);
        }
        //lw
        else if(opcode == 2){
            int regA = (machinecode >>19) & 0x00000007;
            int regB = (machinecode >>16) & 0x00000007;
            int offsetField = (machinecode & 0x0000FFFF);
            state.reg[regB] = state.mem[convertNum(offsetField + state.reg[regA])];
        }
        //sw
        else if(opcode == 3){
            int regA = (machinecode >>19) & 0x00000007;
            int regB = (machinecode >>16) & 0x00000007;
            int offsetField = (machinecode & 0x0000FFFF);
            state.mem[convertNum(offsetField + state.reg[regA])] = state.reg[regB];
        }
        //beq
        else if(opcode == 4){
            int regA = (machinecode >>19) & 0x00000007;
            int regB = (machinecode >>16) & 0x00000007;
            int offsetField = (machinecode & 0x0000FFFF);        
            if (state.reg[regA] == state.reg[regB])
					state.pc += convertNum(offsetField);    
        }
        //jalr
        else if(opcode == 5){
            int regA = (machinecode >>19) & 0x00000007;
            int regB = (machinecode >>16) & 0x00000007;
            state.reg[regB] = state.pc + 1;
            state.pc = state.reg[regA] - 1;
        }
        //halt
        else if(opcode == 6){
            state.pc++;
            inst_count++;
            goto out;
        }
        //noop
        else if(opcode == 7){
            
        }else{
        printf("wrong opcode\n");
        exit(1);
        }

    state.pc++;
    inst_count++;
    }
    out:
	printf("\nmachine halted\n");
	printf("total of %d instructions executed\n", inst_count);
	printf("final state of machine:\n");
	
	printState(&state);

    exit(0);
    return(0);
}


void printState(stateType *statePtr)
{ int i;
 printf("\n@@@\nstate:\n");
 printf("\tpc %d\n", statePtr->pc);
 printf("\tmemory:\n");
 for (i=0; i<statePtr->numMemory; i++) {
 printf("\t\tmem[ %d ] %d\n", i, statePtr->mem[i]);
 }
 printf("\tregisters:\n");
 for (i=0; i<NUMREGS; i++) {
 printf("\t\treg[ %d ] %d\n", i, statePtr->reg[i]);
 }
 printf("end state\n");
}
