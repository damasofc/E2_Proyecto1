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
    return this->archivo.leer(16,sizeBitmap);
}
int registro::getFirstBlockEmpty()
{
    char* bitmap = getBitMap();
    for(int i = 0; i < (sizeof(bitmap)*8); i++)
    {
        if(getBit(*bitmap,i) == 0)
            return i;
    }
    //si llega a retornar -1, significa que ya no hay espacios vacios
    return -1;
    
}
