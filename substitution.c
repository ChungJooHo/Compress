#include "substitution.h"


char return_alphabet(int t)	//인자로 받은 정수에 따라 문자로 리턴
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
	int i, j, count = 0;					//ibuffer의 i번째, output의 j번째
	unsigned int res = 0;					//res(result)에 ibuffer의 내용을 정수형으로 치환한것을 초기화
	for (j = i = 0; i<num; i++)				//ibuffer의 내용을 정수형으로 치환 
	{
		if(ibuffer[i] == 10)continue;		//ibuffer[i]가 엔터일때 continue
		res = res << 2;						//res를 2비트 왼쪽으로 shift (다음 문자를 입력하기위해)
		res += return_int(ibuffer[i]);		//ibuffer의 i번째의 내용에 따라 리턴하는 값을 res에 입력
		count++;							//count++
		if (count == 16)					//i+1의 개수가 16배가 될때마다 res의 값을 파일에 입력, res=0으로 초기화 
		{
			*(unsigned int *)(&output[j]) = res;	//output의 j번째에 res 값을 입력
			j += sizeof(int);				//output에는 int사이즈의 비트가 들어가므로 j+4
			res = 0;						//res=0으로 초기화
			count = 0;						//count=0으로 초기화
		}
	}
	if (count)								//i=num이 되어 루프를 탈출했으나 (count%16 != 0)이 되어 res에 입력하지 못했을때
	{ 
		*(unsigned int *)(&output[j]) = res;		//그때의 res값을 output의 j번째에 입력
		j += sizeof(int);							//j+4
		
		output[j] = count;						//res값을 입력한 output[j]에서 +4한 주소에 파일길이를 16으로 나누었을때 나머지값 저장
		j++;									//j++
	}
	return j;										//output의 길이 리턴
}
int substitution_decode(char*ibuffer, int num, char*output)
{
	int i, j, count, n;								//원본파일에서 엔터가 나오기까지의 길이=count, ibuffer 현주소=i, output 현주소=j
	unsigned int res = 0;							//ibuffer의 내용을 정수형으로 치환 
	for (j = i = count = 0; i <= num - 8;)				//num-8회 ibuffer의 내용을 output에 저장
	{
		res = *(unsigned int *)(&ibuffer[i]);		//ibuffer의 i번째 값을 res에 입력
		int k = 30;									//res값을 오른쪽으로 k만큼 shift연산했을때 2배수의 비트가 남도록 k 초기화(초기값=32-2=30)
		for (n = 0; n < 16; n++)					//32비트를 2비트씩 나누어서 연산하므로 루프를 16회 돌게 함
		{
			int t = (res >> k) % 4;					//res를 k만큼 오른쪽으로 shift 한 후 4로 나누면 shift하고 났을때의 가장 오른쪽 2비트가 나옴
			output[j++] = return_alphabet(t);		//t의 값에 따라 output[j]에 입력, j++
			count++;								//count++
			if (count == 60)						//count가 60이 되었을때
			{
				output[j] = 10;						//output[j]에 엔터 입력
				j++;								//j++
				count = 0;							//count 초기화
			}
			k -= 2;									//k-=2
		}
		i += 4;										//ibuffer에 int사이즈만큼 정수가 입력되어있으므로 i+4
	}

	if ((num - i) % 4)								//ibuffer의 마지막에 도달했을때
	{
		int k = ibuffer[i + 4] * 2 - 2;					//ibuffer[i+4]에 파일길이를 16으로 나누었을때 나머지값(마지막 길이)이 저장되어있으므로 *2-2만큼 shift
		res = *(unsigned int *)(&ibuffer[i]);		//ibuffer[i]번째 값을 res에 입력
		for (n = 0; n < ibuffer[i + 4]; n++)			//ibuffer[i+4]회 루프를 돌게 함 
		{
			int t = (res >> k) % 4;					//위의 for문과 같은 내용의 연산, 루프를 돔
			output[j++] = return_alphabet(t);
			k -= 2;
		}
		output[j++] = 10;							//마지막에 output[j]에 엔터를 입력 j++
	}
	return j;										//output의 길이를 리턴
}
