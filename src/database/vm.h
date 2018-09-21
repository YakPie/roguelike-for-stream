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

// Does the VM needs a stack?
// Do the VM need recursion? Datalog?

enum BYTECODE_OPCODE {
	OPCODE_UNKNOWN	
};

void interpret_vm_bytecode(char* bytecode, size_t length);

#endif
