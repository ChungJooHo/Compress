#include "LZSS.h"

#define WINDOW_SIZE 65535
#define MAX_LEN 259
#define MIN_MATCH 3


// min , max 함수 
int min(int a,int b){return a>b?b:a;}
int max(int a,int b){return a>b?a:b;}

int bcmp(const char *p,const char *q)
// 얼마나 같은지를 비교하는 함수. 구조상 MAX_LEN 까지만 비교한다.  
{
	int i;
	// 실제 비교 
  for(i=0;p[i] == q[i] && i < MAX_LEN && p > &q[i];i++);
  return i;
}

int LZSS_encode(unsigned char *buffer, int size, unsigned char *result)
// LZSS 인코딩 함수. 
{
	int i,j,k;
	
	// rsize - 최종 압축 크기
	// winPos - 슬라이딩 윈도우 시작 위치
	// keyCount - key-value를 위한 카운트. 8번마다 갱신
	// keyPos - key가 저장되는곳의 위치 
  int rsize = 1;
  int winPos = 0;
  int keyCount = 0;
  char *keyPos = result;
  int len, pos;
  
  int *hash_table[256];
  
  // hash table 초기화. 이테이블은 찾으려는 문자열의 시작 주소를 모아놓은곳임. 
  for(i=0;i<256;i++)
	{
		hash_table[i] = (int *)malloc(sizeof(int) * 70000);
		hash_table[i][0] = 0;
  }
  
  
  for(i=0;i<size;)
  {
  	// LZSS 는 슬라이딩 윈도우기법이기 때문에 범위 설정 
  	if(i > WINDOW_SIZE) winPos = i - WINDOW_SIZE;
 
 		len = pos = 0;
 		
 		// hash table을 이용해 과거에 나온 문자열을 검색한다. 
 		for(j=0; j < hash_table[buffer[i]][0]; j++)
 		{
 			int curPos = hash_table[buffer[i]][j+1];
			if(curPos < winPos)
			// 테이블의 값이 나타내는 주소가 윈도우를 넘어서서 없에는 작업. 
 			{
 				// hash table의 끝에서 하나를 가져와 현재 인덱스에 삽입. 
 				hash_table[ buffer[i] ][j+1] = hash_table[ buffer[i] ][ hash_table[ buffer[i]][0] ];
 				hash_table[ buffer[i] ][0]--;
 				j--;
 				continue;	
 			}
 			
 			// 길이를 구한다음 최대값을 저장한다. 
 			k = bcmp(&buffer[i],&buffer[curPos]);
 			if (len < k)
 			{
 				// 최대길이와 그 위치 저장. 
 				len = k;
 				pos = curPos - winPos;
 			}
 		}
    
  	// 매번 나타난 문자를 저장한다. 
    hash_table[ buffer[i] ][0]++;
    hash_table[ buffer[i] ][ hash_table[ buffer[i] ][0] ] = i;
    
    // 최소 3바이트 이상 같으면 압축 
		if(len > MIN_MATCH)
    {
    	for(j = 1; j < len; j++)
    	{
    		for(k=0;k < hash_table[buffer[i]][0]; k++)
    		{
    			int curPos = hash_table[buffer[i]][k+1];
					if(curPos < winPos)
					// 테이블의 값이 나타내는 주소가 윈도우를 넘어서서 없에는 작업. 
					{
						// hash table의 끝에서 하나를 가져와 현재 인덱스에 삽입. 
						hash_table[ buffer[i] ][k+1] = hash_table[ buffer[i] ][ hash_table[ buffer[i]][0] ];
						hash_table[ buffer[i] ][0]--;
						j--;
						continue;	
					}
				}
				hash_table[ buffer[i+j] ][0]++;
				hash_table[ buffer[i+j] ][ hash_table[ buffer[i+j] ][0] ] = i+j;
    	}
    	// keyPos 에 현재 key값을 저장. 매치가 됬다면 1, 안되면 0이 표기됨 
      *keyPos |= 1 << keyCount;
      
      // 표기는 3바이트로 이루어지며, 1바이트는 길이, 2바이트는 위치를 저장함. 
      // 길이 4부터 표기하므로 0 이 4가 되게하는 최적화 기법. 
      *(result+rsize)=(unsigned char)(len-MIN_MATCH-1);
      *(unsigned short *)(result+rsize+1)=pos;
      
      // 3바이트를 표기하므로 3을 증가 
      rsize+=MIN_MATCH;
      
    	// 찾은 길이만큼 증가 
      i+=len;
    }
    
    // 압축안하면 현재 문자를 기록 
    else
    {
    	
      *(result+rsize)=buffer[i];
      rsize++;
      i++;
    }
    
    // key를 8번 기록하면 새로운 위치에 key를 저장 
    keyCount++;
    if(keyCount == 8)
    {
      keyCount = 0;
      keyPos = result + rsize;
      rsize++;
    }
  }
  
  //길이 리턴 
  return rsize;
}

int LZSS_decode(char *buffer, int size, char *result)
// LZSS 디코딩 함수. 인코딩의 역순이다. 
{
	int i,j;
	int winPos, curPos = 0;
	int rsize = 0;
	int keyCount = 0;
	int keyVal = buffer[curPos++];
	for(;curPos<size;)
	{
		
		// 현재 key 값이 0인 경우. 바로 한바이트 읽어온다. 
		if(keyVal % 2 == 0)
		{
			result[rsize++] = buffer[curPos++];
		}
		
		// key값이 1인경우. 압축이 되어있다. 
		else
		{
			int len, pos;
			
			// 첫바이트는 len, 다른 두바이트는 pos 를 나타낸다. 
			len = (unsigned char)buffer[curPos] + MIN_MATCH + 1;
			pos = *(unsigned short *)(&buffer[curPos+1]);
			
			// 3바이트 사용하므로 3증가. 윈도우 위치 설정. 
			curPos += MIN_MATCH;
			winPos = max(rsize - WINDOW_SIZE, 0);
			i = winPos + pos;
			j = i + len;
			
			
			// len,pos 와 window를 가지고 과거로부터 문자를 가져온다. 
			for(;i < j;)
			{
				result[rsize++] = result[i++];
			}
		}
		
		// key를 한비트 쉬프트하여 다음 key를 가져올 수 있도록 함 
		keyVal >>= 1;
		
		// key를 8번 읽어들였으므로 다음 key를 가져옴 
		keyCount++;
		if(keyCount == 8)
		{
			keyCount = 0;
			keyVal = buffer[curPos++];
		}
	}
	
	//디코딩 크기 리턴 
	return rsize;
}
