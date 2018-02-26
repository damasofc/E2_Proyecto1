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
    metaDat.entry_amount = charToInt(archivo->leer(4,4));
    metaDat.block_size = charToInt(archivo->leer(8,4));
    //TODO: aca hay que chequiar no guarda bien el block_amount
    metaDat.block_amount = charToInt(archivo->leer(12,4));
    setMetaData(metaDat);
    this->posPrimerEntry = 16 + metaDat.bm_size;
    leer_directorio(0);
    this->posDirectorioItem = 0;
    this->posPrimerBlock = 16 + meta.bm_size + (FILE_ENTRY_SIZE*meta.entry_amount);

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
    archivo->escribir(to_char_block(),posPrimerBlock+offset, DATA_BLOCK_SIZE);
}

void registro::guardar_entry()
{
    int pos = getFirstEntryEmpty();
    int offset = pos*FILE_ENTRY_SIZE;
    archivo->escribir(to_char_directorio(),posPrimerEntry+offset, FILE_ENTRY_SIZE);
}

void registro::guardar_archivo()
{
    int pos = getFirstEntryEmpty();
    int offset = pos*FILE_ENTRY_SIZE;
    //this->to_char_archivo();
    archivo->escribir(to_char_archivo(),posPrimerEntry+offset, FILE_ENTRY_SIZE);
}

void registro::guardar_directorio()
{
    int pos = getFirstEntryEmpty();
    int offset = pos*FILE_ENTRY_SIZE;
    archivo->set_pos(posPrimerEntry+offset);
    archivo->escribir(to_char_directorio(),posPrimerEntry+offset, FILE_ENTRY_SIZE);
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
	return (byte |= (1 << position));	
}
char* registro::getBitMap()
{
    // this->abrir_archivo();
    int sizeBitmap = meta.bm_size;
    return this->archivo->leer(16,sizeBitmap);
}
int registro::getByteBlockEmpty()
{
    char* bitmap = getBitMap();
    int cont = 0;
    while(cont < meta.bm_size)
    {
        char* x = new char[1];
        x[0] = bitmap[cont];
        for(int i = 7; i >= 0; i--)
        {
            if(getBit(*x,i) == 0)
            {
                delete[] x;
                return cont;
            }
        }
        cont++;
        delete[] x;
    }
    //si llega a retornar -1, significa que ya no hay espacios vacios
    return -1;
}

int registro::getFirstEntryEmpty()
{
    int posBuscar = posPrimerEntry;
    for(int i = 0; i < meta.entry_amount; i++)
    {
        file_entry temp = getEntry(i);
        if(temp.libre == true)
        {
            return i;
        }
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
    strcat(retorno, (char*)&directorio_item->tamano);
    strcat(retorno, directorio_item->tipo);
    strcat(retorno, (char*)&directorio_item->padre);
    strcat(retorno, (char*)&directorio_item->primer_hijo);
    strcat(retorno, (char*)&directorio_item->hermano_derecho);
    strcat(retorno, (char*)&directorio_item->primer_bloque_data);
    strcat(retorno, (char*)&directorio_item->libre);
    return retorno;
}

char* registro::to_char_archivo()
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
    //return reinterpret_cast<char*> (this->archivo_item);
}

void registro::from_char_block(char *input)
{
    strcpy(block_item->data, input);
}

char* registro::readChar(int desde, int cantLeer,char* read)
{
    char* ans = new char[cantLeer];
    int pos = 0;
    for(int i = desde +1; i <= (desde + cantLeer); i++)
    {
        ans[pos] = read[i];
        pos++;
    }
    return ans;
    
}

void registro::from_char_directorio(char *input)
{
    string extract(input);
    strcpy(directorio_item->nombre, extract.substr(0, 30).c_str());
    directorio_item->tamano = charToInt(readChar(31,4,input));
    directorio_item->tipo[0] = readChar(35,1,input)[0];
    directorio_item->padre = charToInt(readChar(39,4,input));
    directorio_item->primer_hijo = charToInt(readChar(43,4,input));
    directorio_item->hermano_derecho = charToInt(readChar(47,4,input));
    directorio_item->primer_bloque_data = charToInt(readChar(51,4,input));
    directorio_item->libre = readChar(55,1,input)[0];
}

void registro::from_char_archivo(char *input)
{
    string extract(input);
    strcpy(this->archivo_item->nombre, extract.substr(0, 30).c_str());
    this->archivo_item->tamano = charToInt(readChar(31,4,input));
    this->archivo_item->tipo[0] = readChar(35,1,input)[0];
    this->archivo_item->padre = charToInt(readChar(39,4,input));
    this->archivo_item->primer_hijo = charToInt(readChar(43,4,input));
    this->archivo_item->hermano_derecho = charToInt(readChar(47,4,input));
    this->archivo_item->primer_bloque_data = charToInt(readChar(51,4,input));
    this->archivo_item->libre = readChar(55,1,input)[0];
}

