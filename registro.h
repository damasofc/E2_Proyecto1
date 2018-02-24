#ifndef E2_PROYECTO1_REGISTRO_H
#define E2_PROYECTO1_REGISTRO_H

#include "disc_structs.h"
#include "data_file.h"
#include <iostream>
#include <cstdlib>

#define FILE_ENTRY_SIZE 52
#define DATA_BLOCK_SIZE 1020
#define METADATA_SIZE 16

using namespace std;

class registro
{
public:
    registro(char* nombre);
    void guardar_entry();
    void guardar_block(int pos);
    void mostrar_archivo();
    void mostrar_directorio();
    char* to_char_block();
    char* to_char_directorio();
    void from_char_block(char* input);
    void from_char_directorio(char* input);
    void from_char_archivo(char* input);
    void leer_file_entry(int pos);
    void leer_data_block(int pos);
    void cerrar_archivo();
    void abrir_archivo();
    int getBit(unsigned char byte,int position);
    int turnBitOff(unsigned char byte,int position);
    int turnBitOn(unsigned char byte,int position);
    //debe obtener el primer bloque de data vacio
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

private:
    data_file *archivo;
    //este Int debe tener la posicion del bloque de data actual;
    int posBlockActual;
    int posPrimerBlock;
    int posPrimerEntry;
    METADATA meta;
    data_block *block_item;
    // esta variable guarda la posicion dentro del disco en la que se encuentra
    // ese directorio.
    int posDirectorioItem;
    file_entry *directorio_item;
    file_entry *archivo_item;
};


#endif //E2_PROYECTO1_REGISTRO_H
