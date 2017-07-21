#include <Windows.h>
#include <stdio.h>

#include "httprequest.h"

int main()
{
	char *data = NULL;
	DWORD dataSize = 0;

	http_request("www.example.com", 80, FALSE, "GET", "/", NULL, 0, NULL, 0, &data, &dataSize);

	printf("%s", data);

	system("PAUSE");
	return 0;
}