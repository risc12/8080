#include<stdio.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include "8080.h"

// Hello

#include "Disassemble8080Op.c"
#include "Emulate8080Op.c"

void ReadFileIntoMemoryAt(State8080* state, char* filename, uint32_t offset)
{
	FILE *file= fopen(filename, "rb");
	if (file==NULL)
	{
		printf("error: Couldn't open %s\n", filename);
		exit(1); 
	}
	fseek(file, 0L, SEEK_END);
	int filesize = ftell(file);
	fseek(file, 0L, SEEK_SET);
	
	uint8_t *buffer = &state->memory[offset];
	fread(buffer, filesize, 1, file);
	fclose(file);
}

static void WriteMem(State8080* state, uint16_t address, uint8_t value)
{
    if (address < 0x2000)
    {
        //        printf("Writing ROM not allowed %x\n", address);
        return;
    }
    if (address >=0x4000)
    {
        //       printf("Writing out of Space Invaders RAM not allowed %x\n", address);
        return;
    }
    
    state->memory[address] = value;
}

static void Push(State8080* state, uint8_t high, uint8_t low)
{
    WriteMem(state, state->sp-1, high);
    WriteMem(state, state->sp-2, low);
    state->sp = state->sp - 2;    
    //    printf ("%04x %04x\n", state->pc, state->sp);
}

void GenerateInterrupt(State8080* state, int interrupt_num)
{
	//perform "PUSH PC"
    Push(state, (state->pc & 0xFF00) >> 8, (state->pc & 0xff));
	
	//Set the PC to the low memory vector
	state->pc = 8 * interrupt_num;
    
    //"DI"
    state->int_enable = 0;
}

State8080* Init8080(void)
{
	State8080* state = calloc(1, sizeof(State8080));
	state->memory = malloc(0x10000);  //16K
	return state;
}

int main (int argc, char**argv)
{
	int done = 0;
	int vblankcycles = 0;
	State8080* state = Init8080();
	
	ReadFileIntoMemoryAt(state, "invaders/invaders.h", 0);
	ReadFileIntoMemoryAt(state, "invaders/invaders.g", 0x800);
	ReadFileIntoMemoryAt(state, "invaders/invaders.f", 0x1000);
	ReadFileIntoMemoryAt(state, "invaders/invaders.e", 0x1800);
	
	while (done == 0)
	{
		done = Emulate8080Op(state);
		// getchar();
	}
	return 0;
}
// 1a3a c9 RET
// --> z.p..  A $00 B $00 C $00 D $1c E $00 H $21 L $00 SP 2400