#include <Windows.h>
#include <stdio.h>

int main()
{
	int last_vKey = 0;

	while (1)
	{
		for (int vKey = 0; vKey < 255; ++vKey)
		{
			if (GetAsyncKeyState(last_vKey) != 0)
				continue;

			last_vKey = 0;

			if (GetAsyncKeyState(vKey) != 0)
			{
				last_vKey = vKey;

				printf("%c", vKey);
			}
		}

	}

	return 0;
}