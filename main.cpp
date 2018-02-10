#include <iostream>

 
using namespace std;

int menuPrincipal(){
    int opc;
    cout << "\nPROYECTO FILE SYSTEM\n"<<endl;
    cout <<"\t1)Crear Disco\n\t2)Utilizar Disco Existente\n\t3)Salir"<<endl;
    cout<<"\tEscriba su opcion: ";
    cin>>opc;
    switch (opc)
    {
        case 1:
            break;
        case 2:
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