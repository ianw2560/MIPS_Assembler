#include "spimcore.h"

#define MAX_VAL_6_BITS 63
#define MAX_VAL_16_BITS 65535
#define MAX_VAL_26_BITS 67108863

/* ALU */
/* 10 Points */
void ALU(unsigned A, unsigned B, char ALUControl, unsigned *ALUresult, char *Zero)
{
	switch(ALUControl)
  	{
  		// Add A and B
		case 0x000 :
		{
			*ALUresult = A + B;
			*Zero = (*ALUresult == 0) ? 1 : 0;
			break;
		}

		// Subtract A and B
		case 0x001 :
		{
			*ALUresult = A - B;
			*Zero = (*ALUresult == 0) ? 1 : 0;
			break;
		}

		// Set Z to 1 if A < B (signed)
		case 0x002 :
		{
			*ALUresult = ((int) A < (int) B) ? 1 : 0;
			*Zero = (*ALUresult) ? 0 : 1;
			break;
		}

		// Set Z to 1 if A < B (unsigned)
		case 0x003 :
		{
			*ALUresult = (A < B) ? 1 : 0;
			*Zero = (*ALUresult == 0) ? 1 : 0;
			break;
		}

		// A AND B
		case 0x004 :
		{
			*ALUresult = (A & B);
			*Zero = (*ALUresult == 0) ? 1 : 0;
			break;
		}

		// A OR B
		case 0x005 :
		{
			*ALUresult = (A | B);
			*Zero = (*ALUresult == 0) ? 1 : 0;
			break;
		}

		// B shifted left by 16
		case 0x006 :
		{
			*ALUresult = B << 16;
			*Zero = (*ALUresult == 0) ? 1 : 0;
			break;
		}

		// Set Z to NOT A
		case 0x007 :
		{
			*ALUresult = ~A;
			*Zero = (*ALUresult == 0) ? 1 : 0;
			break;
		}
	}
}

/* instruction fetch */
/* 10 Points */
int instruction_fetch(unsigned PC, unsigned *Mem, unsigned *instruction)
{
	// Checks to see if the PC is word aligned, e.g. divisible by 4
	if (PC % 4 == 0) 
	{
		// The instruction file says that you just need to do PC >> 2 
		// whenever you go into an address
		*instruction = Mem[(PC >> 2)];
		return 0;
	}
	else
	{
		return 1;
	}
}


/* instruction partition */
/* 10 Points */
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1, 
                            unsigned *r2, unsigned *r3, unsigned *funct, 
                            unsigned *offset, unsigned *jsec)
{
	// Returns first 6 bits
	*op = instruction >> 26;

	// 31 represents 11111 in binary, 
	// isolates the last 5 bits after shifting 21
	*r1 = instruction >> 21 & 31;

	// Returns the 5 bits after r1
	*r2 = instruction >> 16 & 31;

	// Returns the 5 bits after r2
	*r3 = instruction >> 11 & 31;

	// Since funct is the last 6 bits,
	// use & with 63 which is 111111 in binary.
	*funct = instruction & MAX_VAL_6_BITS;

	// Since offset is the last 16 bits,
	// use & with 2^16 - 1 to isolate last 16 bits.
	*offset = instruction & MAX_VAL_16_BITS;

	// Since jsec is the last 26 bits,
	// use & with 2^26 - 1 to isolate last 26 bits of instruction.
	*jsec = instruction & MAX_VAL_26_BITS;
}

