#ifndef E2_PROYECTO1_DATA_FILE_H
#define E2_PROYECTO1_DATA_FILE_H

#include <fstream>
#include <string>
#include <cstring>

using namespace std;

class data_file
{
public:
    data_file(char* direccion);
    void abrir();
    void cerrar();
    void escribir(char* input, int bytes);
    char* leer(int posicion, int bytes);

private:
    char* direccion;
    fstream* archivo;
};


#endif //E2_PROYECTO1_DATA_FILE_H
