#ifndef E2_PROYECTO1_REGISTRO_H
#define E2_PROYECTO1_REGISTRO_H

#include "disc_structs.h"
#include "data_file.h"

#define FILE_ENTRY_SIZE 52
#define DATA_BLOCK_SIZE 1020
#define METADATA_SIZE 16

class registro
{
public:
    registro();
    void guardar_entry();
    void guardar_block();
    void mostrar_entry();
    void mostrar_block();
    char* to_char_block();
    char* to_char_entry();
    void from_char_block(char* input);
    void from_char_entry(char* input);
    void leer_file_entry(int pos);
    void leer_data_block(int pos);
    void cerrar_archivo();
    void abrir_archivo();
    void importar_archivo(char* nombre_archivo);
    void exportar_archivo(char* nombre_archivo);
    int getBit(unsigned char byte,int position);
    int turnBitOff(unsigned char byte,int position);
    int turnBitOn(unsigned char byte,int position);

private:
    data_file archivo;
    data_block block_item;
    file_entry directorio_item;
    file_entry archivo_item;
};


#endif //E2_PROYECTO1_REGISTRO_H
