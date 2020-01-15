#include <stdio.h>
#include <printf.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
	FILE *originalFile;
	FILE *myFile;	 

	originalFile = fopen(argv[1], "r");	 

	if (argc != 3)
	{
		printf("Usage: %s <file1> <file2>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	myFile = fopen(argv[2], "w"); 
	fseek(originalFile, 0, SEEK_SET);

	int readback_ints[25];

	int *index_wanted = readback_ints;

	for (int row = 0; row < 5; row++)
	{
		for (int col = 0; col < 5; col++)
		{
			fread(index_wanted, sizeof(int), 1, originalFile);
			readback_ints[col+row*5] = *index_wanted;  /* This is important 
			so we do not continue to repeat the column 0-4 and write over it */
			index_wanted++;
		}

		fseek(originalFile, (5*sizeof(int)), SEEK_CUR); /* Need to multiply by 5
		to get the entire bytes of all 5 integers. */
	}
	myFile = fopen(argv[2], "w");
	fclose(originalFile);
	for (int i = 0; i < 25; i++)
	{
		fprintf(myFile, "%d\n", readback_ints[i]);
	}
}

/* 
can only read 25 integers total.
values at indicies 0-4 / 10-14 / 20-24 / 30-34 / 40-44.
write out the new 5x5 array in an output file.

array = {{0, 1, 2, 3, 4, 5, 6, 7, 8, 9},
		 {10, 11, 12, 13, 14, 15, 16, 17, 18, 19}, 
		 ...
		 {90, 91, 92, 93, 94, 95, 96, 97, 98, 99}}

desired output:
0
1
2
3
4
10
14 
...
40
41
42
43
44

fread(void *restrict ptr, size_t size, size_t nitems,
	FILE *restrict stream);

fwrite(const void *restrict ptr, size_t size, size_t nitems,
	FILE *restrict stream);

*/