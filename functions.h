#include <iostream>
#include <fstream>
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
    string nombreDisco = name + ".dat";
	if(existFile(nombreDisco.c_str()))
	{
		return;
	}
	int bmSize = cantBloques / 8;
	int tamanoBloque = 1024;
	ofstream out(nombreDisco.c_str(),ios::out | ios::in);
	//metaData
	out.write((char*)&bmSize,4);
	out.write((char*)&cantEntradas,4);
	out.write((char*)&tamanoBloque,4);
	out.write((char*)&cantBloques,4);
	//Bitmap
	out.write((char*)&cantBloques,4);
	int bytesBitMap = bmSize /8;
	char* bitmap = new char[bytesBitMap];
	for(int i = 0; i < bytesBitMap; i++)
	{
		bitmap[i] = 0;
	}
	out.write(bitmap,bytesBitMap);
	//escribir informacion vacia de files entries
	for(int i = 0; i < cantEntradas; i++)
	{
		char* datosVacios = new char[51];
		out.write(datosVacios,51);
		bool ocupado = false;
		out.write((char*)&ocupado,4);
	}
	//escribir informacion vacia de bloques de data
	for(int i = 0; i < cantBloques; i++)
	{
		char* vacio = new char[1020];
		int sig = -1;
		out.write(vacio,1020);
		out.write((char*)&sig,4);
	}
	out.close();
}