/* instruction decode */
/* 15 Points */
int instruction_decode(unsigned op, struct_controls *controls)
{
	// The op decides what the other control signals are.
	// The below functions that use the control signals 
	// will refer to these values

	// R-type instruction
	if (op == 0) // 0 -> 000000
	{
		controls->RegDst = 1;
		controls->Jump = 0;
		controls->Branch = 0;
		controls->MemRead = 0;
		controls->MemtoReg = 0;
		controls->ALUOp = 7;
		controls->MemWrite = 0;
		controls->ALUSrc = 0;
		controls->RegWrite = 1;
		return 0;
	}

	// Add immediate
	if (op == 8) // 8 -> 001000
	{
		controls->RegDst = 0;
		controls->Jump = 0;
		controls->Branch = 0;
		controls->MemRead = 0;
		controls->MemtoReg = 0;
		controls->ALUOp = 0;
		controls->MemWrite = 0;
		controls->ALUSrc = 1;
		controls->RegWrite = 1;
		return 0;
	}
	// Load word 
	if (op == 35) // 35 -> 100011
	{
		controls->RegDst = 0;
		controls->Jump = 0;
		controls->Branch = 0;
		controls->MemRead = 1;
		controls->MemtoReg = 1;
		controls->ALUOp = 0;
		controls->MemWrite = 0;
		controls->ALUSrc = 1;
		controls->RegWrite = 1;
		return 0;
	}

	// Save word
	if (op == 43) // 43 -> 101011
	{
		controls->RegDst = 0;
		controls->Jump = 0;
		controls->Branch = 0;
		controls->MemRead = 0;
		controls->MemtoReg = 0;
		controls->ALUOp = 0;
		controls->MemWrite = 1;
		controls->ALUSrc = 1;
		controls->RegWrite = 0;
		return 0;
	}

	// Load upper immediate 
	if (op == 15) // 15 -> 001111
	{
		controls->RegDst = 0;
		controls->Jump = 0;
		controls->Branch = 0;
		controls->MemRead = 0;
		controls->MemtoReg = 0;
		controls->ALUOp = 6;
		controls->MemWrite = 0;
		controls->ALUSrc = 1;
		controls->RegWrite = 1;
		return 0;
	}

	// Branch on equal
	if (op == 4) // 4 -> 000100
	{
		controls->RegDst = 2;
		controls->Jump = 0;
		controls->Branch = 1;
		controls->MemRead = 0;
		controls->MemtoReg = 0;
		controls->ALUOp = 1;
		controls->MemWrite = 0;
		controls->ALUSrc = 0;
		controls->RegWrite = 0;
		return 0;
	}	
	// Set less than immediate
	if (op == 10) // 10 -> 001010
	{
		controls->RegDst = 0;
		controls->Jump = 0;
		controls->Branch = 0;
		controls->MemRead = 0;
		controls->MemtoReg = 0;
		controls->ALUOp = 2;
		controls->MemWrite = 0;
		controls->ALUSrc = 1;
		controls->RegWrite = 1;
		return 0;
	}

	// Set less than immediate unsigned
	if (op == 11) // 11 -> 001011
	{
		controls->RegDst = 0;
		controls->Jump = 0;
		controls->Branch = 0;
		controls->MemRead = 0;
		controls->MemtoReg = 0;
		controls->ALUOp = 3;
		controls->MemWrite = 0;
		controls->ALUSrc = 1;
		controls->RegWrite = 1;
		return 0;
	}

	// Jump
	if (op == 2) // 2 -> 000010
	{
		controls->RegDst = 2;
		controls->Jump = 1;
		controls->Branch = 0;
		controls->MemRead = 0;
		controls->MemtoReg = 0;
		controls->ALUOp = 0;
		controls->MemWrite = 0;
		controls->ALUSrc = 2;
		controls->RegWrite = 0;
		return 0;
	}

	// Returns halt if no valid instructions are found.
	return 1;
}

/* Read Register */
/* 5 Points */
void read_register(unsigned r1,unsigned r2,unsigned *Reg, unsigned *data1, 
					unsigned *data2)
{
	*data1 = Reg[r1];
	*data2 = Reg[r2];
}


/* Sign Extend */
/* 10 Points */
void sign_extend(unsigned offset, unsigned *extended_value)
{

	int offsetcopy = offset;
	
	// By shifting left and right 16 bits because offsetcopy is an int the left 
	// most bit will fill in the space between bit 32 and bit 16
	offsetcopy = offsetcopy << 16;
	offsetcopy = offsetcopy >> 16;

	*extended_value = offsetcopy;
}

