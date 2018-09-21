#include "vm.h"
#include <stdlib.h>
#include <stdio.h>

void interpret_vm_bytecode(char* bytecode, size_t length)
{
	char *it = bytecode;
	while(it < bytecode+length)
	{
		switch((enum BYTECODE_OPCODE)*it)
		{
			default:
				fprintf(stderr, "Bytecode not recognized\n");
				exit(1);
		}
		it++;
	}
}
