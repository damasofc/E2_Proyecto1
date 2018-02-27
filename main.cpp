#include "functions.h"

 
using namespace std;

void menuDiscoExistente(string discName){
    int opc;
    char* nom = new char[sizeof(discName)];
    strcpy(nom,discName.c_str());
    registro* file = new registro(nom);
    do
    {
        cout<<"\n\tPROYECTO FILE SYSTEM"<<endl;
        cout<<"\tDisco << "<<discName<<" >>"<<endl;
        cout<<"\tDirectorio Actual: "<<file->pathDirActual(file->getEntry(file->getPosDirectorioActual()))<<endl;
        cout<<"\t\t1.Crear Directorio"<<endl;
        cout<<"\t\t2.Cambiar Directorio"<<endl;
        cout<<"\t\t3.Listar Directorio"<<endl;
        cout<<"\t\t4.Eliminar Directorio/Archivo"<<endl;
        cout<<"\t\t5.Importar Archivo"<<endl;
        cout<<"\t\t6.Exportar Archivo"<<endl;
        cout<<"\t\t7.Regresar"<<endl;
        cout<<"Ingrese su opcion: ";
        cin>>opc;
        string name,name2;
        switch(opc){
            case 1:
                cout<<"\t\tIngrese Nombre del nuevo Directorio: ";
                cin>>name;
                file->crearDir(name);
                cout<<"DIRECTORIO CREADO!!";
                break;
            case 2:
                cout<<"\t\tIngrese Nombre del Directorio: ";
                cin>>name;
                file->cambiarDirectorio(name);
                break;
            case 3:
                file->listar_directorio();
                break;
            case 4:
                break;
            case 5:
                cout<<"\t\tIngrese Nombre del archivo a Importar: ";
                cin>>name;
                importar_archivo(name,file);
                cout<<"Archivo importado"<<endl;
                file->mostrarEntry(file->getEntry(1));
                break;
            case 6:
                cout<<"\t\tIngrese Nombre del archivo a Exportar: ";
                cin>>name;
                cout<<"\t\tIngrese Nombre nuevo con el que va a Exportar: ";
                cin>>name2;
                exportarArchivo(*file,name," ",name2);
                cout<<"Archivo Exportado"<<endl;
                break;
            case 7:
                if(file->posDirectorioItem != 0)
                {
                    file->cambiarDirectorio(file->getEntry(file->directorio_item->padre).nombre);
                    opc = 1;
                }
                break;
                
        }
        /* code */
    } while (opc!= 7);
    
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
            createDisc(nombreDisco,cantEntradas,cantBloques);
            cout<<"\t\tDisco Creado!!"<<endl;
            break;
        case 2:
            cout<<"\t\tIngrese Nombre del Disco: ";
            cin>>nombreDisco;
            //aca se comprobara si existe el disco o no para abrirlo
            if(existFile(nombreDisco.c_str())){
                menuDiscoExistente(nombreDisco);
            }
            //aca se debe mostrar un mensaje si no existe ese disco
            
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
    //createDisc("part2",100,1000);
    // createDisc("part2",100,1000);
    // string name = "part2.dat";
    // char* nom = new char[sizeof(name)];
    // strcpy(nom,name.c_str());
    // registro *part2 = new registro(nom);
    // // //part2->crearDir("usr");
    // part2->listar_directorio();
    // // //cout<<part2.getFirstBlockEmpty()<<endl;
    // // //part2.mostrar_archivo();    // // cout<<part2.getFirstEntryEmpty()<<endl;
    // //importar_archivo("infinito.png",part2);
    // //importar_archivo("ISAIAS_26_BGC.doc",part2);
    // //exportarArchivo(*part2,"ISAIAS_26_BGC.doc"," ","can.doc");
    // exportarArchivo(*part2,"infinito.png"," ","infi.png");
    //part2->listar_directorio();
    //part2->mostrarEntry(part2->getEntry(1));
    //part2->listar_directorio();
    //cout<<part2.getFirstEntryEmpty()<<endl;
    
    

    // while (!discDat.eof() && continuar && sizeTotal >= 1020)
	// 	{
	// 		discDat.seekg(bloqueInitial);
	// 		discDat.read(reinterpret_cast<char*>(&leyendo), sizeof(DATA_BLOCK));
	// 		update.write(leyendo.DATA, 1020);

	// 		if (leyendo.siguiente == -1)
	// 		{
	// 			continuar = false;
	// 		}
	// 		else {
	// 			bloqueInitial = leyendo.siguiente;
	// 		}
	// 		sizeTotal -= 1020;
	// 	}
	// 	discDat.seekg(bloqueInitial);
	// 	discDat.read(reinterpret_cast<char*>(&leyendo), sizeof(DATA_BLOCK));
	// 	update.write(leyendo.DATA, sizeTotal);
	// 	update.close();
	// 	return true;
	// }

            
    return 0;
}
