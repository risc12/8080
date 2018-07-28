void UnimplementedInstruction(State8080* state)
{
	//pc will have advanced one, so undo that
	printf ("Error: Unimplemented instruction\n");
	state->pc--;
	Disassemble8080Op(state->memory, state->pc);
	printf("\n");
	exit(1);
}