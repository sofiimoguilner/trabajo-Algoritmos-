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


//Gestor de Clientes.
void registrarCliente(Cliente &cliente);
bool existeCliente(Cliente &cliente, string dni, string username);

// Generador Transacciones
void realizarTransaccion(Transaccion &transaccion, Cliente &cliente);
Cliente buscarCliente(Cliente &cliente, string username, string clave);
void registrarTransaccion (Transaccion &transaccion, Cliente &cliente, string username, int monto, string tipoTransaccion, int fecha);

// Analista de Transacciones
void listarTransacciones(Transaccion &transaccion, string username);
void EliminarTransaccion(Tansaccion &transaccion, string username, string clave);

void registrarCliente(Cliente &cliente) {
    string nombre, username, clave, dni;
	
    cout << "Ingrese nombre: ";
    cin >> nombre;
    cout << "Ingrese DNI: ";
    cin >> dni;
    cout << "Ingrese username: ";
	cin>> username;
    cout << "Ingrese clave: ";
	cin>> clave;
    
    if (existeCliente(cliente, dni, username)){
    	cout<<"Error: Ya existe un cliente con ese DNI o username.\n";
	} else {
		
		cliente.nombre = nombre;
		cliente.dni = dni;
		cliente.username = username;
		cliente.clave = clave;	
		cliente.saldo = 10000;
	
		FILE* archivoClientes = fopen("Clientes.txt", "a");
		if (archivoClientes != NULL){
			fwrite(&cliente, sizeof(Cliente), 1, archivoClientes);
			fclose(archivoClientes);
	
			cout << "Registro exitoso. Saldo inicial asignado: $" << cliente.saldo << endl;
		}		
	}
}

bool existeCliente(Cliente &cliente, string dni, string username){
	FILE* archivoClientes = fopen("Clientes.txt", "rb");
	
	if(archivoClientes !=NULL){
		while(fread(&cliente, sizeof(Cliente), 1, archivoClientes)){
			if(cliente.dni == dni || cliente.username == username){
				fclose(archivoClientes);
				return true;
			}	
		}
	}
	fclose(archivoClientes);
	return false;
}

int main(){
	int opcion;
	Cliente cliente;
	Transaccion transaccion;
	cout<<"- - - BIENVENIDO AL SISTEMA BANCARIO - - -" << endl;
	
	do {
		cout << "\n1. Registrar nuevo cliente\n";
		cout << "2. Realizar transaccion (Cliente)\n";
		cout << "3. Listar transacciones de un cliente\n";
		cout << "4. Eliminar transaccion\n";
		cout << "5. Salir\n";
		cout << "Seleccione una opcion: ";
		cin >> opcion;
		
		switch (opcion){
			case 1:
				registrarCliente(cliente);
				break;
			case 2:
				realizarTransaccion(transaccion, cliente);
				break;
			case 3:{
				string username;
				cout << "Ingrese username: ";
				cin >> username;
				listarTransacciones(transaccion, username);
				break;
			}
			case 4: {string username;
				cout << "Ingrese username: ";
				cin >> username;
				cout << "Ingrese clave: ";
				cin >> clave;
				EliminarTransaccion(transaccion,username,clave);
			case 5:
				cout << endl << "========================";
            	cout << endl << "Saliendo del sistema..." << endl;
            	cout << "========================";
				break;
			default:
				cout << "Opcion invalida\n";
				break;
		}
		
	} while (opcion !=4);
	
	return 0;
}
