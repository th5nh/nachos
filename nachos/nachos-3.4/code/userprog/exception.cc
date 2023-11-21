// exception.cc 
//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.  
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "syscall.h"

//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	For system calls, the following is the calling convention:
//
// 	system call code -- r2
//		arg1 -- r4
//		arg2 -- r5
//		arg3 -- r6
//		arg4 -- r7
//
//	The result of the system call, if any, must be put back into r2. 
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//	"which" is the kind of exception.  The list of possible exceptions 
//	are in machine.h.
//----------------------------------------------------------------------



// Input: - User space address (int)
// - Limit of buffer (int)
// Output:- Buffer (char*)
// Purpose: Copy buffer from User memory space to System memory space
char* User2System(int virtAddr,int limit)
{
	int i;// index
	int oneChar;
	char* kernelBuf = NULL;
	kernelBuf = new char[limit +1];//need for terminal string
	if (kernelBuf == NULL)
		return kernelBuf;
	memset(kernelBuf,0,limit+1);
	//printf("\n Filename u2s:");
	for (i = 0 ; i < limit ;i++)
	{
		machine->ReadMem(virtAddr+i,1,&oneChar);
		kernelBuf[i] = (char)oneChar;
		//printf("%c",kernelBuf[i]);
		if (oneChar == 0)
			break;
	}
	return kernelBuf;
}

// Input: - User space address (int)
// - Limit of buffer (int)
// - Buffer (char[])
// Output:- Number of bytes copied (int)
// Purpose: Copy buffer from System memory space to User memory space
int System2User(int virtAddr,int len,char* buffer)
{
	if (len < 0) return -1;
	if (len == 0)return len;
	int i = 0;
	int oneChar = 0 ;
	do{
		oneChar= (int) buffer[i];
		machine->WriteMem(virtAddr+i,1,oneChar);
		i ++;
	}while(i < len && oneChar != 0);
	return i;
}

// Input: None
// Output: None
// Purpose: Increase program counter
void increasePC()
{
	machine->WriteRegister(PrevPCReg, machine->ReadRegister(PCReg));
	machine->WriteRegister(PCReg, machine->ReadRegister(NextPCReg));
	machine->WriteRegister(NextPCReg, machine->ReadRegister(NextPCReg)+4);
}

void
ExceptionHandler(ExceptionType which)
{
	int type = machine->ReadRegister(2);

	switch(which) {
	case NoException:
		return;
	case PageFaultException:
		DEBUG('a',"\nNo valid translation found\n");
		printf("\nNo valid translation found\n");
		interrupt->Halt();
		break;
	case ReadOnlyException:
		DEBUG('a', "\nWrite attempted to page marked read-only\n");
		printf("\nWrite attempted to page marked read-only\n");
		interrupt->Halt();
		break;
	case BusErrorException:
		DEBUG('a', "\nTranslation resulted invalid physical address\n");
		printf("\nTranslation resulted invalid physical address\n");
		interrupt->Halt();
		break;
	case AddressErrorException:
		DEBUG('a', "\nUnaligned reference or one that was beyond the end of the address space\n");
		printf("\nUnaligned reference or one that was beyond the end of the address space\n");
		interrupt->Halt();
		break;
	case OverflowException:
		DEBUG('a',"\nInteger overflow\n");
		printf("\nInteger overflow\n");
		interrupt->Halt();
		break;
	case IllegalInstrException:
		DEBUG('a', "\nUnimplemented or reserved instr\n");
		printf("\nUnimplemented or reserved instr\n");
		interrupt->Halt();
		break;
	case NumExceptionTypes:
		DEBUG('a', "\nNumber exception types\n");
		printf("\nNumber exception types\n");
		interrupt->Halt();
		break;
	case SyscallException:
		switch(type) {
		case SC_Halt:
			DEBUG('a', "\nShutdown, initiated by user program.\n");
			printf("\nShutdown, initiated by user program\n");
			interrupt->Halt();
			break;
		case SC_Exit:
			break;
		case SC_Exec:
			break;
		case SC_Join:
			break;
		case SC_Create:
			break;
		case SC_Open:
			break;
		case SC_Read:
			break;
		case SC_Write:
			break;
		case SC_Close:
			break;
		case SC_Fork:
			break;
		case SC_Yield:
			break;
		case SC_Sub:
		{
			printf("\nHello Sub\n");
			int op1 = machine->ReadRegister(4);
			int op2 = machine->ReadRegister(5);
			int result = op1 - op2;
			machine->WriteRegister(2, result);
			interrupt->Halt();
			break;
		}
		default:
			printf("Unexpected user mode exception %d %d\n", which, type);
	               	ASSERT(FALSE);
                	break;
		}
		increasePC();
	default:
		printf("Unexpected user mode exception %d\n", which);
		ASSERT(FALSE);
		break;
	}





//    if ((which == SyscallException) && (type == SC_Halt)) {
//	DEBUG('a', "Shutdown, initiated by user program.\n");
//   	interrupt->Halt();
//    } else {
//	printf("Unexpected user mode exception %d %d\n", which, type);
//	ASSERT(FALSE);
//    }
}
