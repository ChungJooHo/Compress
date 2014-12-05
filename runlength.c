#include "runlength.h"
#include <stdio.h>
#include <stdlib.h>

int runlength_encode(char *buffer, int size, char *output)
{
	int i,j,count;
	count = 1;
	j = 0;
	for (i = 1; i < size; i++)
	{
		if (buffer[i - 1] == buffer[i])
		{
			count++;
			if (count == 255)
			{
				output[j++] = buffer[i - 1];
				output[j++] = count;
				count = 1;
			}
		}
		else
		{
			output[j++] = buffer[i - 1];
			if(count > 1) output[j++] = count;
			count = 1;
		}
	}
	if (count > 1)
	{
		output[j++] = buffer[i - 1];
		output[j++] = count;
	}
	return j;	
}
