/* sort.c 
 *    Test program to sort a large number of integers.
 *
 *    Intention is to stress virtual memory system.
 *
 *    Ideally, we could read the unsorted array off of the file system,
 *	and store the result back to the file system!
 */

#include "syscall.h"



int main()
{
	int A[100];	/* size of physical memory; with code, we'll run out of space!*/
	int i, j, tmp, n, x;
	PrintString("\nNhap n: ");
	n=ReadInt();
	PrintString("\nBat dau nhap mang: ");


    	/* first initialize the array, in reverse sorted order */
    	for (i = 0; i < n; i++)	
	{	
       		x = ReadInt();
		//A[i]=x;
		//PrintInt(x);
		A[i]=x;
	}

    	PrintString("\nMang da sap xep: ");
    	for (i = 0; i < n-1; i++)
        	for (j = i + 1; j < n; j++)
	   		if (A[i] > A[j]) 
			{	
	      			tmp = A[i];
	      			A[i] = A[j];
	      			A[j] = tmp;
    	   		}
	for (i = 0; i < n; i++)	
	{	
       		
		PrintInt(A[i]);
	}
    	Exit(A[0]);		/* and then we're done -- should be 0! */
}
