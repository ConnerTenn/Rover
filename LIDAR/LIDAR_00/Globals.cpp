
#include "Globals.h"

void ArrCpy(u8 *dest, const u8 src[], u64 len)
{
	for (u64 i = 0; i < len; i++) { *(dest + i) = src[i]; }
}

void ArrCpy(char *dest, const char src[], u64 len)
{
	for (u64 i = 0; i < len; i++) { *(dest + i) = src[i]; }
}

