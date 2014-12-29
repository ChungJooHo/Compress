#include "LZSS.h"

#define WINDOW_SIZE 65535
#define MAX_LEN 259
#define MIN_MATCH 3


// min , max �Լ� 
int min(int a,int b){return a>b?b:a;}
int max(int a,int b){return a>b?a:b;}

int bcmp(const char *p,const char *q)
// �󸶳� �������� ���ϴ� �Լ�. ������ MAX_LEN ������ ���Ѵ�.  
{
	int i;
	// ���� �� 
  for(i=0;p[i] == q[i] && i < MAX_LEN && p > &q[i];i++);
  return i;
}

int LZSS_encode(unsigned char *buffer, int size, unsigned char *result)
// LZSS ���ڵ� �Լ�. 
{
	int i,j,k;
	
	// rsize - ���� ���� ũ��
	// winPos - �����̵� ������ ���� ��ġ
	// keyCount - key-value�� ���� ī��Ʈ. 8������ ����
	// keyPos - key�� ����Ǵ°��� ��ġ 
  int rsize = 1;
  int winPos = 0;
  int keyCount = 0;
  char *keyPos = result;
  int len, pos;
  
  int *hash_table[256];
  
  // hash table �ʱ�ȭ. �����̺��� ã������ ���ڿ��� ���� �ּҸ� ��Ƴ�������. 
  for(i=0;i<256;i++)
	{
		hash_table[i] = (int *)malloc(sizeof(int) * 70000);
		hash_table[i][0] = 0;
  }
  
  
  for(i=0;i<size;)
  {
  	// LZSS �� �����̵� ���������̱� ������ ���� ���� 
  	if(i > WINDOW_SIZE) winPos = i - WINDOW_SIZE;
 
 		len = pos = 0;
 		
 		// hash table�� �̿��� ���ſ� ���� ���ڿ��� �˻��Ѵ�. 
 		for(j=0; j < hash_table[buffer[i]][0]; j++)
 		{
 			int curPos = hash_table[buffer[i]][j+1];
			if(curPos < winPos)
			// ���̺��� ���� ��Ÿ���� �ּҰ� �����츦 �Ѿ�� ������ �۾�. 
 			{
 				// hash table�� ������ �ϳ��� ������ ���� �ε����� ����. 
 				hash_table[ buffer[i] ][j+1] = hash_table[ buffer[i] ][ hash_table[ buffer[i]][0] ];
 				hash_table[ buffer[i] ][0]--;
 				j--;
 				continue;	
 			}
 			
 			// ���̸� ���Ѵ��� �ִ밪�� �����Ѵ�. 
 			k = bcmp(&buffer[i],&buffer[curPos]);
 			if (len < k)
 			{
 				// �ִ���̿� �� ��ġ ����. 
 				len = k;
 				pos = curPos - winPos;
 			}
 		}
    
  	// �Ź� ��Ÿ�� ���ڸ� �����Ѵ�. 
    hash_table[ buffer[i] ][0]++;
    hash_table[ buffer[i] ][ hash_table[ buffer[i] ][0] ] = i;
    
    // �ּ� 3����Ʈ �̻� ������ ���� 
		if(len > MIN_MATCH)
    {
    	for(j = 1; j < len; j++)
    	{
    		for(k=0;k < hash_table[buffer[i]][0]; k++)
    		{
    			int curPos = hash_table[buffer[i]][k+1];
					if(curPos < winPos)
					// ���̺��� ���� ��Ÿ���� �ּҰ� �����츦 �Ѿ�� ������ �۾�. 
					{
						// hash table�� ������ �ϳ��� ������ ���� �ε����� ����. 
						hash_table[ buffer[i] ][k+1] = hash_table[ buffer[i] ][ hash_table[ buffer[i]][0] ];
						hash_table[ buffer[i] ][0]--;
						j--;
						continue;	
					}
				}
				hash_table[ buffer[i+j] ][0]++;
				hash_table[ buffer[i+j] ][ hash_table[ buffer[i+j] ][0] ] = i+j;
    	}
    	// keyPos �� ���� key���� ����. ��ġ�� ��ٸ� 1, �ȵǸ� 0�� ǥ��� 
      *keyPos |= 1 << keyCount;
      
      // ǥ��� 3����Ʈ�� �̷������, 1����Ʈ�� ����, 2����Ʈ�� ��ġ�� ������. 
      // ���� 4���� ǥ���ϹǷ� 0 �� 4�� �ǰ��ϴ� ����ȭ ���. 
      *(result+rsize)=(unsigned char)(len-MIN_MATCH-1);
      *(unsigned short *)(result+rsize+1)=pos;
      
      // 3����Ʈ�� ǥ���ϹǷ� 3�� ���� 
      rsize+=MIN_MATCH;
      
    	// ã�� ���̸�ŭ ���� 
      i+=len;
    }
    
    // ������ϸ� ���� ���ڸ� ��� 
    else
    {
    	
      *(result+rsize)=buffer[i];
      rsize++;
      i++;
    }
    
    // key�� 8�� ����ϸ� ���ο� ��ġ�� key�� ���� 
    keyCount++;
    if(keyCount == 8)
    {
      keyCount = 0;
      keyPos = result + rsize;
      rsize++;
    }
  }
  
  //���� ���� 
  return rsize;
}

int LZSS_decode(char *buffer, int size, char *result)
// LZSS ���ڵ� �Լ�. ���ڵ��� �����̴�. 
{
	int i,j;
	int winPos, curPos = 0;
	int rsize = 0;
	int keyCount = 0;
	int keyVal = buffer[curPos++];
	for(;curPos<size;)
	{
		
		// ���� key ���� 0�� ���. �ٷ� �ѹ���Ʈ �о�´�. 
		if(keyVal % 2 == 0)
		{
			result[rsize++] = buffer[curPos++];
		}
		
		// key���� 1�ΰ��. ������ �Ǿ��ִ�. 
		else
		{
			int len, pos;
			
			// ù����Ʈ�� len, �ٸ� �ι���Ʈ�� pos �� ��Ÿ����. 
			len = (unsigned char)buffer[curPos] + MIN_MATCH + 1;
			pos = *(unsigned short *)(&buffer[curPos+1]);
			
			// 3����Ʈ ����ϹǷ� 3����. ������ ��ġ ����. 
			curPos += MIN_MATCH;
			winPos = max(rsize - WINDOW_SIZE, 0);
			i = winPos + pos;
			j = i + len;
			
			
			// len,pos �� window�� ������ ���ŷκ��� ���ڸ� �����´�. 
			for(;i < j;)
			{
				result[rsize++] = result[i++];
			}
		}
		
		// key�� �Ѻ�Ʈ ����Ʈ�Ͽ� ���� key�� ������ �� �ֵ��� �� 
		keyVal >>= 1;
		
		// key�� 8�� �о�鿴���Ƿ� ���� key�� ������ 
		keyCount++;
		if(keyCount == 8)
		{
			keyCount = 0;
			keyVal = buffer[curPos++];
		}
	}
	
	//���ڵ� ũ�� ���� 
	return rsize;
}
