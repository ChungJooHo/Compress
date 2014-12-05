#include "substitution.h"

int substitution_encode(char *buffer, int size, char *output)
{
	int i, j, count = 0;
	unsigned int res = 0;
	for (j = i = 0; i < size; i++)				//ibuffer�� ������ ���������� ġȯ 
	{
		res = res << 2;

		if (buffer[i] == 't')
			res += 1;
		else if (buffer[i] == 'g')
			res += 2;
		else if (buffer[i] == 'c')
			res += 3;
		else if (buffer[i] == 'a')
			res += 0;
		else
			continue;
		
		count++;
		
		if (count == 16)			//i+1�� ������ 16�谡 �ɶ����� res�� ���� ���Ͽ� �Է�, res=0���� �ʱ�ȭ 
		{
			*(unsigned int *)(&output[j]) = res;
			j += sizeof(int);
			res = 0;
			count = 0;
		}
	}
	if (count)
	{	
		*(unsigned int *)(&output[j]) = res;
		j += sizeof(int);
		output[j] = count;
		j++;
	}
	return j;
}


