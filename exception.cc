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

/* 
Input: - User space address (int) 
 - Limit of buffer (int) 
Output:- Buffer (char*) 
Purpose: Copy buffer from User memory space to System memory space 
*/ 
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

/* 
Input: - User space address (int) 
 - Limit of buffer (int) 
 - Buffer (char[]) 
Output:- Number of bytes copied (int) 
Purpose: Copy buffer from System memory space to User memory space 
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


void IncreasePC()
{
	int counter=machine->ReadRegister(PCReg);
	machine->WriteRegister(PrevPCReg, counter);
	counter=machine->ReadRegister(NextPCReg);
	machine->WriteRegister(PCReg, counter);
	machine->WriteRegister(NextPCReg, counter+4);
}

void ExceptionHandler(ExceptionType which)
{
	int type = machine->ReadRegister(2);

	switch (which) {
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
		switch (type){

		case SC_Halt:			
			DEBUG('a', "\nShutdown, initiated by user program. ");
			printf("\nShutdown, initiated by user program. ");
			interrupt->Halt();
			return;		

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
				machine->WriteRegister(2,-1); // trả về lỗi cho chương 
				// trình người dùng 
				delete filename; 
				return; 
			} 
			DEBUG('a',"\n Finish reading filename."); 
			//DEBUG(‘a’,"\n File name : '"<<filename<<"'"); 
			// Create file with size = 0 
			// Dùng đối tượng fileSystem của lớp OpenFile để tạo file, 
			// việc tạo file này là sử dụng các thủ tục tạo file của hệ điều 
			// hành Linux, chúng ta không quản ly trực tiếp các block trên 
			// đĩa cứng cấp phát cho file, việc quản ly các block của file 
			// trên ổ đĩa là một đồ án khác 
			if (!fileSystem->Create(filename,0)) 
			{ 
				printf("\n Error create file '%s'",filename); 
				machine->WriteRegister(2,-1); 
				delete filename; 
				return; 
			} 
			machine->WriteRegister(2,0); // trả về cho chương trình 
			// người dùng thành công 
			IncreasePC();
			delete filename; 
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
				//Neu co xuat hien dau cham
				if(numberchar[i] == '.')
				{
					//Kiem tra sau dau cham co so nao khac so 0 khong
					for(int j = i + 1; j < length; j++)
					{
						if(numberchar[j]!='0')
						{
							printf("\n\n Khong phai so nguyen");
                            				DEBUG('a', "\n Khong phai so nguyen");
							//Tra ve thanh ghi r2 so 0 mac dinh
                            				machine->WriteRegister(2, 0);
                            				IncreasePC();
                            				delete numberchar;
                            				return;
						}
					}
					tail = i - 1;
				}
				//Kiem tra co xuat hien ki tu khac ky tu so khong
                        	if(numberchar[i] < '0' && numberchar[i] > '9')
                        	{
                            		printf("\n\n Khong phai so nguyen");
                            		DEBUG('a', "\n Khong phai so nguyen");
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
			//Neu la so 0 thi in ra man hinh (so 0 la mac dinh)
		   	if(number == 0)
                   	{
                        	gSynchConsole->Write("0", 1); 
                        	IncreasePC();
                        	return;    
                    	}                    
                    	
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
				//numberchar[numberOfNum + 1] = 0;
                        	gSynchConsole->Write(numberchar, length + 1);
                        	delete numberchar;
                        	IncreasePC();
                        	return;
                    	}

		    	//numberchar[numberOfNum] = 0;	
			for(int i = 0; i < length; i++)
			{
				numberchar[length - i - 1] = reverse[i];
				
			}
                    	gSynchConsole->Write(numberchar, length);
                    	delete numberchar;
                    	IncreasePC();
                    	return;        			
					
		}
		
		default:
			break;
		}
		IncreasePC();
	}

	
				
}			
