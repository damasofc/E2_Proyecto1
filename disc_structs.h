#ifndef E2_PROYECTO1_DISC_STRUCTS_H
#define E2_PROYECTO1_DISC_STRUCTS_H


struct data_block
{
    char data[1020];
    int siguiente = -1;
};

struct file_entry
{
    char nombre[30];
    int tamano = 0;
    char tipo[1]; // 'D' Directorio  o  'A' Archivo
    int padre = -1;
    int primer_hijo = -1;
    int hermano_derecho = -1;
    int primer_bloque_data = -1;
    bool libre = true;
};

struct METADATA
{
    int bm_size = 0; //bit map size
    int entry_amount = 0; //entradas(tamano)
    int block_size = 1024; //tamano bloque
    int block_amount = 0; //cantidad bloques
};

#endif //E2_PROYECTO1_DISC_STRUCTS_H