void registro::leer_data_block(int pos)
{
    int offset = 1024*pos;
    char* retorno = new char[DATA_BLOCK_SIZE];
    strcpy(retorno, archivo->leer(offset+posPrimerBlock,DATA_BLOCK_SIZE));
    from_char_block(retorno);
}

void registro::leer_directorio(int pos)
{
    int offset = FILE_ENTRY_SIZE*pos;
    int posColocar = offset+this->posPrimerEntry;
    archivo->abrir();
    char* retorno = new char[FILE_ENTRY_SIZE];
    memcpy(retorno,archivo->leer(posColocar,FILE_ENTRY_SIZE),FILE_ENTRY_SIZE);
    //strcpy(retorno, archivo->leer(posColocar,FILE_ENTRY_SIZE));
    from_char_directorio(retorno);
    this->posDirectorioItem = pos;
}

void registro::listar_directorio()
{
    cout<<"\t"<<this->pathDirActual(*this->directorio_item)<<endl<<endl;
    file_entry temp = getEntry(this->directorio_item->primer_hijo);
    // mostrarEntry(temp);
    bool continua = true;
    cout<<"\tNombre\t\t\tTamano\t\t\tTipo"<<endl;
    if(this->directorio_item->primer_hijo == -1)
        return;
    while(continua)
    {
        cout<<"\t"<<temp.nombre<<"\t\t\t"<<temp.tamano<<"\t\t\t"<<temp.tipo<<endl;
        if(temp.hermano_derecho != -1)
        {
            temp = getEntry(temp.hermano_derecho);
        }
        else
        {
            continua = false;
        }
    }
}

string registro::pathDirActual(file_entry fil)
{
    string rem = "root/";
    if(this->directorio_item->padre == -1)
    {
        return rem;
    }
    return pathDirActual(getEntry(fil.padre))+"/"+fil.nombre;
}

file_entry registro::getEntry(int pos)
{
    int offset = FILE_ENTRY_SIZE*pos;
    int posColocar = offset+this->posPrimerEntry;
    archivo->abrir();
    char* retorno = new char[FILE_ENTRY_SIZE];
    memcpy(retorno,archivo->leer(posColocar,FILE_ENTRY_SIZE),FILE_ENTRY_SIZE);
    //----------------
    file_entry nuevo;
    string extract(retorno);
    strcpy(nuevo.nombre, extract.substr(0, 30).c_str());
    nuevo.tamano = charToInt(readChar(31,4,retorno));
    nuevo.tipo[0] = readChar(35,1,retorno)[0];
    nuevo.padre = charToInt(readChar(39,4,retorno));
    nuevo.primer_hijo = charToInt(readChar(43,4,retorno));
    nuevo.hermano_derecho = charToInt(readChar(47,4,retorno));
    nuevo.primer_bloque_data = charToInt(readChar(51,4,retorno));
    nuevo.libre = readChar(55,1,retorno)[0];
    return nuevo;
}

file_entry registro::getEntry(string name)
{
    file_entry ne;
    for(int i = 0; i < meta.entry_amount; i++)
    {
        file_entry f = getEntry(i);
        if(f.nombre == name)
        {
            return f;
        }
    }
    return ne;
    
}

