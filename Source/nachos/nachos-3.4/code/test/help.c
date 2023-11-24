#include "syscall.h"
#include "copyright.h"

int main()
{
	const char* helpTable =
    "\nChao mung den voi chuong trinh cua chung toi!\n"
                          "Chuong trinh nay bao gom cac tinh nang sau:\n"
                          "- Sap xep mot mang n so nguyen bang thuat toan Bubble Sort.\n"
                          "- Hien thi bang ma ASCII.\n"
                          "Voi gioi han so nguyen n khong vuot qua 100.\n"
                          "Chung toi la mot nhom gom 5 thanh vien.";

	int len;
	PrintString("\n\t\t\t----- INTRODUCTION -----\n");
	PrintString(helpTable);
	PrintString("\n(1). Hoang Van Quoc       - 21120541");
	PrintString("\n(2). Tran Doan Thanh Vinh - 21120596");
	PrintString("\n(3). Nguyen Vu Thanh      - 21120335");
	PrintString("\n(4). Nguyen Van Sieu      - 21120321");
	PrintString("\n(5). Nguyen Trong Tri     - 21120344\n");

	return 0;
}
