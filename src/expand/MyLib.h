#ifndef MYLIB_H
#define MYLIB_H

#define BITS_PER_BYTE 8
#define TRUE 1
#define FALSE 0

typedef unsigned char uchar;
typedef unsigned long ulong;
typedef unsigned int uint;
typedef unsigned short ushort;

#ifdef LITTLE_ENDIAN

#define SwapULong(val) (val << 24 | (val << 8 & 0xFF0000) \
 | (val >> 8 & 0xFF00) | val >> 24 & 0xFF)
#define SwapUShort(val) (val << BITS_PER_BYTE | val >> BITS_PER_BYTE)

#else

#define SwapULong(val) (val)
#define SwapUShort(val) (val)

#endif

#endif

