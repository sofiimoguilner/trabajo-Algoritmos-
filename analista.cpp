#include <iostream>
#include <string>
#include <cstdio>
#include <fstream>

using namespace std;

struct Cliente {
    string nombre;
    string dni;
    string username;
    string clave;
    int saldo;
};

struct Transaccion {
    int id;
    string username;
    int fecha; // formato "aaaa/mm/dd"
    int monto;
    string tipo; // Egreso o Ingreso
};


// Generador de Transacciones
bool buscarCliente(Cliente &cliente, string username, string clave);

// Declaracion de funciones
void listarTransacciones(Transaccion &transaccion, string username);
void mostrarMontoMaximo(Transaccion &transaccion);


void Listado5(Transaccion &t, Cliente &cliente, string username, string clave);
void ordenar(Transaccion transaccion[], int cont);
void DeA5(Transaccion transaccion[],int cont, string username);

void listarTransacciones(Transaccion &transaccion, string username){
    FILE* archivoTransacciones = fopen("Transacciones.txt", "rb");
	
    bool hayTransacciones = false;
    cout << "\n- - - Transacciones de " << username << " - - -\n";
    
    while (fread(&transaccion, sizeof(Transaccion), 1, archivoTransacciones)) {
        if (transaccion.username == username) {
            hayTransacciones = true;
            cout << "ID: " << transaccion.id
				 << ", Fecha: " << transaccion.fecha 
				 << ", Monto: $" << transaccion.monto
				 << ", Tipo: " << transaccion.tipo << endl;
        }
    }
	
    if (!hayTransacciones) {
        cout << "No se encontraron transacciones para este usuario.\n";
    }
	
    fclose(archivoTransacciones);
}

void mostrarMontoMaximo(Transaccion &transaccion){
	FILE* archivoTransacciones = fopen("Transacciones.txt", "rb");
	
	Transaccion max[10]; // Array para almacenar el máximo por cada cliente
	int contador=0;
	
	while(fread(&transaccion, sizeof(Transaccion), 1, archivoTransacciones)){
		bool usuarioEncontrado = false;
		
		// Verificar si ya hemos encontrado a este usuario antes
		for (int i=0; i<contador; i++){
			if (max[i].username == transaccion.username){
                usuarioEncontrado = true;
                
                // Si el monto actual es mayor al almanecado, lo actualizamos
				if (transaccion.monto > max[i].monto){
				max[i].monto = transaccion.monto;
				max[i].fecha = transaccion.fecha;
				}
				break; // Salimos del bucle una vez encontrado al cliente
			}
		}
		
		// Si no encontramos al cliente, lo agregamos al array
		if (!usuarioEncontrado && contador < 10){
			max[contador].username = transaccion.username;
			max[contador].fecha = transaccion.fecha;
			max[contador].monto = transaccion.monto;
			contador++;
		}
	}
	
	// Mostrar Transacciones Máximas
	for (int i=0; i < contador; i++){
		cout << endl << "========================" << endl;
		cout << "Cliente: " << max[i].username << endl;
		cout << "Fecha: " << max[i].fecha << endl;
		cout << "Monto: " << max[i].monto << endl;
		cout << "========================" << endl;
	}
	
	fclose(archivoTransacciones);
}


void Listado5(Transaccion &t, Cliente &cliente, string username, string clave)
{
    buscarCliente(cliente, username, clave); //verificar existencia
    
    FILE* archivoTransacciones = fopen("Transacciones.txt", "rb");
    

	int contadorTransacciones = 0;
	while(fread(&t, sizeof(Transaccion), 1, archivoTransacciones)){
		contadorTransacciones++;
	}
	fclose(archivoTransacciones);

	
	Transaccion transaccion[contadorTransacciones];
	int cont = 0;

    while(fread(&t, sizeof(Transaccion), 1, archivoTransacciones))
    {
        if(t.username == username)
		{
			transaccion[cont]=t;
			cont++;
		}
		ordenar(transaccion,cont);
		DeA5(transaccion,cont,username);

		

    }

}

//ordeno por fecha
void ordenar(Transaccion transaccion[], int cont)
{
	Transaccion t;
	for(int i=0; i<cont; i++)
	{
		for(int j=0; j<cont-1; j++)
		{
			if(transaccion[j].fecha<transaccion[j+1].fecha)
			{
				t = transaccion[j];
				transaccion[j]=transaccion[j+1];
				transaccion[j+1]=t;
			}

		}

	}
}

//muestro de a 5
void DeA5(Transaccion transaccion[],int cont, string username)
{
	int desde = 0, pag=1;

	while(desde < cont)
	{
		cout<<"Transacciones del cliente "<<username<<endl;
		cout<<"Pagina "<<pag;
		for(int i=0; i<desde+5 ; i++)
		{
			cout<<i+1<<". ID "<<transaccion[i].id<<"- Fecha "<<transaccion[i].fecha
			<<"- Monto "<<transaccion[i].monto<<"- Tipo "<<transaccion[i].tipo<<endl;
		}
		char rta;
		cout<<"Desea ver la siguente pagina? (S/N)";
		cin>>rta;
		if( rta != 'S' && rta!= 's')
		{
			break;
		}
		desde+=5;
		pag++;

	}

