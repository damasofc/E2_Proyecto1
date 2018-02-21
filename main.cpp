#include "functions.h"

 
using namespace std;

int menuDiscoExistente(string discName,string directorioName = "root"){
    int opc;
    cout<<"\n\tPROYECTO FILE SYSTEM"<<endl;
    cout<<"\tDisco << "<<discName<<" >>"<<endl;
    cout<<"\tDirectorio Actual: /"<<directorioName<<endl;
    cout<<"\t\t1.Crear Directorio"<<endl;
    cout<<"\t\t2.Cambiar Directorio"<<endl;
    cout<<"\t\t3.Listar Directorio"<<endl;
    cout<<"\t\t4.Eliminar Directorio/Archivo"<<endl;
    cout<<"\t\t5.Importar Archivo"<<endl;
    cout<<"\t\t6.Exportar Archivo"<<endl;
    cout<<"\t\t7.Regresar"<<endl;
    cout<<"Ingrese su opcion: ";
    cin>>opc;
    switch(opc){
        case 1:
            break;
        case 2:
            break;
        case 3:
            break;
        case 4:
            break;
        case 5:
            break;
        case 6:
            break;
        default:
            return 7;
    }
    return -1;
}
int menuPrincipal(){
    int opc,opcDiscoExistente;
    cout << "\nPROYECTO FILE SYSTEM\n"<<endl;
    cout <<"\t1)Crear Disco\n\t2)Utilizar Disco Existente\n\t3)Salir"<<endl;
    cout<<"\tEscriba su opcion: ";
    cin>>opc;
    int cantEntradas,cantBloques;
    //esta variable se usara para ambos casos, para crear el disco y asi mismo para abrirlo
    string nombreDisco;
    switch (opc)
    {
        case 1:
            cout<<"\nPROYECTO FILE SYSTEM"<<endl;
            cout<<"\t\tCrear Disco"<<endl;
            cout<<"\t\tIngrese Nombre del Disco: ";
            cin>>nombreDisco;
            cout<<"\t\tIngrese Cantidad de Entradas: ";
            cin>>cantEntradas;
            cout<<"\n\t\tIngrese Cantidad de Bloques: ";
            cin>>cantBloques;
            //Aca se llamara la funcion crear Disco
            break;
        case 2:
            cout<<"\t\tIngrese Nombre del Disco: ";
            cin>>nombreDisco;
            //aca se comprobara si existe el disco o no para abrirlo
            if(true){
                do
                {
                    opcDiscoExistente = menuDiscoExistente(nombreDisco);
                } while (opcDiscoExistente != 7);
            }
            //aca se debe mostrar un mensaje si no existe ese disco
            
            break;
    }
    return opc;
}
int main()
{
    // int i;
    // do
    // {
    //     i = menuPrincipal();

    // } while(i!=3);s
    //createDisc("part1",100,200);
    importar_archivo("/home/damasofc/Imágenes/contextMenu.png");
    
    

            
    return 0;
}
