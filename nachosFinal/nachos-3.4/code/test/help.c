#include "syscall.h" 
#include "copyright.h" 

#define maxlen 32 

int main() 
{ 
	int number;
	PrintString("\n Nhap so:");
	number=ReadInt();
//	number = 'A';
	PrintString("So la: ");
	PrintInt(number);
	return 0;
//	char c;
//	PrintString("Nhap mot ki tu: ");
//	c = ReadChar();
//	PrintString("Ki tu vua nhap la: ");
//	PrintChar(c);

/*	char input[256];
	ReadString(input, 256);

	PrintString("String la: ");
	PrintString(input);

	const char* helpTable =
    "\nChao mung den voi chuong trinh cua chung toi!\n"
                          "Chuong trinh nay bao gom cac tinh nang sau:\n"
                          "- Sap xep mot mang n so nguyen bang thuat toan Bubble Sort.\n"
                          "- Hien thi bang ma ASCII.\n"
                          "Voi gioi han so nguyen n khong vuot qua 100.\n"
                          "Chung toi la mot nhom gom 5 thanh vien.";

	int len; 
	PrintString("\n\t\t\t-----HELP TRONG NACHOS-----\n\n");
	PrintString(helpTable);
	//Halt(); */
	return 0;
}