void registro::leer_directorio(string name)
{
    int inicioBuscar = posPrimerEntry;
    for(int i = 0; i < meta.entry_amount; i++)
    {
        char* retorno = new char[FILE_ENTRY_SIZE];
        strcpy(retorno, archivo->leer(inicioBuscar + (i*FILE_ENTRY_SIZE),FILE_ENTRY_SIZE));
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

void registro::leer_archivo(int pos)
{
    int offset = FILE_ENTRY_SIZE*pos;
    int posColocar = offset+this->posPrimerEntry;
    archivo->abrir();
    char* retorno = new char[FILE_ENTRY_SIZE];
    memcpy(retorno,archivo->leer(posColocar,FILE_ENTRY_SIZE),FILE_ENTRY_SIZE);
    from_char_archivo(retorno);
    this->posArchivoActual = pos;
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
    if(this->directorio_item->primer_hijo == -1)
    {
        this->directorio_item->primer_hijo = pos;
        this->guardar_file_Entry(*this->directorio_item,posDirectorioItem);
    }
    else
    {
        file_entry temp = getEntry(this->directorio_item->primer_hijo);
        bool conti = true;
        int posTemp = this->directorio_item->primer_hijo;
        while(conti)
        {
            if(temp.hermano_derecho == -1)
            {
                temp.hermano_derecho = pos;
                guardar_file_Entry(temp,posTemp);
                conti = false;
                return;
            }
            temp = getEntry(temp.hermano_derecho);
            posTemp = temp.hermano_derecho;
        }
    }
}

void registro::leerFirstBlockDataArchivo()
{

}

int registro::charToInt(char* x)
{
    int i = (x[3] << 24) | (x[2] << 16) | (x[1] << 8) | (x[0]);
    return i;
}

void registro::crearDir(string name)
{
    file_entry nuevoDir;
    strcpy(nuevoDir.nombre,name.c_str());
    char tip[1];
    tip[0] = 'D';
    mempcpy(nuevoDir .tipo,tip,1);
    nuevoDir.padre = getPosDirectorioActual();
    nuevoDir.libre = false;
    if(this->directorio_item->primer_hijo == -1)
    {
        this->directorio_item->primer_hijo = getFirstEntryEmpty();
        guardar_file_Entry(*this->directorio_item,posDirectorioItem);
        guardar_file_Entry(nuevoDir,getFirstEntryEmpty());
        return;
    }
    file_entry temp = getEntry(this->directorio_item->primer_hijo);
    int posTemp = this->directorio_item->primer_hijo;
    bool conti = true;
    while(conti)
    {
        if(temp.hermano_derecho == -1)
        {
            temp.hermano_derecho = getFirstEntryEmpty();
            guardar_file_Entry(temp,posTemp);
            guardar_file_Entry(nuevoDir,getFirstEntryEmpty());
            conti = false;
            return;
        }
        temp = getEntry(temp.hermano_derecho);
        posTemp = temp.hermano_derecho;

    }


}

void registro::guardar_file_Entry(file_entry file,int pos)
{
    int offset = pos*FILE_ENTRY_SIZE;
    archivo->escribir(reinterpret_cast<char*>(&file),posPrimerEntry+offset, FILE_ENTRY_SIZE);
}

int registro::getFirstBlockEmpty()
{
    char* bitmap = getBitMap();
    int conte = 0;
    int a = 0;
    int v = -1;
    for(int i = 0; i < meta.bm_size; i++)
    {
        char* x = new char[1];
        x[0] = bitmap[i];
        for(int m = 7; m >= 0; m--)
        {
            if(getBit(x[0],m) == 0)
            {
                delete[] x;
                v = conte + a;
                return v;
                //return (conte*10)+ a;
                //return ((10*i)+a);
            }
            a++;
        }
        conte++;
        /* code */
        delete[] x;
    }
    
    //si llega a retornar -1, significa que ya no hay espacios vacios
    return v;
}

void registro::actualizar_block_data(data_block b,int pos)
{
    int offset = pos*DATA_BLOCK_SIZE;
    archivo->escribir(reinterpret_cast<char*>(&b),posPrimerBlock+offset,DATA_BLOCK_SIZE);
}

void registro::guardar_block_data(data_block block,int pos)
{
    int offset = pos*DATA_BLOCK_SIZE;
    archivo->escribir(reinterpret_cast<char*>(&block),posPrimerBlock+offset,DATA_BLOCK_SIZE);
    int byteOcupar = getByteBlockEmpty();
    char* temp = new char[1];
    temp[0] = *archivo->leer(16+byteOcupar,1);
    for(int i = 7; i >= 0; i--)
    {
        int bit = getBit(temp[0],i);
        if(bit == 0)
        {
            bit = turnBitOn(temp[0],i);
            archivo->escribir(reinterpret_cast<char*>(&bit),16+byteOcupar,1);
            //cout<<"despues"<<charToInt(archivo->leer(16+byteOcupar,1))<<endl;
            i = 0;
        }
    }
}

void registro::mostrarEntry(file_entry fil)
{
    cout << "Nombre: " <<fil.nombre << endl;
    cout << "Size: " <<fil.tamano << endl;
    cout << "Tipo: " <<fil.tipo << endl;
}

data_block registro::getDataBlock(int pos)
{
    int offset = DATA_BLOCK_SIZE*pos;
    int posColocar = offset+posPrimerBlock;
    archivo->abrir();
    char* retorno = new char[DATA_BLOCK_SIZE];
    data_block nuevo;
    strcpy(nuevo.data,archivo->leer(posColocar,1020));
    //memcpy(retorno,archivo->leer(posColocar,DATA_BLOCK_SIZE),DATA_BLOCK_SIZE);
    //----------------
    //memcpy(nuevo.data,retorno,1020);
    //memcpy(reinterpret_cast<char*> (nuevo.data),*readChar(0,1020,retorno),1020);
    nuevo.siguiente = charToInt(archivo->leer(posColocar+1020,4));
    return nuevo;
}
