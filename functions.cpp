#include <iostream>
 
using namespace std;

struct data_block{
	char data[1020];
	int siguiente;
};

struct file_entry {
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

void createDisc(string name, int cantEntradas, int cantBloques){
    
}

