#include <time.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
	unsigned short memory[400];
	unsigned short AC;
	unsigned short PC;
	unsigned short DR;
	unsigned short AR;
	unsigned short IR;
	unsigned short TR;
	unsigned short E;

	srand(time(0));

	//initialize the memory
	int temp;
	int idx;
	for(idx = 0; idx < 400; ++idx)
	{
		temp = rand() % 0xE;
		if(temp == 7)
		{
			memory[idx] = (temp << 12);
			temp = rand() % 12;
			memory[idx] += (1 << temp);
		}
		else
		{
			memory[idx] = temp << 12;
			memory[idx] += rand() % 400;
		}
	}

    //set random register values
    PC = rand() % 400;
    AC = rand() % 0x0FFF;
    PC = rand() % 0x0FFF;
    DR = rand() % 0x0FFF;
    AR = rand() % 0x0FFF;
    IR = rand() % 0x0FFF;
    TR = rand() % 0x0FFF;

	//set for testing
	PC = 100;
	memory[100] = 0x7080;
	memory[101] = 0x7040;

    for(int idx = 0; idx < 2; ++idx)
	{
	//T0
    AR = PC;

	printf("   |  IR  |  AC  |  DR  |  PC  |  AR  | M[AR]|   E  |\n");
	printf("---|------+------+------+------+------+------+------|\n");
	printf("T0 | %04hX | %04hX | %04hX | %04hX | %04hX | %04hX | %04hX |\n",
			IR, AC, DR, PC, AR, memory[AR], E);
	printf("---|------+------+------+------+------+------+------|\n");

    //T1
    IR = memory[AR];
    ++PC;
	printf("T1 | %04hX | %04hX | %04hX | %04hX | %04hX | %04hX | %04hX |\n",
			IR, AC, DR, PC, AR, memory[AR], E);
	printf("---|------+------+------+------+------+------+------|\n");

    //T2
	if((0x7000&IR) == 0x7000)
	{
		switch(0x0FFF&IR)
		{
			//CLA
			case 0x0800:
				//code
			break;

			//CLE
			case 0x0400:
				//code
			break;
			
			//CMS
			case 0x0200:
				//code
			break;
			
			//CME
			case 0x0100:
				//code
			break;
			
			//CIR
			case 0x0080:
				E = (0x0001&AC) << 15;
				//C >> operator can be logical or
				//arithmetic (implementation dependent)
				AC >>= 1;
				AC &= 0x7FFF;
				AC |= E;
			break;
			
			//CIL
			case 0x0040:
				E = (0x8000&AC) >> 15;
				AC <<= 1;
				AC |= E;
			break;
			
			//INC
			case 0x0020:
				//code
			break;
			
			//SPA
			case 0x0010:
				//code
			break;
			
			//SNA
			case 0x0008:
				//code
			break;
			
			//SZA
			case 0x0004:
				//code
			break;

			//SZE
			case 0x0002:
				//code
			break;
			
			//HLT
			case 0x0001:
				exit(2);
			break;
		}
	}
	else
	{
		switch(IR&0x7000)
		{
			//AND
			case 0x0000:
				//code
			break;

			//ADD
			case 0x1000:
				//code
			break;

			//LDA
			case 0x2000:
				//code
			break;

			//STA
			case 0x3000:
				//code
			break;

			//BUN
			case 0x4000:
				//code
			break;

			//BSA
			case 0x5000:
				//code
			break;

			//ISZ
			case 0x6000:
				//code
			break;
		}
	}
	printf("T2 | %04hX | %04hX | %04hX | %04hX | %04hX | %04hX | %04hX |\n",
			IR, AC, DR, PC, AR, memory[AR], E);
	printf("   |------+------+------+------+------+------+------|\n");
	printf("\n");
	}

	return 0;
}
