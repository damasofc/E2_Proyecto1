#include <iostream>
 
using namespace std;

struct data_block
{
	char data[1020];
	int siguiente;
};

struct file_entry 
{
	char nombre[30];
	int tamano;
    // 'D' DIrectorio  o  'A' Archivo
	char tipo;
	int padre;
	int primer_hijo;
	int hermano_derecho;
	int primer_bloque_data;
	bool libre;

};

struct METADATA
{
	int bm_size; //bit map size
	int entry_amount; //entradas(tamano)
	int block_size; //tamano bloque
	int block_amount; //cantidad bloques
};

void createDisc(string name, int cantEntradas, int cantBloques){
    
}

