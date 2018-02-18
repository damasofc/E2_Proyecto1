#include "data_file.h"

void data_file::abrir(char *direccion)
{
    this->archivo->open(direccion, ios::binary | ios::in | ios::out | ios::app);
}

void data_file::cerrar()
{
    this->archivo->close();
}

void data_file::escribir(char *input, int bytes)
{
    this->archivo->write(input, bytes);
}

char* data_file::leer(int posicion, int bytes)
{
    char* retorno = new char[bytes];
    this->archivo->seekg(posicion);
    this->archivo->read(retorno, bytes);
    return retorno;
}