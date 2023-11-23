#include "syscall.h"

int main() {
	char c;
	PrintString("Nhap mot ki tu: ");
	c = ReadChar();
	PrintString("Ki tu vua nhap la: ");
	PrintChar(c);
}
