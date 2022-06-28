#include <stdio.h>

int main(void)
{
	int i=0;
	int n=0;
	int sum=0;

	scanf("%d\n", &n);
	for(i=0;i<n;i++)
		sum+=n;

	printf("%d\n", sum);

	return 0;
}
