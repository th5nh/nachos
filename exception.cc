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


#define MaxFileLength 32

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


/* Util functions */
void IncreasePC()
{
	int counter = machine->ReadRegister(PCReg);
   	machine->WriteRegister(PrevPCReg, counter);
    	counter = machine->ReadRegister(NextPCReg);
    	machine->WriteRegister(PCReg, counter);
   	machine->WriteRegister(NextPCReg, counter + 4);
}

char* User2System(int virtAddr, int limit)
{
	int i; //chi so index
	int oneChar;
	char* kernelBuf = NULL;
	kernelBuf = new char[limit + 1]; //can cho chuoi terminal
	if (kernelBuf == NULL)
		return kernelBuf;
		
	memset(kernelBuf, 0, limit + 1);
	
	for (i = 0; i < limit; i++)
	{
		machine->ReadMem(virtAddr + i, 1, &oneChar);
		kernelBuf[i] = (char)oneChar;
		if (oneChar == 0)
			break;
	}
	return kernelBuf;
}

int System2User(int virtAddr, int len, char* buffer)
{
	if (len < 0) return -1;
	if (len == 0)return len;
	int i = 0;
	int oneChar = 0;
	do{
		oneChar = (int)buffer[i];
		machine->WriteMem(virtAddr + i, 1, oneChar);
		i++;
	} while (i < len && oneChar != 0);
	return i;
}


void
ExceptionHandler(ExceptionType which)
{
    int type = machine->ReadRegister(2);

    switch (which) {
	case NoException:
		return;
	case SyscallException:
		switch (type){
			case SC_Halt:
				DEBUG('a', "\n Shutdown, initiated by user program.");
				printf ("\n\n Shutdown, initiated by user program.\n");
				interrupt->Halt();
				break;


			case SC_Create:
			{
				int virtAddr;
				char* filename;
				DEBUG('a',"\n SC_Create call ...");
				DEBUG('a',"\n Reading virtual address of filename");
				// Lấy tham số tên tập tin từ thanh ghi r4
				virtAddr = machine->ReadRegister(4);
				DEBUG ('a',"\n Reading filename.");
				// MaxFileLength là = 32
				filename = User2System(virtAddr,MaxFileLength+1);
				if (filename == NULL)
				{
					printf("\n Not enough memory in system");
					DEBUG('a',"\n Not enough memory in system");
					machine->WriteRegister(2,-1); 
					delete filename;
					return;
				}
				DEBUG('a',"\n Finish reading filename.");
				
				if (!fileSystem->Create(filename,0))
				{
					printf("\n Error create file '%s'",filename);
					machine->WriteRegister(2,-1);
					delete filename;
					return;
				}
				machine->WriteRegister(2,0);
				delete filename;
				break;
			}

			case SC_ReadChar:
			    {
				    // Tao buffer de doc size 255
				    char* buffer = new char[255];
				    int len = gSynchConsole->Read(buffer, 255);
	
	                // Xu ly loi khi doc
				    if(len == -1)
				    {
					    printf("Doc ki tu bi loi");
					    DEBUG('a', "\nERROR: reading char.");
					    break;
				    }
				    
	
					// Ghi ki tu vua doc vao thanh ghi r2
					char c = buffer[0];
					machine->WriteRegister(2, c);
	
				    delete buffer; // giai phong vung cap phat
				    IncreasePC();
				    return;
			    }
	
			    case SC_PrintChar:
			    {
				    // Doc ki tu o thanh ghi r4
				    char c = (char)machine->ReadRegister(4);
	                   
	                // In ra console
				    gSynchConsole->Write(&c, 1);
				    IncreasePC();
	                return;
			    }

			case SC_ReadString:
			{
				int virtAddr, length;
				char* buffer;
				virtAddr = machine->ReadRegister(4); 
				length = machine->ReadRegister(5); 
				buffer = User2System(virtAddr, length);
				gSynchConsole->Read(buffer, length);
				System2User(virtAddr, length, buffer);
				delete buffer; 
				IncreasePC(); 
				return;
			}

			case SC_PrintString:
			{
				int virtAddr;
				char* buffer;
				virtAddr = machine->ReadRegister(4);
				buffer = User2System(virtAddr, 255); 
				int length = 0;
				while (buffer[length] != 0) length++; 
				gSynchConsole->Write(buffer, length + 1); 
				delete buffer; 
				IncreasePC(); 
				return;

			}

			
			case SC_Fork:
			{

				int virtAddr;
				virtAddr = machine->ReadRegister(4);

			    // Tao thread moi
			    Thread *newThread = new Thread("new thread");


				// Tro khong gian cua thread vua tao den thread hien tai
			    newThread->space = currentThread->space;
				// Luu trang thai register
			    newThread->SaveUserState();
				

				// Luu trang thai CPU register
			    newThread->userRegisters[PCReg] = virtAddr;
			    newThread->userRegisters[NextPCReg] = newThread->userRegisters[PCReg] + 4;
		    
			    machine->WriteRegister(PCReg, virtAddr);
			    machine->WriteRegister(NextPCReg, virtAddr + 4);

				// Phuc hoi lai trang thai register
			    newThread->RestoreUserState();	

				IncreasePC();
				return;
			}

			case SC_Yield:
			{
			}

			default:
				printf("\n Unexpected user mode exception (%d %d)", which,
				type);
				interrupt->Halt();
		} // end Exception switch
	} // end which switch
}
