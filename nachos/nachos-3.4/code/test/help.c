#include "syscall.h"
#include "copyright.h"
#define MAX_LENGTH 255

int main()
{
	char tmp;
	PrintString("\n\tYOUR TEST\n");
	PrintString("[.] Enter your string: ");
	ReadChar(tmp);
//	PrintString("Your string: ");
	PrintChar(tmp);
	return 0;
}
