#include <cstdio>  
#include <cstdlib>
 
const int SZ = 1 << 20;  //提升IO buff 
struct fastio{
	char inbuf[SZ];
	char outbuf[SZ];
	fastio(){
		setvbuf(stdin, inbuf, _IOFBF, SZ);
		setvbuf(stdout, outbuf, _IOFBF, SZ);
	}
}io;
 
#define N 10000001
int points[N];
 
int main()
{
	int n, m, i, x;
	int a, b;
	scanf("%d %d", &n, &m);
 
	for (i = 0; i < N; i++)
		points[i] = 0;
	
	while (n--)
	{
		scanf("%d", &x);
		points[x + 1] = 1;
	}
 
	for (i = 2; i <= N; ++i)
		points[i] += points[i - 1];
 
	while (m--)
	{
		scanf("%d %d", &a, &b);
		printf("%d\n", points[b + 1] - points[a]);
	}
	return 0;
}