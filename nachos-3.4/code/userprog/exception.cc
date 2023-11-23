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
#define MAX_STRING_LENGTH   2056

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

void IncreasePC()
{
    int counter = machine->ReadRegister(PCReg);
    machine->WriteRegister(PrevPCReg, counter);
    counter = machine->ReadRegister(NextPCReg);
    machine->WriteRegister(PCReg, counter);
    machine->WriteRegister(NextPCReg, counter + 4);
}

char *User2System(int virtAddr, int limit)
{
    int i; // chi so index
    int oneChar;
    char *kernelBuf = NULL;
    kernelBuf = new char[limit + 1]; // can cho chuoi terminal
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

int System2User(int virtAddr, int len, char *buffer)
{
    if (len < 0)
        return -1;
    if (len == 0)
        return len;
    int i = 0;
    int oneChar = 0;
    do
    {
        oneChar = (int)buffer[i];
        machine->WriteMem(virtAddr + i, 1, oneChar);
        i++;
    } while (i < len && oneChar != 0);
    return i;
}

void ExceptionHandler(ExceptionType which)
{
    int type = machine->ReadRegister(2);

    switch (which)
    {
    case NoException:
        return;
    case PageFaultException:
        DEBUG('a', "\n No valid translation found");
        printf("\n\n No valid translation found");
        interrupt->Halt();
        break;

    case ReadOnlyException:
        DEBUG('a', "\n Write attempted to page marked read-only");
        printf("\n\n Write attempted to page marked read-only");
        interrupt->Halt();
        break;

    case BusErrorException:
        DEBUG('a', "\n Translation resulted invalid physical address");
        printf("\n\n Translation resulted invalid physical address");
        interrupt->Halt();
        break;

    case AddressErrorException:
        DEBUG('a', "\n Unaligned reference or one that was beyond the end of the address space");
        printf("\n\n Unaligned reference or one that was beyond the end of the address space");
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
        DEBUG('a', "\n Number exception types");
        printf("\n\n Number exception types");
        interrupt->Halt();
        break;

    case SyscallException:
    {
        switch (type)
        {
	case SC_Halt:
            DEBUG('a', "\nShutdown, initiated by user program. ");
            printf("\nShutdown, initiated by user program. ");
            interrupt->Halt();
            return;
        case SC_ReadString:
        {
            int bufferAddr, maxStringLengthInput;
            char *inputBuffer;

            bufferAddr = machine->ReadRegister(4);                  // Get the address of the 'buffer' parameter from register 4
            maxStringLengthInput = machine->ReadRegister(5);             // Get the maximum length of the input string from register 5
	    if(length > MAX_STRING_LENGTH) {
		DEBUG('a', "\n Not enough buffer to read tring");
        	printf("\n Not enough buffer to read tring");
		IncreasePC();
		return;
	    }
            inputBuffer = User2System(bufferAddr, maxStringLengthInput); // Copy the string from User Space to System Space
		
            gSynchConsole->Read(inputBuffer, maxStringLengthInput); // Use SynchConsole's Read function to read the string

            System2User(bufferAddr, maxStringLengthInput, inputBuffer); // Copy the string from System Space to User Space

            delete[] inputBuffer; // Free the allocated memory for the buffer

            IncreasePC(); // Increment Program Counter
            return;
        }

        case SC_PrintString:
        {
            int bufferAddr;
            char *inputBuffer;
	
            bufferAddr = machine->ReadRegister(4);      // Get the address of the 'buffer' parameter from register 4
            inputBuffer = User2System(bufferAddr, MAX_STRING_LENGTH); // Copy the string from User Space to System Space with a buffer of 255 characters
		
            int length = 0;
            while (inputBuffer[length] != 0)
                length++; // Calculate the actual length of the string
	if(length > MAX_STRING_LENGTH) {
		DEBUG('a', "\n Not enough buffer to printString");
        	printf("\n Not enough buffer to printString");
		IncreasePC();
		return;
	}
            gSynchConsole->Write(inputBuffer, length + 1); // Use SynchConsole's Write function to print the string

            delete[] inputBuffer; // Free the allocated memory for the buffer

            IncreasePC(); // Tang Program Counter
            // return;
            break;
        }

        case SC_Create:
        {
            int virtAddr;
            char *filename;
            DEBUG('a', "\n SC_Create call ...");
            DEBUG('a', "\n Reading virtual address of filename");
            // Lấy tham số tên tập tin từ thanh ghi r4
            virtAddr = machine->ReadRegister(4);
            DEBUG('a', "\n Reading filename.");
            int MaxFileLength = 32;
            filename = User2System(virtAddr, MaxFileLength + 1);
            if (filename == NULL)
            {
                printf("\n Not enough memory in system");
                DEBUG('a', "\n Not enough memory in system");
                machine->WriteRegister(2, -1); // trả về lỗi cho chương
                // trình người dùng
                delete filename;
                return;
            }
            DEBUG('a', "\n Finish reading filename.");
            // DEBUG(‘a’,"\n File name : '"<<filename<<"'");
            //  Create file with size = 0
            //  Dùng đối tượng fileSystem của lớp OpenFile để tạo file,
            //  việc tạo file này là sử dụng các thủ tục tạo file của hệ điều
            //  hành Linux, chúng ta không quản ly trực tiếp các block trên
            //  đĩa cứng cấp phát cho file, việc quản ly các block của file
            //  trên ổ đĩa là một đồ án khác
            if (!fileSystem->Create(filename, 0))
            {
                printf("\n Error create file '%s'", filename);
                machine->WriteRegister(2, -1);
                delete filename;
                return;
            }
            machine->WriteRegister(2, 0); // trả về cho chương trình
                                
          // người dùng thành công
	    IncreasePC();
            delete filename;
            break;
        }
        default:
            printf("\n Unexpected user mode exception (%d %d)", which,
                   type);
            interrupt->Halt();
        }
    }
    }
}
