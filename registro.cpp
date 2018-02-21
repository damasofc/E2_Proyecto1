#include "registro.h"

int getBit(unsigned char byte,int position)
{
	return (byte >> position) & 0x1;
}
//estas 2 funciones debo probarlas.
int turnBitOff(unsigned char byte,int position)
{
	return (byte ^= (1 << position));
}
int turnBitOn(unsigned char byte, int position)
{
	return byte |= (1u << position);	
}
