/* Authors:
 * Andrew W. Gillespie
 * Melissa A. Cloud
 * LaJuan A. Cammom-Joy
 * Shou Wang
 */

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
	unsigned short E;

	srand(time(0));

	//initialize the memory
	int temp;
	int idx;
	int idx2;
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
			memory[idx] += rand() % 395;
		}
	}

    //set random register values
    PC = rand() % 400;
    AC = rand() % 0x0FFF;
    DR = rand() % 0x0FFF;
    AR = rand() % 400;
    IR = rand() % 0x0FFF;
	E = rand() % 2;

	/*set for testing


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
	memory[7] = 0x7100;
	memory[8] = 0x5069;
	memory[20] = 0x6067;
	memory[21] = 0x4069;
	memory[9] = 0x1068;
	memory[10] = 0x7400;
	memory[11] = 0x7800;
	memory[12] = 0x7001;
	memory[13] = 0x7010;
	memory[14] = 0x7008;
	memory[15] = 0x7004;
	memory[16] = 0x7002;
	*/

    for(int idx = 0; idx < 3; ++idx)
	{
		PC = rand() % 395;

		for(int idx2 = 0; idx2 < 5; ++idx2)
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
						AC = ~AC;
					break;

					//CME
					case 0x0100:
						E = !E;
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
					break;

					//SPA
					case 0x0010:
						if (AC >> 15 == 0)
						{
							PC += 1;
						}
					break;

					//SNA
					case 0x0008:
						if (AC >> 15 == 1)
						{
							PC += 1;
						}
					break;

					//SZA
					case 0x0004:
						if (AC == 0)
						{
							PC += 1;
						}
					break;

					//SZE
					case 0x0002:
						if (E == 0)
						{
							PC += 1;
						}
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
				AR = (memory[AR]&0x0FFF);
				switch(IR&0x7000)
				{
					//AND
					case 0x0000:
						if((IR&0x8000) == 0) //Direct
						{
						}
						else //Indirect
						{
							AR = (memory[AR]&0xFFF);
							//AR is NOT guaranteed to be < 400
							if(AR > 399) AR %= 400;
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
							AR = (memory[AR]&0xFFF);
							//AR is NOT guaranteed to be < 400
							if(AR > 399) AR %= 400;
						}
						print(3, AC, PC, DR, AR, IR, E, memory);

						DR = memory[AR];
						print(4, AC, PC, DR, AR, IR, E, memory);

						AC += DR;
						print(5, AC, PC, DR, AR, IR, E, memory);
					break;

					//LDA
					case 0x2000:
						if((IR&0x8000) == 0) //Direct
						{
						}
						else //Indirect
						{
							AR = (memory[AR]&0xFFF);
							//AR is NOT guaranteed to be < 400
							if(AR > 399)
							{
								AR %= 400;
							}
						}
						print(3, AC, PC, DR, AR, IR, E, memory);

						DR = memory[AR];
						print(4, AC, PC, DR, AR, IR, E, memory);

						AC = DR;
						print(5, AC, PC, DR, AR, IR, E, memory);
					break;

					//STA
					case 0x3000:
						if((IR&0x8000) == 0) //Direct
						{
						}
						else //Indirect
						{
							AR = (memory[AR]&0xFFF);
							//AR is NOT guaranteed to be < 400
							if(AR > 399)
							{
								AR %= 400;
							}
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
							AR = (memory[AR]&0xFFF);
							//AR is NOT guaranteed to be < 400
							if(AR > 399)
							{
								AR %= 400;
							}
						}
						print(3, AC, PC, DR, AR, IR, E, memory);
						PC = (memory[AR] & 0x0FFF);
						print(4, AC, PC, DR, AR, IR, E, memory);
					break;

					//BSA
					case 0x5000:
						if((IR&0x8000) == 0) //Direct
						{
						}
						else //Indirect
						{
							AR = (memory[AR]&0xFFF);
							//AR is NOT guaranteed to be < 400
							if(AR > 399)
							{
								AR %= 400;
							}
						}
						print(3, AC, PC, DR, AR, IR, E, memory);
						memory[AR] = PC;
						AR++;
						print(4, AC, PC, DR, AR, IR, E, memory);
						PC = AR;
						print(5, AC, PC, DR, AR, IR, E, memory);
					break;

					//ISZ
					case 0x6000:
						if((IR&0x8000) == 0) //Direct
						{
						}
						else //Indirect
						{
							AR = (memory[AR]&0xFFF);
							//AR is NOT guaranteed to be < 400
							if(AR > 399)
							{
								AR %= 400;
							}
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
