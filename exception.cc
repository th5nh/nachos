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

#define MaxFileLength 32 // Do dai quy uoc cho file name

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

// Doi program counter cua he thong ve sau 4 byte de tiep tuc nap lenh
void IncreasePC() 
{
    int counter = machine->ReadRegister(PCReg);
    machine->WriteRegister(PrevPCReg, counter);
    counter = machine->ReadRegister(NextPCReg);
    machine->WriteRegister(PCReg, counter);
    machine->WriteRegister(NextPCReg, counter + 4);
}

/*
Input: Khong gian dia chi User(int) - gioi han cua buffer(int)
Output: Bo nho dem Buffer(char*)
Chuc nang: Sao chep vung nho User sang vung nho System
*/
char* User2System(int virtAddr,int limit)
{
    int i; // chi so index
    int oneChar;
    char* kernelBuf = NULL;

    kernelBuf = new char[limit +1]; // can cho chuoi terminal
    if (kernelBuf == NULL)
	return kernelBuf;
    memset(kernelBuf,0,limit+1);

    for (i = 0 ; i < limit ;i++)
    {
 	machine->ReadMem(virtAddr+i,1,&oneChar);
 	kernelBuf[i] = (char)oneChar;
 	if (oneChar == 0)
 	    break;
    }
    return kernelBuf;
}

/*
Input: Khong gian vung nho User(int) - gioi han cua buffer(int) - bo nho dem buffer(char*)
Output: So byte da sao chep(int)
Chuc nang: Sao chep vung nho System sang vung nho User
*/
int System2User(int virtAddr,int len,char* buffer)
{
    if (len < 0) return -1;
    if (len == 0) return len;
    int i = 0;
    int oneChar = 0 ;
    do{
        oneChar= (int) buffer[i];
 	machine->WriteMem(virtAddr+i,1,oneChar);
 	i ++;
    }while(i < len && oneChar != 0);
    return i;
} 

// Ham xu ly ngoai le runtime exception va system call
void ExceptionHandler(ExceptionType which)
{
    int type = machine->ReadRegister(2);
    switch(which) 
    {
	case NoException:
	    return;

	case PageFaultException:
	    DEBUG('a', "\n No valid translation found.");
	    printf("\n\n No valid translation found.");
	    interrupt->Halt();
	    break;

	case ReadOnlyException:
	    DEBUG('a', "\n Write attempted to page marked read-only.");
	    printf("\n\n Write attempted to page marked read-only.");
	    interrupt->Halt();
	    break;

	case BusErrorException:
	    DEBUG('a', "\n Translation resulted invalid physical address.");
	    printf("\n\n Translation resulted invalid physical address.");
	    interrupt->Halt();
	    break;

	case AddressErrorException:
	    DEBUG('a', "\n Unaligned reference or one that was beyond the end of the address space.");
	    printf("\n\n Unaligned reference or one that was beyond the end of the address space.");
	    interrupt->Halt();
	    break;

	case OverflowException:
	    DEBUG('a', "\nInteger overflow in add or sub.");
	    printf("\n\n Integer overflow in add or sub.");
	    interrupt->Halt();
	    break;

	case IllegalInstrException:
	    DEBUG('a', "\n Unimplemented or reserved instr.");
	    printf("\n\n Unimplemented or reserved instr.");
	    interrupt->Halt();
	    break;

	case NumExceptionTypes:
	    DEBUG('a', "\n Number exception types.");
	    printf("\n\n Number exception types.");
	    interrupt->Halt();
	    break;

        case SyscallException:
            switch (type)
            {
		case SC_Halt:
		    DEBUG('a', "\n Shutdown, initiated by user program.");
		    printf("\n\n Shutdown, initiated by user program.");
		    interrupt->Halt();
		    break; 

                 /*case SC_Exec:
		    // Input: Vi tri int
		    // Output: Fail: return -1, Success: return id cua thread dang chay
		    // SpaceId Exec(char *name);
		    int virtAddr;
		    virtAddr = machine->ReadRegister(4); // Doc dia chi ten chuong trinh tu thanh ghi r4
		    char* name;
		    name = User2System(virtAddr, MaxFileLength + 1); // Lay ten chuong trinh, nap vao kernel
	
		    if(name == NULL)
		    {
			printf("\n Not enough memory in System.");
			machine->WriteRegister(2, -1);
			// IncreasePC();
			break;
		    }
		    OpenFile *oFile = fileSystem->Open(name);
		    if (oFile == NULL)
		    {
			printf("\n Exec:: Can't open this file.");
			machine->WriteRegister(2, -1);
			IncreasePC();
			break;
		    }

		    delete oFile;

		    // Return child process id
		    int id = pTab->ExecUpdate(name); 
		    machine->WriteRegister(2, id);

		    delete[] name;	
		    IncreasePC();
		    break;
		
		case SC_Exit:
		    // void Exit(int status);
		    // Input: Status code
		    int exitStatus = machine->ReadRegister(4);

		    if(exitStatus != 0)
		    {
		        IncreasePC();
			break;
				
		    }			
			
		    int res = pTab->ExitUpdate(exitStatus);
		    // machine->WriteRegister(2, res);

		    currentThread->FreeSpace();
		    currentThread->Finish();
		    IncreasePC();
		    break;*/

		case SC_Sub:
		    int op1 = machine->ReadRegister(4);
		    int op2 = machine->ReadRegister(5);
		    int result = op1 - op2;
		    machine->WriteRegister(2, result);
		    interrupt->Halt();
 		    break;	
            }
	    IncreasePC();
 
	default:
	    printf("\n Unexpected user mode exception (%d %d)", which, type);
	    interrupt->Halt(); 
    }
}