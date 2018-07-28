#include<stdio.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include "8080.h"

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
	}
	return 0;
}