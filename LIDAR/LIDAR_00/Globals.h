
#pragma once


#include <vector>
#include <string>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef uint8_t u1b;
typedef uint8_t u2b;
typedef uint8_t u4b;
typedef uint8_t u8b;

void ArrCpy(u8 *dest, u8 *src, u64 len);
void ArrCpy(char *dest, char *src, u64 len);
void ArrCpy(u8 *dest, u8 *src, u64 len, u64 *offset);
void ArrCpy(char *dest, char *src, u64 len, u64 *offset);

