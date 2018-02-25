#include <atomic>
#include "registro.h"

registro::registro(char* nombre)
{
    archivo = new data_file(nombre);
    this->directorio_item = new file_entry();
    this->archivo_item = new file_entry();
    this->block_item = new data_block();
    METADATA metaDat;
    archivo->abrir();
    metaDat.bm_size = charToInt(archivo->leer(0,4));
    metaDat.entry_amount = charToInt(archivo->leer(5,8));
    metaDat.block_size = charToInt(archivo->leer(9,12));
    metaDat.block_amount = charToInt(archivo->leer(13,16));
    setMetaData(metaDat);
    leer_directorio(0);

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
    char* test = new char[FILE_ENTRY_SIZE];
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
    strcpy(retorno, archivo_item->nombre);
    strcat(retorno, (char*)&archivo_item->tamano);
    strcat(retorno, archivo_item->tipo);
    strcat(retorno, (char*)&archivo_item->padre);
    strcat(retorno, (char*)&archivo_item->primer_hijo);
    strcat(retorno, (char*)&archivo_item->hermano_derecho);
    strcat(retorno, (char*)&archivo_item->primer_bloque_data);
    strcat(retorno, (char*)&archivo_item->libre);
    return retorno;
}

char* registro::to_char_archivo()
{
    char* retorno = new char[FILE_ENTRY_SIZE];
    strcpy(retorno, directorio_item->nombre);
    strcat(retorno, (char*)&directorio_item->tamano);
    strcat(retorno, directorio_item->tipo);
    strcat(retorno, (char*)&directorio_item->padre);
    strcat(retorno, (char*)&directorio_item->primer_hijo);
    strcat(retorno, (char*)&directorio_item->hermano_derecho);
    strcat(retorno, (char*)&directorio_item->primer_bloque_data);
    strcat(retorno, (char*)&directorio_item->libre);
    return retorno;
}

void registro::from_char_block(char *input)
{
    strcpy(block_item->data, input);
}

void registro::from_char_directorio(char *input)
{
    string extract(input);
    strcpy(directorio_item->nombre, extract.substr(0, 30).c_str());
    directorio_item->tamano = atoi(extract.substr(30, 4).c_str());
    strcpy(directorio_item->tipo, extract.substr(34, 1).c_str());
    directorio_item->padre = atoi(extract.substr(35,4).c_str());
    directorio_item->primer_hijo = atoi(extract.substr(39,4).c_str());
    directorio_item->hermano_derecho = atoi(extract.substr(43,4).c_str());
    directorio_item->primer_bloque_data = atoi(extract.substr(47,4).c_str());
    directorio_item->libre = extract.substr(41, 1).c_str() != "0";
}

void registro::from_char_archivo(char *input)
{
    string extract(input);
    strcpy(archivo_item->nombre, extract.substr(0, 30).c_str());
    archivo_item->tamano = atoi(extract.substr(30, 4).c_str());
    strcpy(archivo_item->tipo, extract.substr(34, 1).c_str());
    archivo_item->padre = atoi(extract.substr(35,4).c_str());
    archivo_item->primer_hijo = atoi(extract.substr(39,4).c_str());
    archivo_item->hermano_derecho = atoi(extract.substr(43,4).c_str());
    archivo_item->primer_bloque_data = atoi(extract.substr(47,4).c_str());
    archivo_item->libre = extract.substr(41, 1).c_str() != "0";
}

void registro::leer_data_block(int pos)
{
    int offset = 1024*pos;
    archivo->set_pos(offset+posPrimerBlock);
    char* retorno = new char[DATA_BLOCK_SIZE];
    strcpy(retorno, archivo->leer(DATA_BLOCK_SIZE));
    from_char_block(retorno);
}

void registro::leer_directorio(int pos)
{
    int offset = FILE_ENTRY_SIZE*pos;
    archivo->abrir();
    archivo->set_pos(offset+posPrimerEntry);
    char* retorno = new char[FILE_ENTRY_SIZE];
    strcpy(retorno, archivo->leer(FILE_ENTRY_SIZE));
    from_char_directorio(retorno);
}

void registro::leer_archivo(int pos)
{
    int offset = FILE_ENTRY_SIZE*pos;
    archivo->set_pos(offset+posPrimerEntry);
    char* retorno = new char[FILE_ENTRY_SIZE];
    strcpy(retorno, archivo->leer(FILE_ENTRY_SIZE));
    from_char_archivo(retorno);
}

void registro::leer_archivo(string name)
{
    int inicioBuscar = posPrimerEntry;
    for(int i = 0; i < meta.entry_amount; i++)
    {
        archivo->set_pos(inicioBuscar + (i*FILE_ENTRY_SIZE));
        char* retorno = new char[FILE_ENTRY_SIZE];
        strcpy(retorno, archivo->leer(FILE_ENTRY_SIZE));
        char* nomb = new char[30];
        for(int m = 0; m < 30; m++)
        {
            nomb[m] = retorno[m];
        }
        if(name.c_str() == nomb)
        {
            from_char_archivo(retorno);
            return;
        }
    }
}

int registro::getPosDirectorioActual()
{
    return posDirectorioItem;
}

void registro::addNewArchivoToDir(int pos)
{

}

void registro::leerFirstBlockDataArchivo()
{

}

int registro::charToInt(char* x)
{
    int i = (x[3] << 24) | (x[2] << 16) | (x[1] << 8) | (x[0]);
    return i;
}