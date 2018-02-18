#ifndef E2_PROYECTO1_DATA_FILE_H
#define E2_PROYECTO1_DATA_FILE_H

#include <fstream>

using namespace std;

class data_file
{
public:
    void abrir(char* direccion);
    void cerrar();
    void escribir(char* input, int bytes);
    char* leer(int posicion, int bytes);

private:
    char* direccion;
    fstream* archivo;
};


#endif //E2_PROYECTO1_DATA_FILE_H
