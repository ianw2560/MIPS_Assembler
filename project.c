#include "spimcore.h"


/* ALU */
/* 10 Points */
void ALU(unsigned A, unsigned B, char ALUControl, unsigned *ALUresult, char *Zero)
{
	switch(ALUControl)
  	{
		case 0x000 :
		{
			printf("CASE 0\n");
			*ALUresult = A + B;
			printf("%d + %d = %d", A, B, *ALUresult);
			break;
		}
		case 0x001 :
		{
			printf("CASE 1\n");
			*ALUresult = A - B;
			printf("%d - %d = %d", A, B, *ALUresult);
			break;
		}
		case 0x002 :
		{
			printf("CASE 2\n");
			*ALUresult = (A < B) ? 1 : 0;
			if (*ALUresult == 1)
				printf("%d < %d\n", A, B);
			else
				printf("%d >= %d\n", B, A);
			break;
		}
		case 0x003 :
		{
			printf("CASE 3\n");
			*ALUresult = (A < B) ? 1 : 0;
			if (*ALUresult == 1)
				printf("%d < %d\n", A, B);
			else
				printf("%d >= %d\n", B, A);
			break;
		}
		case 0x004 :
		{
			printf("CASE 4\n");
			*ALUresult = (A & B);
			printf("%d AND %d = %d", A, B, *ALUresult);
			break;
		}
		case 0x005 :
		{
			printf("CASE 5\n");
			break;
		}
		case 0x006 :
		{
			printf("CASE 6\n");
			break;
		}
		case 0x007 :
		{
			printf("CASE 7\n");
			break;
		}
	}

}

/* instruction fetch */
/* 10 Points */
int instruction_fetch(unsigned PC, unsigned *Mem, unsigned *instruction)
{

}


/* instruction partition */
/* 10 Points */
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1, 
                            unsigned *r2, unsigned *r3, unsigned *funct, 
                            unsigned *offset, unsigned *jsec)
{

}

/* instruction decode */
/* 15 Points */
int instruction_decode(unsigned op,struct_controls *controls)
{

}

/* Read Register */
/* 5 Points */
void read_register(unsigned r1,unsigned r2,unsigned *Reg,unsigned *data1,unsigned *data2)
{

}


/* Sign Extend */
/* 10 Points */
void sign_extend(unsigned offset,unsigned *extended_value)
{

}

/* ALU operations */
/* 10 Points */
int ALU_operations(unsigned data1,unsigned data2,unsigned extended_value,unsigned funct,char ALUOp,char ALUSrc,unsigned *ALUresult,char *Zero)
{

}

/* Read / Write Memory */
/* 10 Points */
int rw_memory(unsigned ALUresult,unsigned data2,char MemWrite,char MemRead,unsigned *memdata,unsigned *Mem)
{

}


/* Write Register */
/* 10 Points */
void write_register(unsigned r2,unsigned r3,unsigned memdata,unsigned ALUresult,char RegWrite,char RegDst,char MemtoReg,unsigned *Reg)
{

}

/* PC update */
/* 10 Points */
void PC_update(unsigned jsec,unsigned extended_value,char Branch,char Jump,char Zero,unsigned *PC)
{

}

int main(void)
{
	unsigned num = 3;
	unsigned *f = &num;
	char *zero = 0;
	ALU(1, 2, 4, f, zero);
}