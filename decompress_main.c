#include "compress.h"

// decompress 메인 
int main(int argc,char *argv[])
{
	// 인자가 있으면 함수 수행 
  if(argc == 2)
	{
		compress(argv[1],1);
	}
}
