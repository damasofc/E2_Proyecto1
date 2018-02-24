#include <iostream>
#include <fstream>
#include <string.h>
#include <cstring>
#include <bitset>
#include "disc_structs.h"
#include "registro.h"
 
using namespace std;

bool existFile(const char *fileName)
{
    ifstream infile(fileName);
    return infile.good();
}

bool createDisc(string name, int cantEntradas, int cantBloques)
{
    string nombreDisco = name + ".dat";
	if(existFile(nombreDisco.c_str()))
	{
		return false;
	}
	int bmSize = cantBloques / 8;
	ofstream out(nombreDisco.c_str(),ios::in | ios::out | ios::binary);
	out.open(nombreDisco.c_str());
	//metaData
	METADATA meta;
	meta.bm_size = bmSize;
	meta.entry_amount = cantEntradas;
	meta.block_size = 1024;
	meta.block_amount = cantBloques;
	out.write(reinterpret_cast<char *> (&meta),sizeof(meta));
	//Bitmap
	char* bitmap = new char[bmSize];
	for(int i = 0; i < bmSize; i++)
	{
		bitmap[i] = 0;
	}
	out.write(bitmap,bmSize);
	for(int i = 0; i < cantEntradas; i++)
	{
		file_entry fat;
		for(int m = 0; m < 30; m++)
		{
			fat.nombre[m] = '-';
		}
		fat.tamano = 0;
		fat.tipo[0] = 'O';
		fat.padre = -1;
		fat.primer_hijo = -1;
		fat.hermano_derecho = -1;
		fat.primer_bloque_data = -1;
		fat.libre = true;
		out.write(reinterpret_cast<const char*>(&fat),sizeof(fat));
	}
	//escribir informacion vacia de bloques de data
	cout<<"sizeof de data block: "<<sizeof(data_block)<<endl;
	for(int i = 0; i < cantBloques; i++)
	{
		data_block bloque;
		bloque.siguiente = -1;
		out.write(reinterpret_cast<char *>(&bloque),sizeof(bloque));
	}
	return true;
}

void importar_archivo(string nombre_archivo,registro regis)
{
	ifstream in(nombre_archivo.c_str(),ios::in | ios::binary);
	int punteroPos = 0;
	in.seekg(0,ios::end);
	int tamanoArch = in.tellg();
	in.seekg(0,ios::beg);
	int dataBlocksOcupar = tamanoArch/1020;
	int tamanoUltimoDataBlock = tamanoArch%1020;
	//INICIO: crear el fileEntry para el archivo
	file_entry file;
	strcpy(file.nombre,nombre_archivo.c_str());
	file.tamano = tamanoArch;
	file.tipo[0] = 'A';
	file.padre = regis.getPosDirectorioActual();
	file.primer_hijo = -1;
	file.hermano_derecho = -1;
	file.libre = false;
	file.primer_bloque_data = regis.getFirstBlockEmpty();
	regis.from_char_archivo(reinterpret_cast<char*> (&file));
	//aca debo de guardarlo en el disco
	regis.addNewArchivoToDir(regis.getFirstEntryEmpty());
	regis.guardar_entry();


	//FIN: crear el fileEntry para el archivo
	for(int m = 0; m < dataBlocksOcupar; m++)
	{	
		char* block = new char[1020];
		if(m == 0)
		{
			in.read(block,1020);
			regis.from_char_block(block);
			regis.guardar_block(regis.getFirstBlockEmpty());	
		}
		else
		{
			in.read(block,1020);
			//INICIO: le digo cual es el siguiente bloque al que esta actualmente y de ultimo lo guardo nuevamente
			regis.setNextBlockItm(regis.getFirstBlockEmpty());
			regis.guardar_block(regis.getPosBlockActual());
			//FIN: le digo cual es el siguiente bloque al que esta actualmente y de ultimo lo guardo nuevamente
			regis.from_char_block(block);
			//en esta funcion de guardar block debe actualizarse la posicion actual del block
			regis.guardar_block(regis.getFirstBlockEmpty());	
		}
		
	}
	char* block = new char[tamanoUltimoDataBlock];
	in.read(block,tamanoUltimoDataBlock);
	regis.setNextBlockItm(regis.getFirstBlockEmpty());
	regis.guardar_block(regis.getPosBlockActual());
	regis.from_char_block(block);
	regis.guardar_block(regis.getFirstBlockEmpty());
	
}

void exportarArchivo(registro regis,string archivoExport,string dirDestino,string nombreNuevo)
{
	ofstream out(nombreNuevo.c_str(),ios::binary | ios::app);
	out.open(nombreNuevo.c_str(),ios::binary | ios::app);
	regis.leer_archivo(archivoExport);
	regis.leerFirstBlockDataArchivo();
	char* primer = new  char[sizeof(regis.to_char_block())];
	strcpy(primer,regis.to_char_block());
	out.write(primer,1020);
	char* val = new char[4];
	val[0] = primer[1021];
	val[1] = primer[1022];
	val[2] = primer[1023];
	val[3] = primer[1024];
	int sigPos = regis.charToInt(val);
	while(sigPos != -1)
	{
		regis.leer_data_block(sigPos);
		char* primer = new  char[sizeof(regis.to_char_block())];
		strcpy(primer,regis.to_char_block());
		out.write(primer,1020);
		char* val = new char[4];
		val[0] = primer[1021];
		val[1] = primer[1022];
		val[2] = primer[1023];
		val[3] = primer[1024];
		int sigPos = regis.charToInt(val);
	}
}

