/* Assembler code fragment for LC-1K */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define MAXLINELENGTH 1000
#define NUMMEMORY 65536 /* maximum number of words in memory */

int readAndParse(FILE *, char *, char *, char *, char *, char *);
int isNumber(char *);
int rtype(char*,char*,char*);
int itype(char*,char*,char*,int,int);
int jtype(char*,char*);
int filltype(char*,int);

int inst_count=0;
char labelList[NUMMEMORY][7];

int
main(int argc, char *argv[])
{
	char *inFileString, *outFileString;
	FILE *inFilePtr, *outFilePtr;
	char label[MAXLINELENGTH], opcode[MAXLINELENGTH], arg0[MAXLINELENGTH], 
		 arg1[MAXLINELENGTH], arg2[MAXLINELENGTH];
	
	// input check
	if (argc != 3) {
		printf("error: usage: %s <assembly-code-file> <machine-code-file>\n", argv[0]);
		exit(1);
	}

	inFileString = argv[1];
    outFileString =argv[2];

	// input file check
	inFilePtr = fopen(inFileString, "r");
	if (inFilePtr == NULL) {
		printf("error in opening %s\n", inFileString);
		exit(1);
	}

	// output file check
	outFilePtr = fopen(outFileString, "w");
	if (outFilePtr == NULL) {
		printf("error in opening %s\n", outFileString);
		exit(1);
	}

int lineNumber=0;
//First pass
while(1){
 /* here is an example for how to use readAndParse to read a line from
 inFilePtr */
 if (! readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2) ) {
 /* reached end of file */
 break;
   }
   //label length chek
if(strlen(label) > 6) {
   printf("undefined labels\n");
   exit(1);
}

//is there any same label?
   if(strcmp(label,"")){
      if(lineNumber == 0)
        {lineNumber = 0;}
      else{
         for(int i = 0; i < lineNumber;i++){
          
            if(strcmp(labelList[i],label) == 0){
               printf("duplicate labels\n");
               exit(1);
            }
         }
      }
   }else {lineNumber++; continue;}
   
   strcpy(labelList[lineNumber++],label);
   
}
 /* this is how to rewind the file ptr so that you start reading from the
 beginning of the file */
 rewind(inFilePtr);
 /* after doing a readAndParse, you may want to do the following to test the
 opcode */
int machinecode[NUMMEMORY];

//Second Pass
 while(1){
    if(!readAndParse(inFilePtr,label,opcode,arg0,arg1,arg2))
    break;


//add
 if (!strcmp(opcode, "add")) {
 /* do whatever you need to do for opcode "add" */
   int add = rtype(arg0,arg1,arg2);
   if(add == -1){
      printf("line number %d, argument of add is wrong!",inst_count);
      exit(1);
   }
   machinecode[inst_count++] = (0x00000000 | add);
 }

 //nor
 else if(!strcmp(opcode,"nor")){
int nor = rtype(arg0,arg1,arg2);
if(nor== -1){
      printf("line number %d, argument of nor is wrong!",inst_count);
      exit(1);
   }
   machinecode[inst_count++] = (0x00400000 | nor);
 }

//lw
else if(!strcmp(opcode,"lw")){
int bflag = 0;
int lw = itype(arg0,arg1,arg2,lineNumber,bflag);
if(lw== -1){
      printf("line number %d, argument of lw is wrong!",inst_count);
      exit(1);
   }
 machinecode[inst_count++] = (0x00800000 | lw);

}

//sw
else if(!strcmp(opcode,"sw")){
   int bflag=0;
   int sw = itype(arg0,arg1,arg2,lineNumber,bflag);
   if(sw== -1){
      printf("line number %d, argument of sw is wrong!",inst_count);
      exit(1);
   }
 machinecode[inst_count++] = (0x00C00000 | sw);
}

//beq
else if(!strcmp(opcode,"beq")){
   int bflag =1;
   int beq = itype(arg0,arg1,arg2,lineNumber,bflag);
   if(beq == -1){
      printf("line number %d, argument of beq is wrong!",inst_count);
      exit(1);
   }
 machinecode[inst_count++] = (0x01000000 | beq);
}

//jalr
else if(!strcmp(opcode,"jalr")){
   int jalr = jtype(arg0,arg1);
   if(jalr== -1){
      printf("line number %d, argument of jalr is wrong!",inst_count);
      exit(1);
   }
   machinecode[inst_count++] = (0x01400000 | jalr);
}

//halt
else if(!strcmp(opcode,"halt")){

 machinecode[inst_count++] = 0x01800000;
}

//noop
else if(!strcmp(opcode,"noop")){

 machinecode[inst_count++] = 0x01C00000;
}

//.fill
else if(!strcmp(opcode,".fill")){
   int fill = filltype(arg0, lineNumber);
   machinecode[inst_count++] = fill;
}
else{
printf("%s is wrong opcode. please check your assembly file.\n",opcode);
exit(1);

   }

}

 //print
