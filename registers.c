#include <time.h>
#include <stdio.h>
#include <stdlib.h>

void print(int tier, unsigned short AC, unsigned short PC, unsigned short DR,
		             unsigned short AR, unsigned short IR, unsigned short E,
					 unsigned short memory[]
		  );

int main(int argc, char** argv)
{
	unsigned short memory[400];
	unsigned short AC;
	unsigned short PC;
	unsigned short DR;
	unsigned short AR;
	unsigned short IR;
	//unsigned short TR;
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
    //PC = rand() % 0x0FFF;
    DR = rand() % 0x0FFF;
    AR = rand() % 400;
    IR = rand() % 0x0FFF;
    //TR = rand() % 0x0FFF;
	E = rand() % 2;

	//set for testinga


	memory[0x0064] = 0x0035;
	memory[0x0065] = 0x00A4;
	memory[0x0066] = 0x0040;
	memory[0x0067] = 0x0025;
	memory[0x0068] = 0x0020;
	memory[0x0069] = 19;

	PC = 0;
	memory[0] = 0x7200;
	memory[1] = 0x7080;
	memory[2] = 0x7040;
	memory[3] = 0x7020;
	memory[4] = 0x0064;
	memory[5] = 0x2065;
	memory[6] = 0x3066;
	memory[7] = 0x5069;
	memory[20] = 0x6067;
	memory[21] = 0x4069;
	memory[8] = 0x1068;
	memory[9] = 0x7400;
	memory[10] = 0x7800;
	memory[11] = 0x7001;

    for(int idx = 0; idx < 14; ++idx)
	{
		print(-1, AC, PC, DR, AR, IR, E, memory);
		//T0
		AR = PC;
		print(0, AC, PC, DR, AR, IR, E, memory);

		//T1
		IR = memory[AR];
		++PC;
		print(1, AC, PC, DR, AR, IR, E, memory);
		
		//T2
		AR = (0x0FFF&IR);
		print(2, AC, PC, DR, AR, IR, E, memory);
		if((0x7000&IR) == 0x7000)
		{
			switch(0x0FFF&IR)
			{
				//CLA
				case 0x0800:
					AC ^= AC; //AC xor AC is 0
				break;

				//CLE
				case 0x0400:
					E ^= E; //E xor E is 0
				break;

				//CMS
				case 0x0200:
					AC = (0x0FFF&(~AC));
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
					AC++;
					//print();
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
			print(3, AC, PC, DR, AR, IR, E, memory);
		}
		else
		{
			AR = memory[AR];
			switch(IR&0x7000)
			{
				//AND
				case 0x0000:
					if((IR&0x8000) == 0) //Direct
					{
					}
					else //Indirect
					{
						AR = memory[AR];
					}
					print(3, AC, PC, DR, AR, IR, E, memory);

					DR = memory[AR];
					print(4, AC, PC, DR, AR, IR, E, memory);

					AC &= DR;
					print(5, AC, PC, DR, AR, IR, E, memory);
				break;

				//ADD
				case 0x1000:
					if((IR&0x8000) == 0) //Direct
					{
					}
					else //Indirect
					{
						AR = memory[AR];
					}
					print(3, AC, PC, DR, AR, IR, E, memory);

					DR = memory[AR];
					print(4, AC, PC, DR, AR, IR, E, memory);

					AC &= DR;
					print(5, AC, PC, DR, AR, IR, E, memory);
				break;

				//LDA
				case 0x2000:
					if((IR&0x8000) == 0) //Direct
					{
					}
					else //Indirect
					{
						AR = memory[AR];
					}
					print(3, AC, PC, DR, AR, IR, E, memory);

					DR = memory[AR];
					print(4, AC, PC, DR, AR, IR, E, memory);

					AC = (DR&0x0FFF);
					print(5, AC, PC, DR, AR, IR, E, memory);
				break;

				//STA
				case 0x3000:
					if((IR&0x8000) == 0) //Direct
					{
					}
					else //Indirect
					{
						AR = memory[AR];
					}
					print(3, AC, PC, DR, AR, IR, E, memory);
					memory[AR] = AC;
					print(4, AC, PC, DR, AR, IR, E, memory);
				break;

				//BUN
				case 0x4000:
					if((IR&0x8000) == 0) //Direct
					{
					}
					else //Indirect
					{
						AR = memory[AR];
					}
					print(3, AC, PC, DR, AR, IR, E, memory);
					PC = memory[AR];
					print(4, AC, PC, DR, AR, IR, E, memory);
				break;

				//BSA
				case 0x5000:
					if((IR&0x8000) == 0) //Direct
					{
					}
					else //Indirect
					{
						AR = memory[AR];
					}
					print(3, AC, PC, DR, AR, IR, E, memory);
					memory[AR] = PC;
					AR++;
					PC = AR;
				break;

				//ISZ
				case 0x6000:
					if((IR&0x8000) == 0) //Direct
					{
					}
					else //Indirect
					{
						AR = memory[AR];
					}
					print(3, AC, PC, DR, AR, IR, E, memory);
					DR = memory[AR];
					print(4, AC, PC, DR, AR, IR, E, memory);

					DR++;
					print(5, AC, PC, DR, AR, IR, E, memory);

					memory[AR] = DR;

					if (DR == 0)
					{
						PC++;
					}
					print(6, AC, PC, DR, AR, IR, E, memory);
				break;
			}
		}
		printf("\n");
	}

	return 0;
}

void print(int tier, unsigned short AC, unsigned short PC, unsigned short DR,
		             unsigned short AR, unsigned short IR, unsigned short E,
					 unsigned short memory[]
		  )
{
	if(tier == -1)
	{
		printf("    |  IR  |  AC  |  DR  |  PC  |  AR  | M[AR]|   E  |\n");
		printf("----|------+------+------+------+------+------+------|\n");
	}
	if(tier == -1)
	{
		printf("init| %04hX | %04hX | %04hX | %04hX | %04hX | %04hX | %04hX |\n",
			IR, AC, DR, PC, AR, memory[AR], E);
	}
	else
	{
	printf(" T%i | %04hX | %04hX | %04hX | %04hX | %04hX | %04hX | %04hX |\n",
			tier, IR, AC, DR, PC, AR, memory[AR], E);
	}
	printf("----|------+------+------+------+------+------+------|\n");
}
