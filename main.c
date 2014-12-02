#include "compress.h"

int main(int argc,char *argv[])
{
  int p = time(0);
  if(argc == 2)
	{
		compress(argv[1]);
	}
  printf("%d",time(0)-p);
}
