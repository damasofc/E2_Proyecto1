#include "registro.h"

registro::registro(char *nombre)
{
    archivo = new data_file(nombre);
}

void registro::abrir_archivo()
{
    archivo->abrir();
}

void registro::cerrar_archivo()
{
    archivo->cerrar();
}

int registro::getPosBlockActual()
{
    return posBlockActual;
}

void registro::mostrar_archivo()
{
    cout << "Nombre: " <<archivo_item->nombre << endl;
    cout << "Size: " <<archivo_item->tamano << endl;
    cout << "Tipo: Archivo" << endl;
}

void registro::mostrar_directorio()
{
    cout << "Nombre: " <<directorio_item->nombre << endl;
    cout << "Size: " <<directorio_item->tamano << endl;
    cout << "Tipo: Directorio" << endl;
}

void registro::setMetaData(METADATA meta)
{
    this->meta = meta;
}

void registro::setPrimerBlock()
{
    int bytesEntradas = meta.entry_amount*52;
    posPrimerBlock = bytesEntradas + meta.bm_size + METADATA_SIZE;
}

void registro::guardar_block(int pos)
{
    int offset = pos*1024;
    archivo->set_pos(posPrimerBlock+offset);
    archivo->escribir(to_char_block(), DATA_BLOCK_SIZE);
}

void registro::guardar_entry()
{
    int pos = getFirstEntryEmpty();
    int offset = pos*FILE_ENTRY_SIZE;
    archivo->set_pos(posPrimerEntry+offset);
    archivo->escribir(to_char_directorio(), FILE_ENTRY_SIZE);
}

int registro::getBit(unsigned char byte,int position)
{
	return (byte >> position) & 0x1;
}
//estas 2 funciones debo probarlas.
int registro::turnBitOff(unsigned char byte,int position)
{
	return (byte ^= (1 << position));
}
int registro::turnBitOn(unsigned char byte, int position)
{
	return byte |= (1u << position);	
}
char* registro::getBitMap()
{
    // this->abrir_archivo();
    int sizeBitmap;
    sizeBitmap = *this->archivo->leer(0,4);
    return this->archivo->leer(16,sizeBitmap);
}
int registro::getFirstBlockEmpty()
{
    char* bitmap = getBitMap();
    for(int i = 0; i < (sizeof(bitmap)*8); i++)
    {
        if(getBit(*bitmap,i) == 0)
            return i;
    }
    //si llega a retornar -1, significa que ya no hay espacios vacios
    return -1;
}

int registro::getFirstEntryEmpty()
{
    archivo->set_pos(posPrimerBlock);
    char* test;
    for(int i = 0; i < meta.entry_amount; i++)
    {
        strcpy(test, archivo->leer(FILE_ENTRY_SIZE));
        from_char_archivo(test);
        if(archivo_item->libre)
            return i;
    }
    return -1;
}

void registro::setNextBlockItm(int posNext)
{
    block_item->siguiente = posNext;
}

char* registro::to_char_block()
{
    char* retorno = new char[DATA_BLOCK_SIZE];
    strcpy(retorno, block_item->data);
    return retorno;
}

char* registro::to_char_directorio()
{
    char* retorno = new char[FILE_ENTRY_SIZE];
    strcpy(retorno, directorio_item->nombre);
    //strcpy(retorno, itoa())
}

void registro::from_char_block(char *input)
{
    strcpy(block_item->data, input);
}

