#include "data_file.h"

data_file::data_file(char* direccion)
{
    strcpy(this->direccion, direccion);
}

void data_file::abrir()
{
    this->archivo->open(this->direccion, ios::binary | ios::in | ios::out | ios::app);
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

char* data_file::leer(int bytes)
{
    char* retorno = new char[bytes];
    this->archivo->read(retorno, bytes);
    return retorno;
}

void data_file::set_pos(int pos)
{
    this->archivo->seekg(pos);
}