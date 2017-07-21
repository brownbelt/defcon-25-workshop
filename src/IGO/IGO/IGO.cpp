#include <stdio.h>
#include <stdlib.h>

struct IGO
{
	IGO(int n)
	{
		printf("In IGO::IGO(%d)\n", n);
	}
};

// instantiation of global objects
IGO *igo0 = new IGO(0);
IGO igo1(1);

int main()
{
	printf("In main()\n");

	system("PAUSE");
	return 0;
}