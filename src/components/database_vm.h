#ifndef DATABASE_VM_H
#define DATABASE_VM_H

#include <stdlib.h>
#include <stdio.h>

// INSERT = prototype
// QUERY = prototype
// SUBSCRIBE TO A QUERY
// UPDATE = not implemented
// DELETE = not implemented
// CREATE TABLE = prototype

enum BYTECODE_OPCODE {
	OPCODE_UNKNOWN	
};

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

#endif
