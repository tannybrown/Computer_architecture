#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUMMEMORY 65536 /* maximum number of data words in memory */
#define NUMREGS 8 /* number of machine registers */
#define ADD 0
#define NOR 1
#define LW 2
#define SW 3
#define BEQ 4
#define JALR 5 /* JALR will not implemented for this project */
#define HALT 6
#define NOOP 7
#define NOOPINSTRUCTION 0x1c00000
#define MAXLINELENGTH 1000

typedef struct IFIDStruct {
int instr;
int pcPlus1;
} IFIDType;

typedef struct IDEXStruct {
int instr;
int pcPlus1;
int readRegA;
int readRegB;
int offset;
} IDEXType;

typedef struct EXMEMStruct {
int instr;
int branchTarget;
int aluResult;
int readRegB;
} EXMEMType;

typedef struct MEMWBStruct {
int instr;
int writeData;
} MEMWBType;

typedef struct WBENDStruct {
int instr;
int writeData;
} WBENDType;

typedef struct stateStruct {
int pc;
int instrMem[NUMMEMORY];
int dataMem[NUMMEMORY];
int reg[NUMREGS];
int numMemory;
IFIDType IFID;
IDEXType IDEX;
EXMEMType EXMEM;
MEMWBType MEMWB;
WBENDType WBEND;
int cycles; /* number of cycles run so far */
} stateType;




void
printState(stateType *statePtr)
{
int i;
printf("\n@@@\nstate before cycle %d starts\n", statePtr->cycles);
printf("\tpc %d\n", statePtr->pc);
printf("\tdata memory:\n");
for (i=0; i<statePtr->numMemory; i++) {
printf("\t\tdataMem[ %d ] %d\n", i, statePtr->dataMem[i]);
}
printf("\tregisters:\n");
for (i=0; i<NUMREGS; i++) {
printf("\t\treg[ %d ] %d\n", i, statePtr->reg[i]);
}
printf("\tIFID:\n");
printf("\t\tinstruction ");
printInstruction(statePtr->IFID.instr);
printf("\t\tpcPlus1 %d\n", statePtr->IFID.pcPlus1);
printf("\tIDEX:\n");
printf("\t\tinstruction ");
printInstruction(statePtr->IDEX.instr);
printf("\t\tpcPlus1 %d\n", statePtr->IDEX.pcPlus1);
printf("\t\treadRegA %d\n", statePtr->IDEX.readRegA);
printf("\t\treadRegB %d\n", statePtr->IDEX.readRegB);
printf("\t\toffset %d\n", statePtr->IDEX.offset);
printf("\tEXMEM:\n");
printf("\t\tinstruction ");
printInstruction(statePtr->EXMEM.instr);
printf("\t\tbranchTarget %d\n", statePtr->EXMEM.branchTarget);
printf("\t\taluResult %d\n", statePtr->EXMEM.aluResult);
printf("\t\treadRegB %d\n", statePtr->EXMEM.readRegB);
printf("\tMEMWB:\n");
printf("\t\tinstruction ");
printInstruction(statePtr->MEMWB.instr);
printf("\t\twriteData %d\n", statePtr->MEMWB.writeData);
printf("\tWBEND:\n");
printf("\t\tinstruction ");
printInstruction(statePtr->WBEND.instr);
printf("\t\twriteData %d\n", statePtr->WBEND.writeData);
}
int
field0(int instruction)
{
return( (instruction>>19) & 0x7);
}

int
field1(int instruction)
{
return( (instruction>>16) & 0x7);
}

int
field2(int instruction)
{
return(instruction & 0xFFFF);
}

int
opcode(int instruction)
{
return(instruction>>22);
}

void
printInstruction(int instr)
{
char opcodeString[10];
if (opcode(instr) == ADD) {
strcpy(opcodeString, "add");
} else if (opcode(instr) == NOR) {
strcpy(opcodeString, "nor");
} else if (opcode(instr) == LW) {
strcpy(opcodeString, "lw");
} else if (opcode(instr) == SW) {
strcpy(opcodeString, "sw");
} else if (opcode(instr) == BEQ) {
strcpy(opcodeString, "beq");
} else if (opcode(instr) == JALR) {
strcpy(opcodeString, "jalr");
} else if (opcode(instr) == HALT) {
strcpy(opcodeString, "halt");
} else if (opcode(instr) == NOOP) {
strcpy(opcodeString, "noop");
} else {
strcpy(opcodeString, "data");
}
printf("%s %d %d %d\n", opcodeString, field0(instr), field1(instr),
field2(instr));
}

int convertNum(int num) {
	// Converts 16-bit number into a 32-bit number
	if (num & (1 << 15)) num -= (1 << 16);
	return num;
}