for (int i = 0; i < lineNumber; i++) {
        printf("(address %d): %d (hex 0x%x)\n", i, machinecode[i], machinecode[i]);
        if (outFilePtr != NULL)
            fprintf(outFilePtr, "%d\n", machinecode[i]);
    }


 exit(0);
 return(0);
}/*
* Read and parse a line of the assembly-language file. Fields are returned
* in label, opcode, arg0, arg1, arg2 (these strings must have memory already
* allocated to them). *
* Return values:
* 0 if reached end of file
* 1 if all went well *
* exit(1) if line is too long.
*/


int readAndParse(FILE *inFilePtr, char *label, char *opcode, char *arg0,
 char *arg1, char *arg2)
{ char line[MAXLINELENGTH];
 char *ptr = line;
 /* delete prior values */
 label[0] = opcode[0] = arg0[0] = arg1[0] = arg2[0] = '\0';
 /* read the line from the assembly-language file */
 if (fgets(line, MAXLINELENGTH, inFilePtr) == NULL) {
 /* reached end of file */
 return(0);
 }
 /* check for line too long (by looking for a \n) */
 if (strchr(line, '\n') == NULL) {
 /* line too long */
 printf("error: line too long\n");
 exit(1);
 }
 /* is there a label? */
 ptr = line;
 if (sscanf(ptr, "%[^\t\n\r ]", label)) {
 /* successfully read label; advance pointer over the label */
 ptr += strlen(label);
 }
 /*
 * Parse the rest of the line. Would be nice to have real regular
 * expressions, but scanf will suffice.
 */
 sscanf(ptr, "%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]",
 opcode, arg0, arg1, arg2);
 return(1);
}

int isNumber(char *string)
{ /* return 1 if string is a number */
 int i;
 return( (sscanf(string, "%d", &i)) == 1);
}

//for rtype
int rtype(char * arg0,char* arg1,char* arg2){
   int regA=0;
   int regB=0;
   int destReg=0;
   
   if(isNumber(arg0) && isNumber(arg1) && isNumber(arg2)){
      regA = (atoi(arg0)<<19);
      regB = (atoi(arg1)<<16);
      destReg = atoi(arg2);
   }else{
      return -1;
   }


return regA | regB | destReg;
}

//for itype
int itype(char * arg0,char* arg1,char* arg2,int line,int bflag){
   int regA=0;
   int regB=0;
   int offsetField=0;
   if(isNumber(arg0) && isNumber(arg1)){
      if(isNumber(arg2)){
         offsetField = atoi(arg2);
      }else{
         if(!strcmp(arg2,""))
         {return -1;}
         else{
            for(int i = 0; i < line;i++){
               if(strcmp(labelList[i],arg2) == 0){
                  offsetField = i;
                  break;
               }
               offsetField = i;
               if(i == line-1) {
                   printf("undefined label error\n");
                   exit(1);
                   }
           }         
         }
      }
      if(bflag == 1 && (!isNumber(arg2))){
      offsetField -= (inst_count +1);
      }

      if (offsetField < -32768 || offsetField > 32767) {
            printf("offsetField don't fit 16bit\n");
            exit(1);
          return -1;
    		}
      if(bflag == 1){
      offsetField &= 0x0000FFFF;
      }

      regA = (atoi(arg0)<<19);
      regB = (atoi(arg1)<<16);


   }else{
       return -1;
   }

   return regA | regB | offsetField;
}

//for jtype
int jtype(char * arg0,char* arg1){
   int regA=0;
   int regB=0;
   if(isNumber(arg0) && isNumber(arg1)){
      regA = (atoi(arg0)<<19);
      regB = (atoi(arg1)<<16);

   }else{
      return -1;
   }
   return regA | regB;
}

int filltype(char * arg0,int line){
   int temp=0;
   if (!strcmp(arg0, "")) {
		printf("line number %d, argument of .fill is wrong",inst_count);
      exit(1);
	}else{
      if(isNumber(arg0)){
         return atoi(arg0);
      }else{
          for(int i = 0; i < line;i++){
               if(strcmp(labelList[i],arg0) == 0){
                  temp = i;
                  break;
               }
               
               if(i == line-1) {
                  printf("line number %d, argument of .fill is wrong",inst_count);
                  exit(1);
               }
           }
          return temp;
      }
   }
}

