#include <Windows.h>
#include <stdio.h>

// .text:0000000140001000 ; Imagebase   : 140000000
#define IMAGE_BASE				0x140000000

// .data:0000000140003034 word_140003034  dw 64h; DATA XREF : sub_140001070 + 1Dr
#define PLAYERHEALTH_OFFSET		0x140003034

WORD PlayerHealth = 100;

ULONG_PTR fix_offset(ULONG_PTR uOffset)
{
	ULONG_PTR dwASLROffset = (ULONG_PTR)GetModuleHandle(NULL);
	return ((uOffset - IMAGE_BASE) + dwASLROffset);
}

void patch_word(LPWORD addr, WORD w)
{
	DWORD dwProtect;
	VirtualProtect((LPVOID)addr, 2, PAGE_EXECUTE_READWRITE, &dwProtect);

	*addr = w;

	VirtualProtect((LPVOID)addr, 2, dwProtect, &dwProtect);
}

int main()
{
	LPWORD offset = (LPWORD)fix_offset(PLAYERHEALTH_OFFSET);

	printf("PlayerHealth:\t%p = %d\n", &PlayerHealth, PlayerHealth);
	printf("Offset: \t%p\n", offset);

	patch_word(offset, 9001);

	printf("PlayerHealth:\t%p = %d\n", &PlayerHealth, PlayerHealth);

	system("PAUSE");
	return 0;
}