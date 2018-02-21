#include "registro.h"

int registro::getBit(unsigned char byte,int position)
{
	return (byte >> position) & 0x1;
}
//estas 2 funciones debo probarlas.
int registro::turnBitOff(unsigned char byte,int position)
{
	return (byte ^= (1 << position));
}
int registro::turnBitOn(unsigned char byte, int position)
{
	return byte |= (1u << position);	
}
char* registro::getBitMap()
{
    // this->abrir_archivo();
    int sizeBitmap;
    sizeBitmap = *this->archivo.leer(0,4);
    return new char();
}
int registro::getFirstBlockEmpty()
{

}
