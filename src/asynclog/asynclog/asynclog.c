#include <Windows.h>
#include <stdio.h>

int main()
{
	while (1)
	{
		for (int vKey = 0; vKey < 256; ++vKey)
		{
			if (GetAsyncKeyState(vKey) != 0)
				printf("%c", vKey);
		}
	}

	return 0;
}