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
		temp = rand % 0xE;
		if(temp == 7)
		{
			memory[idx] = (temp << 12);
			temp = rand % 12;
			memory[idx] += (1 << temp)
		}
		else
		{
			memory[idx] = temp << 12;
			memory[idx] += rand % 400;
		}
	}

    //set random register values
    PC = rand() % 400;
    AC = rand() % 0x
    PC
    DR
    AR
    IR
    TR

    //T0
    AR = PC;

    //T1
    IR = memory[PC];
    ++PC;

    //T2


	return 0;
}
