#include <iostream>

 
using namespace std;

int menuDiscoExistente(string discName){
    cout<<"\n\tPROYECTO FILE SYSTEM"<<endl;
    cout<<"\tDisco << "<<discName<<" >>"<<endl;

    return 1;
}
int menuPrincipal(){
    int opc,opcDiscoExistente;
    cout << "\nPROYECTO FILE SYSTEM\n"<<endl;
    cout <<"\t1)Crear Disco\n\t2)Utilizar Disco Existente\n\t3)Salir"<<endl;
    cout<<"\tEscriba su opcion: ";
    cin>>opc;
    int cantEntradas,cantBloques;
    string nombreDisco;
    switch (opc)
    {
        case 1:
            cout<<"\nPROYECTO FILE SYSTEM"<<endl;
            cout<<"\t\tCrear Disco"<<endl;
            cout<<"\t\tIngrese Cantidad de Entradas: ";
            cin>>cantEntradas;
            cout<<"\n\t\tIngrese Cantidad de Bloques: ";
            cin>>cantBloques;
            break;
        case 2:
            do
            {
                cout<<"\t\tIngrese Nombre del Disco: ";
                cin>>nombreDisco;
                opcDiscoExistente = menuDiscoExistente(nombreDisco);
            } while (opcDiscoExistente != 7);
            
            break;
    }
    return opc;
}
int main()
{
    int i;
    do
    {
        i = menuPrincipal();

    } while(i!=3);

            
    return 0;
}