int 
main(int argc, char *argv[])
{ 
    char line[MAXLINELENGTH];
    stateType state,newState;
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
        if (sscanf(line, "%d", state.instrMem+state.numMemory) != 1) {
        printf("error in reading address %d\n", state.numMemory);
        exit(1);
        }
        printf("memory[%d]=%d\n", state.numMemory, state.instrMem[state.numMemory]);
    }

	printf( "%d memory words\n", state.numMemory );
	printf( "\tinstruction memory: \n" );

	for(int i = 0; i < state.numMemory; i++)
	{
        state.dataMem[i] = state.instrMem[i];
		printf( "\t\tinstrMem[ %d ] ", i );
		printInstruction(state.instrMem[i]);
	}

    //initialization
	state.cycles = 0;
	state.pc = 0;

	for (int i = 0; i < NUMREGS; i++){
		state.reg[i] = 0;
    }
	state.IFID.instr = NOOPINSTRUCTION;
	state.IDEX.instr = NOOPINSTRUCTION;
	state.EXMEM.instr = NOOPINSTRUCTION;
	state.MEMWB.instr = NOOPINSTRUCTION;
	state.WBEND.instr = NOOPINSTRUCTION;



    while (1) {
    printState(&state);
    /* check for halt */
    if (opcode(state.MEMWB.instr) == HALT) {
        printf("machine halted\n");
        printf("total of %d cycles executed\n", state.cycles);
        exit(0);
    }
    newState = state;
    newState.cycles++;
    /* --------------------- IF stage --------------------- */
    // Use PC to index memory to read instruction
    newState.IFID.instr = state.instrMem[state.pc];

    // Increment the PC (assume no branches for now)
    newState.pc = state.pc+ 1;
    newState.IFID.pcPlus1 = state.pc+ 1;
    /* --------------------- ID stage --------------------- */
    // Decode is easy, just pass on the opcode and let later stages figure out their own control signals for the instruction.
    newState.IDEX.instr = state.IFID.instr;
    newState.IDEX.readRegA = state.reg[field0(state.IFID.instr)];
    newState.IDEX.readRegB = state.reg[field1(state.IFID.instr)];
    newState.IDEX.offset = convertNum(field2(state.IFID.instr));
    newState.IDEX.pcPlus1 = state.IFID.pcPlus1;

    /* --------------------- EX stage --------------------- */
    // The inputs are the contents of regA and either the contents of regB or the offset field on the instruction
    newState.EXMEM.instr = state.IDEX.instr;
    newState.EXMEM.readRegB = state.IDEX.readRegB;


    int opEX = opcode(state.IDEX.instr);
    int regA = state.IDEX.readRegA;
    int regB = state.IDEX.readRegB;
    int off = state.IDEX.offset;

    switch(opEX) {
			case ADD:
				newState.EXMEM.aluResult = regA + regB;
				break;
			case NOR:
				newState.EXMEM.aluResult = ~(regA | regB);
				break;
			case LW:
                newState.EXMEM.aluResult = regA + off;
                break;
			case SW:
				newState.EXMEM.aluResult = regA + off;
				break;
			case BEQ:
                    newState.EXMEM.aluResult = regA - regB;
            	break;
			case JALR:
			    break;
            case HALT:
				break;
			case NOOP:
				break;
			default: 
				break;
		}

    // Also, calculate PC+1+offset in case this is a branch.
    newState.EXMEM.branchTarget = state.IDEX.offset + state.IDEX.pcPlus1;

    /* --------------------- MEM stage --------------------- */
    newState.MEMWB.instr = state.EXMEM.instr;
    int opMEM = opcode(state.EXMEM.instr);
    switch(opMEM){
            case ADD:
                newState.MEMWB.writeData = state.EXMEM.aluResult;
                break;
            case NOR:
                newState.MEMWB.writeData = state.EXMEM.aluResult;
                break;
            case LW:
                newState.MEMWB.writeData = state.dataMem[state.EXMEM.aluResult];
                break;
            case SW:
                newState.dataMem[state.EXMEM.aluResult] = state.EXMEM.readRegB;
                break;
            case BEQ:
                if(state.EXMEM.aluResult == 0){
                newState.pc = state.EXMEM.branchTarget;
                }
                break;
            case JALR:
                break;
            case HALT:
                break;
            case NOOP:
                break;
            default:
                break;
    }
    // ALU result contains address for ld and st instructions.

    // Opcode bits control memory R/W and enable signals.

    /* --------------------- WB stage --------------------- */
    newState.WBEND.instr = state.MEMWB.instr;
    newState.WBEND.writeData = state.MEMWB.writeData;
    
    int opWB = opcode(state.MEMWB.instr);
    int destReg;
    // Write MemData to destReg for ld instruction
    if(opWB == LW){
        destReg = field1(state.MEMWB.instr);
        newState.reg[destReg] = state.MEMWB.writeData;
    }
    // Write ALU result to destReg for add or nand instructions.
    if(opWB == ADD){
        destReg = field2(state.MEMWB.instr);
        newState.reg[destReg] = state.MEMWB.writeData;
    }
     if(opWB == NOR){
        destReg = field2(state.MEMWB.instr);
        newState.reg[destReg] = state.MEMWB.writeData;
    }
    // Opcode bits also control register write enable signal.


    state = newState; /* this is the last statement before end of the loop.
    It marks the end of the cycle and updates the
    current state with the values calculated in this
    cycle */
    }
    return(0);
}





