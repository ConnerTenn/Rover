
#include "Globals.h"

void ArrCpy(u8 *dest, u8 *src, u64 len)
{
	for (u64 i = 0; i < len; i++) { *(dest + i) = src[i]; }
}

void ArrCpy(char *dest, char *src, u64 len)
{
	for (u64 i = 0; i < len; i++) { *(dest + i) = src[i]; }
}

void ArrCpy(u8 *dest, u8 *src, u64 len, u64 *offset)
{
	for (u64 i = 0; i < len; i++, (*offset)++) { *(dest + *offset) = src[i]; }
	//*offset += len;
}

void ArrCpy(char *dest, char *src, u64 len, u64 *offset)
{
	for (u64 i = 0; i < len; i++, (*offset)++) { *(dest + *offset) = src[i]; }
	//*offset += len;
}

