#include <Windows.h>
#include <stdio.h>

void hooked_heal_function(int hp)
{
	hp += 9000;
	printf("Maliciously healing for %d\n", hp);
}

void heal_function(int hp)
{
	printf("Healing for %d...\n", hp);
}

void hook_function(void *original, void *hook)
{
	/* MOV RAX, 0x1122334455667788 */
	/* JMP RAX */
	char asm[] = "\x48\xB8\x88\x77\x66\x55\x44\x33\x22\x11\xFF\xE0";
	unsigned long long *addr = (unsigned long long *)&asm[2];

	*addr = (unsigned long long)hook;

	DWORD dwProtect;
	SIZE_T nWritten;
	VirtualProtect(&heal_function, 0x1000, PAGE_EXECUTE_READWRITE, &dwProtect);
	WriteProcessMemory(GetCurrentProcess(), original, asm, 14, &nWritten);
}

int main()
{
	heal_function(100);

	hook_function(&heal_function, &hooked_heal_function);

	heal_function(100);

	system("PAUSE");
	return 0;
}