/* ALU operations */
/* 10 Points */
int ALU_operations(unsigned data1,unsigned data2,unsigned extended_value,unsigned funct,char ALUOp,char ALUSrc,unsigned *ALUresult,char *Zero)
{
	// rtype instruction
	if (ALUOp == 7)
	{
		// Add
		if (funct == 32) // 32 -> 100000
		{
			ALU(data1, data2, 0, ALUresult, Zero);
			return 0;
		}
		// Subtract
		if (funct == 34) // 34 -> 100010
		{
			ALU(data1, data2, 1, ALUresult, Zero);
			return 0;
		}
		// And
		if (funct == 36) // 36 -> 100100
		{
			ALU(data1, data2, 4, ALUresult, Zero);
			return 0;
		}
		// Or
		if (funct == 37) // 37 -> 100101
		{
			ALU(data1, data2, 5, ALUresult, Zero);
			return 0;
		}
		// Set less than
		if (funct == 42) // 42 -> 101010
		{
			ALU(data1, data2, 2, ALUresult, Zero);
			return 0;
		}
		// Set less than unsigned
		if (funct == 43) // 43 -> 101011
		{
			ALU(data1, data2, 3, ALUresult, Zero);
			return 0;
		}
	}

	// ALUOp addition
	if (ALUOp == 0)
	{
		if (ALUSrc == 0)
		{
			ALU(data1, data2, 0, ALUresult, Zero);
			return 0;
		}
		else if (ALUSrc == 1)
		{
			ALU(data1, extended_value, 0, ALUresult, Zero);
			return 0;
		}
		else if (ALUSrc == 2)
		{
			return 0;
		}
	}

	// ALUOp subtraction
	if (ALUOp == 1)
	{
		if (ALUSrc == 0)
		{
			ALU(data1, data2, 1, ALUresult, Zero);
			return 0;
		}
		else if (ALUSrc == 1)
		{
			
			ALU(data1, extended_value, 1, ALUresult, Zero);
			return 0;
		}
	}

	// Set less than
	if (ALUOp == 2)
	{
		if (ALUSrc == 0)
		{
			ALU(data1, data2, 2, ALUresult, Zero);
			return 0;
		}
		else if (ALUSrc == 1)
		{
			ALU(data1, extended_value, 2, ALUresult, Zero);
			return 0;
		}
	}

	// Set less than unsigned
	if (ALUOp == 3)
	{
		if (ALUSrc == 0)
		{
			ALU(data1, data2, 3, ALUresult, Zero);
			return 0;
		}
		else if (ALUSrc == 1)
		{
			ALU(data1, extended_value, 3, ALUresult, Zero);
			return 0;
		}
	}

	// And
	if (ALUOp == 4)
	{
		if (ALUSrc == 0)
		{
			ALU(data1, data2, 4, ALUresult, Zero);
			return 0;
		}
		else if (ALUSrc == 1)
		{
			ALU(data1, extended_value, 4, ALUresult, Zero);
			return 0;
		}
	}

	// Or
	if (ALUOp == 5)
	{
		if (ALUSrc == 0)
		{
			ALU(data1, data2, 5, ALUresult, Zero);
			return 0;
		}
		else if (ALUSrc == 1)
		{
			ALU(data1, extended_value, 5, ALUresult, Zero);
			return 0;
		}
	}

	// Shift left 16 bits
	if (ALUOp == 6)
	{
		if (ALUSrc == 0)
		{
			ALU(data1, data2, 6, ALUresult, Zero);
			return 0;
		}
		else if (ALUSrc == 1)
		{
			ALU(data1, extended_value, 6, ALUresult, Zero);
			return 0;
		}
	}
	
	return 1;
}

/* Read / Write Memory */
/* 10 Points */
int rw_memory(unsigned ALUresult, unsigned data2, char MemWrite, char MemRead,
				unsigned *memdata, unsigned *Mem)
{
	if (MemRead)
	{
		if (ALUresult % 4 != 0)
			return 1;
		else
			*memdata = Mem[ALUresult >> 2];
	}

	if (MemWrite)
	{
		if (ALUresult % 4 != 0)
			return 1;
		else
			Mem[ALUresult >> 2] = data2;
	}

	return 0;
}


/* Write Register */
/* 10 Points */
void write_register(unsigned r2, unsigned r3, unsigned memdata, 
					unsigned ALUresult, char RegWrite, char RegDst, 
					char MemtoReg, unsigned *Reg)
{
	if (RegWrite)
	{
		if (!MemtoReg)
		{
			if (RegDst)
			{
				Reg[r3] = ALUresult;
			}
			else 
			{
				Reg[r2] = ALUresult;
			}
		}
		
		if (MemtoReg)
		{
			Reg[r2] = memdata;
		}
	}
}

/* PC update */
/* 10 Points */
void PC_update(unsigned jsec, unsigned extended_value, char Branch,
				char Jump, char Zero, unsigned *PC)
{
	*PC += 4;
	
	if (Zero && Branch && !Jump)
	{
		*PC += (extended_value << 2);
	}
	else if (Jump)
	{
		*PC = (*PC & 0xf0000000) | (jsec << 2);
	}
}
