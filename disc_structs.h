#ifndef E2_PROYECTO1_DISC_STRUCTS_H
#define E2_PROYECTO1_DISC_STRUCTS_H


struct data_block
{
    char data[1020];
    int siguiente;
};

struct file_entry
{
    char nombre[30];
    int tamano;
    char tipo[1]; // 'D' Directorio  o  'A' Archivo
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

#endif //E2_PROYECTO1_DISC_STRUCTS_H
