#include <fstream>
#include <cstring>
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
	ofstream out(nombreDisco.c_str(),ios::binary);
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
		if(i == 0 )
		{
			strcpy(fat.nombre,"root");
			fat.libre = false;
			fat.tamano = 0;
			fat.tipo[0] = 'D';
		}
		else
		{
			for(int m = 0; m < 30; m++)
			{
				fat.nombre[m] = '-';
			}
			fat.tamano = 0;
			fat.libre = true;
			fat.tipo[0] = 'O';

		}
		fat.padre = -1;
		fat.primer_hijo = -1;
		fat.hermano_derecho = -1;
		fat.primer_bloque_data = -1;
		out.write(reinterpret_cast<char*>(&fat),sizeof(fat));
	}
	//escribir informacion vacia de bloques de data
	for(int i = 0; i < cantBloques; i++)
	{
		data_block bloque;
		bloque.siguiente = -1;
		out.write(reinterpret_cast<char *>(&bloque),sizeof(bloque));
	}
	return true;
}

void importar_archivo(string nombre_archivo,registro* regis)
{
	ifstream in(nombre_archivo.c_str(),ios::in | ios::binary);
	int punteroPos = 0;
	in.seekg(0,ios::end);
	int tamanoArch = in.tellg();
	in.seekg(0,ios::beg);
	int dataBlocksOcupar = tamanoArch/1020;
	int tamanoUltimoDataBlock = tamanoArch%1020;
	//INICIO: crear el fileEntry para el archivo
	file_entry* file = new file_entry;
	strcpy(file->nombre,nombre_archivo.c_str());
	file->tamano = tamanoArch;
	file->tipo[0] = 'A';
	file->padre = regis->getPosDirectorioActual();
	file->primer_hijo = -1;
	file->hermano_derecho = -1;
	file->libre = false;
	file->primer_bloque_data = regis->getFirstBlockEmpty();
	//cout<<"tamano archivo "<< file->tamano<<endl;
	//return;
	//regis->from_char_archivo(reinterpret_cast<char*> (file));
	//aca debo de guardarlo en el disco
	regis->addNewArchivoToDir(regis->getFirstEntryEmpty());
	regis->guardar_file_Entry(*file,regis->getFirstEntryEmpty());
	//regis->guardar_archivo();

	//FIN: crear el fileEntry para el archivo
	data_block block;
	int tempPos = -1;
	int conte = 0;
	while(conte < dataBlocksOcupar)
	{	
		char read[1020];
		if(conte == 0)
		{
			in.read(read,1020);
			memcpy(block.data,read,1020);
			block.siguiente = -1;
			//INICIO: le digo cual es el siguiente bloque al que esta actualmente y de ultimo lo guardo nuevamente
			tempPos = regis->getFirstBlockEmpty();
			regis->guardar_block_data(block,regis->getFirstBlockEmpty());
		}
		else
		{
			block.siguiente = regis->getFirstBlockEmpty();
			regis->actualizar_block_data(block,tempPos);
			//regis->guardar_block_data(block,tempPos);
			in.read(read,1020);
			memcpy(block.data,read,1020);
			block.siguiente = -1;
			//INICIO: le digo cual es el siguiente bloque al que esta actualmente y de ultimo lo guardo nuevamente
			tempPos = regis->getFirstBlockEmpty();
			//cout<<"antes-- "<<regis->getFirstBlockEmpty()<<endl;
			regis->guardar_block_data(block,regis->getFirstBlockEmpty());
			//cout<<"despues-- "<<regis->getFirstBlockEmpty()<<endl;
		}
		conte++;
	}
	cout<<"antes de ir al ultimo-- "<<regis->getFirstBlockEmpty()<<endl;
	block.siguiente = regis->getFirstBlockEmpty();
	regis->actualizar_block_data(block,tempPos);
	//regis->guardar_block_data(block,tempPos);
	data_block blockFinal;
	in.read(blockFinal.data,tamanoUltimoDataBlock);
	regis->guardar_block_data(blockFinal,regis->getFirstBlockEmpty());
	
}

void exportarArchivo(registro regis,string archivoExport,string dirDestino,string nombreNuevo)
{
	ofstream out(nombreNuevo.c_str(),ios::out | ios::binary);
	//out.open(nombreNuevo.c_str(),ios::binary | ios::app);
	file_entry f = regis.getEntry(archivoExport);
	data_block b = regis.getDataBlock(f.primer_bloque_data);
	out.write(b.data,1020);
	int dataBlocksLeer = f.tamano/1020;
	int lastBlockSize = f.tamano%1020;
	b = regis.getDataBlock(b.siguiente);
	bool continua = true;
	int dataBlocksLeidos = 1;
	while(continua)
	{
		dataBlocksLeidos++;
		//strcat(escritura,b.data);
		if(dataBlocksLeidos == (dataBlocksLeer+1))
		{
			out.write(b.data,lastBlockSize);
			continua = false;
			break;
		}
		out.write(b.data,1020);
		if(b.siguiente == -1)
		{
			continua = false;
		}
		else
		{
			b = regis.getDataBlock(b.siguiente);
		}
	}
	cout<<"datas leidos: "<<dataBlocksLeidos<<endl;
	//out.write(escritura,strlen(escritura));
}

