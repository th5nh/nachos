/* sort.c
 *    Test program to sort a large number of integers.
 *
 *    Intention is to stress virtual memory system.
 *
 *    Ideally, we could read the unsorted array off of the file system,
 *	and store the result back to the file system!
 */

#include "syscall.h"

int
main()
{
	int A[100];	/* size of physical memory; with code, we'll run out of space!*/
	int i, j, tmp, n;

	PrintString("\n\t\t -------- SORT -------\n");
	PrintString("\n[.] Nhap n: ");
	n = ReadInt();
	PrintString("[.] Bat dau nhap mang:\n");

    	/* first initialize the array, in reverse sorted order */
    	for (i = 0; i < n; i++)
	{
		A[i] = ReadInt();
	}

	PrintString("\n# Mang cua ban: ");

	for (i = 0; i < n; i++) {
		PrintInt(A[i]);
		PrintChar(' ');
	}

    	// Bubble sort
    	for (i = 0; i < n-1; i++) {
        	for (j = i + 1; j < n; j++) {
	   		if (A[i] > A[j]) {
	      			tmp = A[i];
	      			A[i] = A[j];
	      			A[j] = tmp;
    	   		}
		}
	}

	PrintString("\n# Mang da sap xep: ");
	for (i = 0; i < n; i++) {
		PrintInt(A[i]);
		PrintChar(' ');
	}

    	//Exit(A[0]);		/* and then we're done -- should be 0! */
	return 0;
}
