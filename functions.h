#include <iostream>
#include <fstream>
#include <string.h>
#include <cstring>
#include "disc_structs.h"
 
using namespace std;

bool existFile(const char *fileName)
{
    ifstream infile(fileName);
    return infile.good();
}

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
	return (byte |= (1 << position));	
}

void createDisc(string name, int cantEntradas, int cantBloques)
{
	int posPuntero = 0;
    string nombreDisco = name + ".dat";
	if(existFile(nombreDisco.c_str()))
	{
		return;
	}
	int bmSize = cantBloques / 8;
	int tamanoBloque = 1024;
	ofstream out(nombreDisco.c_str(),ios::in | ios::out | ios::binary);
	out.open(nombreDisco.c_str());
	//metaData
	out.write((char*)&bmSize,4);
	out.write((char*)&cantEntradas,4);
	out.write((char*)&tamanoBloque,4);
	out.write((char*)&cantBloques,4);
	//Bitmap
	out.write((char*)&cantBloques,4);
	char* bitmap = new char[bmSize];
	for(int i = 0; i < bmSize; i++)
	{
		bitmap[i] = 0;
	}
	out.write(bitmap,bmSize);
	posPuntero = 20 + bmSize +1;
	//escribir informacion vacia de files entries
	file_entry fat[cantEntradas];
	for(int i = 0; i < cantEntradas; i++)
	{
		for(int m = 0; m < 30; m++)
		{
			fat[i].nombre[m] = '-';
		}
		fat[i].tamano = 0;
		fat[i].tipo = 'O';
		fat[i].padre = -1;
		fat[i].primer_hijo = -1;
		fat[i].hermano_derecho = -1;
		fat[i].primer_bloque_data = -1;
		fat[i].libre = true;
		//out.write(reinterpret_cast<const char*>(&fat[i]),sizeof(file_entry));
	}
	out.write(reinterpret_cast<const char*>(fat),sizeof(fat));
	//escribir informacion vacia de bloques de data
	for(int i = 0; i < cantBloques; i++)
	{
		data_block* bloque = new data_block();
		bloque->siguiente = -1;
		out.write((char*)bloque,sizeof(bloque));
	}
	return;
}

