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
void IncreasePC()
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
	{
		switch(type) {
		case SC_Halt:
		{
			DEBUG('a', "\nShutdown, initiated by user program.\n");
			printf("\nShutdown, initiated by user program\n");
			interrupt->Halt();
			break;
		}
		case SC_ReadInt:
		{
			//Chuoi luu so dang char
			char* numberchar;
			//Do dai toi da cua chuoi
                    	int MAX_BUFFER = 256;			
                    	numberchar = new char[MAX_BUFFER];
			//Do dai duoc doc
                    	int length = gSynchConsole->Read(numberchar, MAX_BUFFER);
			//Khoi tao so nguyen bang 0
                    	int number = 0; 						
                        //Mac dinh la so duong           
                    	bool isNegative = false; 
			//Vi tri bat dau cua so, mac dinh la 0
                    	int head = 0;
			//Vi tri ket thuc cua so, mac dinh la chieu dai duoc doc
                    	int tail = length -1;
			
			//Kiem tra xem co phai so am khong
                    	if(numberchar[0] == '-')
                    	{
                        	isNegative = true;
				//Doi vi tri bat dau so sang 1 ky tu
                        	head = 1;                        			   		
                    	}
                    
                    	//Kiem tra so co hop le khong
                    	for(int i = head; i < length; i++)					
                    	{
				
				//Kiem tra co xuat hien ki tu khac ky tu so khong
                        	if(numberchar[i] < '0' || numberchar[i] > '9')
                        	{
                            		printf("\n Khong phai so nguyen, tra ve so 0 mac dinh");
                            		DEBUG('a', "\n Khong phai so nguyen, tra ve so 0 mac dinh");
					//Tra ve thanh ghi r2 so 0 mac dinh
                            		machine->WriteRegister(2, 0);
                            		IncreasePC();
                            		delete numberchar;
                            		return;
                        	}
				    
                    	}			
                    
             		//Chuyen so tu char sang int       
                    	for(int i = head; i<= tail; i++)
                    	{
                        	number = number * 10 + (int)(numberchar[i] - 48); 
                    	}
                    
                    	//Chuyen thanh so am neu co
                    	if(isNegative)
                    	{
                        	number = number * -1;
                    	}
			//Tra ve thanh ghi r2
                    	machine->WriteRegister(2, number);
                    	IncreasePC();
                    	delete numberchar;
                    	return;	
		}
		case SC_PrintInt:
		{	
		    	//Doc so tu thanh ghi r4
                    	int number = machine->ReadRegister(4);
			                   
                    	
			//Mac dinh la so duong
                    	bool isNegative = false; 
			//Do dai so
                    	int length = 0; 
                    	int head = 0; 
			//Kiem tra so co am khong
                    	if(number < 0)
                    	{
                        	isNegative = true;
				//Chuyen thanh so duong cho de thao tac
                        	number = number * -1; 
				//Nhuong 1 vi tri cho dau tru
                        	head = 1; 
                    	} 
	
                    	//Bien tam de dem do dai so
                    	int temp = number;
			//Dem do dai 
                    	while(temp > 0)
                    	{
                        	length = length + 1;
                        	temp = temp / 10;
                    	}
    
		    	//Chuoi so de in ra man hinh
                    	char* numberchar;
			//Chuoi so nguoc
			char* reverse;
			//Do dai toi da cua so
                    	int MAX_BUFFER = 256;
                    	numberchar = new char[MAX_BUFFER];
			reverse = new char[MAX_BUFFER];

			//Chuyen so thanh chuoi nguoc
                    	for(int i = 0; i < length; i++)
                    	{
                        	reverse[i] = (char)((number % 10) + 48);
                        	number = number / 10;
                    	}
                    	if(isNegative)
                    	{
				//Them dau tru
                        	numberchar[0] = '-';
				//Dao lai chuoi
				for(int i = 0; i < length; i++)
				{
					numberchar[length - i] = reverse[i];				
				}
				//In so ra man hinh
                        	gSynchConsole->Write(numberchar, length + 1);
                        	delete numberchar;
                        	IncreasePC();
                        	return;
                    	}

		    	//Dao lai chuoi
			for(int i = 0; i < length; i++)
			{
				numberchar[length - i - 1] = reverse[i];				
			}
			//In so ra man hinh
                    	gSynchConsole->Write(numberchar, length);
                    	delete numberchar;
                    	IncreasePC();
                    	return;        			
					
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
			int bufferAddr, maxStringLengthInput;
			char *inputBuffer;

            		bufferAddr = machine->ReadRegister(4);                  // Get the address of the 'buffer' parameter from register 4
            		maxStringLengthInput = machine->ReadRegister(5);             // Get the maximum length of the input string from register 5
	    		if(maxStringLengthInput > MAX_STRING_LENGTH) {
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
            return;
            //break;
		}
		default:
			IncreasePC(); //and do nothing
                	//break;
			return;
		}
		break;
	}
	default:
		printf("\nUnexpected user mode exception %d\n", which);
		ASSERT(FALSE);
		break;
	}
}
