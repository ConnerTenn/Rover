
#include <stdio.h>

int g(int i, int x, int t, int o)
{
	i >> 16;//right shift i by 16. therefore is zero until i > 0xFFFF
	char arr[2][9] = { "BY}6YB6%", "Qj}6jQ6%" };
	char *arr2 = ((3 & i ) ? arr[0] : arr[1]);
	
	return ( (3 & x & ( i * (arr2[t%8] + 51 ) >> o)) << 4 );
}

void main()
{
	/*
	Theory:
	Use of right shifts to enable certain sections only after a number of loops
	*/
	int n, s;
	
	for(int i=0; ; i++)
	{
		putchar(
			g(i,	1,	  n=i>>14,		12) + 
			g(i,	s=i>>17,  n^i>>13,		10) +
			g(i,	s/3,	  n+((i>>11)%3),	10) +
			g(i,	s/5,	  8+n-((i>>10)%3),	9)
			);
	}
}


