#include "substitution.h"


char return_alphabet(int t)	//���ڷ� ���� ������ ���� ���ڷ� ����
{
	if (t == 0)					//a=0
		return 'a';
	else if (t == 1)			//t=1
		return 't';
	else if (t == 2)			//g=2
		return 'g';
	else if (t == 3)			//c=3
		return 'c';
}

int substitution_encode(char*ibuffer, int num, char*output)
{
	int i, j, count = 0;					//ibuffer�� i��°, output�� j��°
	unsigned int res = 0;					//res(result)�� ibuffer�� ������ ���������� ġȯ�Ѱ��� �ʱ�ȭ
	for (j = i = 0; i<num; i++)				//ibuffer�� ������ ���������� ġȯ 
	{
		if(ibuffer[i] == 10)continue;		//ibuffer[i]�� �����϶� continue
		res = res << 2;						//res�� 2��Ʈ �������� shift (���� ���ڸ� �Է��ϱ�����)
		res += return_int(ibuffer[i]);		//ibuffer�� i��°�� ���뿡 ���� �����ϴ� ���� res�� �Է�
		count++;							//count++
		if (count == 16)					//i+1�� ������ 16�谡 �ɶ����� res�� ���� ���Ͽ� �Է�, res=0���� �ʱ�ȭ 
		{
			*(unsigned int *)(&output[j]) = res;	//output�� j��°�� res ���� �Է�
			j += sizeof(int);				//output���� int�������� ��Ʈ�� ���Ƿ� j+4
			res = 0;						//res=0���� �ʱ�ȭ
			count = 0;						//count=0���� �ʱ�ȭ
		}
	}
	if (count)								//i=num�� �Ǿ� ������ Ż�������� (count%16 != 0)�� �Ǿ� res�� �Է����� ��������
	{ 
		*(unsigned int *)(&output[j]) = res;		//�׶��� res���� output�� j��°�� �Է�
		j += sizeof(int);							//j+4
		
		output[j] = count;						//res���� �Է��� output[j]���� +4�� �ּҿ� ���ϱ��̸� 16���� ���������� �������� ����
		j++;									//j++
	}
	return j;										//output�� ���� ����
}
int substitution_decode(char*ibuffer, int num, char*output)
{
	int i, j, count, n;								//�������Ͽ��� ���Ͱ� ����������� ����=count, ibuffer ���ּ�=i, output ���ּ�=j
	unsigned int res = 0;							//ibuffer�� ������ ���������� ġȯ 
	for (j = i = count = 0; i <= num - 8;)				//num-8ȸ ibuffer�� ������ output�� ����
	{
		res = *(unsigned int *)(&ibuffer[i]);		//ibuffer�� i��° ���� res�� �Է�
		int k = 30;									//res���� ���������� k��ŭ shift���������� 2����� ��Ʈ�� ������ k �ʱ�ȭ(�ʱⰪ=32-2=30)
		for (n = 0; n < 16; n++)					//32��Ʈ�� 2��Ʈ�� ����� �����ϹǷ� ������ 16ȸ ���� ��
		{
			int t = (res >> k) % 4;					//res�� k��ŭ ���������� shift �� �� 4�� ������ shift�ϰ� �������� ���� ������ 2��Ʈ�� ����
			output[j++] = return_alphabet(t);		//t�� ���� ���� output[j]�� �Է�, j++
			count++;								//count++
			if (count == 60)						//count�� 60�� �Ǿ�����
			{
				output[j] = 10;						//output[j]�� ���� �Է�
				j++;								//j++
				count = 0;							//count �ʱ�ȭ
			}
			k -= 2;									//k-=2
		}
		i += 4;										//ibuffer�� int�����ŭ ������ �ԷµǾ������Ƿ� i+4
	}

	if ((num - i) % 4)								//ibuffer�� �������� ����������
	{
		int k = ibuffer[i + 4] * 2 - 2;					//ibuffer[i+4]�� ���ϱ��̸� 16���� ���������� ��������(������ ����)�� ����Ǿ������Ƿ� *2-2��ŭ shift
		res = *(unsigned int *)(&ibuffer[i]);		//ibuffer[i]��° ���� res�� �Է�
		for (n = 0; n < ibuffer[i + 4]; n++)			//ibuffer[i+4]ȸ ������ ���� �� 
		{
			int t = (res >> k) % 4;					//���� for���� ���� ������ ����, ������ ��
			output[j++] = return_alphabet(t);
			k -= 2;
		}
		output[j++] = 10;							//�������� output[j]�� ���͸� �Է� j++
	}
	return j;										//output�� ���̸� ����
}
