#include "syscall.h"
#include "copyright.h"
#define MAX_LENGTH 255
int main()
{

	const char* helpTable =
    "Chao mung den voi chuong trinh cua chung toi!\n"
                          "Chuong trinh nay bao gom cac tinh nang sau:\n"
                          "- Sap xep mot mang n so nguyen bang thuat toan Bubble Sort.\n"
                          "- Hien thi bang ma ASCII.\n"
                          "Voi gioi han so nguyen n khong vuot qua 100.\n"
                          "Chung toi la mot nhom gom 5 thanh vien.";

	int len; 
	PrintString("\n\t\t\t-----HELP TRONG NACHOS-----\n\n");
	PrintString(helpTable);
	Halt();
	return 0;
}
