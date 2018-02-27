#ifndef E2_PROYECTO1_REGISTRO_H
#define E2_PROYECTO1_REGISTRO_H

#include "disc_structs.h"
#include "data_file.h"
#include <iostream>
#include <cstdlib>
#include <string.h>

#define FILE_ENTRY_SIZE 60
#define DATA_BLOCK_SIZE 1024
#define METADATA_SIZE 16

using namespace std;

class registro
{
public:
    registro(char* nombre);
    void guardar_entry();
    void guardar_archivo();
    void guardar_file_Entry(file_entry file,int pos);
    void guardar_directorio();
    void guardar_block(int pos);
    void guardar_block_data(data_block block,int pos);
    void mostrar_archivo();
    void mostrar_directorio();
    char* to_char_block();
    char* to_char_directorio();
    char* to_char_archivo();
    void from_char_block(char* input);
    void from_char_directorio(char* input);
    void from_char_archivo(char* input);
    void leer_directorio(int pos);
    void listar_directorio();
    data_block getDataBlock(int pos);
    file_entry getEntry(int pos);
    file_entry getEntry(string name);
    string pathDirActual(file_entry fil);
    void leer_directorio(string name);
    void leer_archivo(int pos);
    //La funcion leer archivo igual, pero buscando por nombre
    void leer_archivo(string name);
    void leer_data_block(int pos);
    void cerrar_archivo();
    void abrir_archivo();
    int getBit(unsigned char byte,int position);
    int turnBitOff(unsigned char byte,int position);
    int turnBitOn(unsigned char byte,int position);
    //debe obtener el primer bloque de data vacio
    int getByteBlockEmpty();
    int getFirstBlockEmpty();
    //debe obtener el primer File Entry vacio
    int getFirstFileEntryEmpty();
    char* getBitMap();
    //esta funcion debe colocarle al block_item la posicion del siguiente bloque de data
    void setNextBlockItm(int posNext);
    //solo retorna el valor de posBlockActual
    int getPosBlockActual();
    void setPrimerBlock();
    int getFirstEntryEmpty();
    void setMetaData(METADATA meta);
    //esta funcion debe agregar la posicion del nuevo archivo creado, al directorio en el que se encuentra actualmente
    void addNewArchivoToDir(int pos);
    int getPosDirectorioActual();
    void actualizar_block_data(data_block b,int pos);
    //esta funcion debe obtener el primer bloque de data del archivo
    //y colocarlo en la variable  block_item
    void leerFirstBlockDataArchivo();
    int charToInt(char* x);
    char* readChar(int desde, int cantLeer, char* read);
    void crearDir(string name);
    void mostrarEntry(file_entry fil);
    void cambiarDirectorio(string nameDir);

    data_file *archivo;
    //este Int debe tener la posicion del bloque de data actual;
    int posBlockActual;
    int posArchivoActual;
    int posPrimerBlock;
    int posPrimerEntry;
    METADATA meta;
    data_block *block_item;
    // esta variable guarda la posicion dentro del disco en la que se encuentra
    // ese directorio.
    int posDirectorioItem;
    file_entry *directorio_item;
    file_entry *archivo_item;
    string dirPath;
};


#endif //E2_PROYECTO1_REGISTRO_H
