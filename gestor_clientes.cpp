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
void realizarTransaccion(Transaccion &transaccion, Cliente &cliente, string username, string clave);
bool buscarCliente(Cliente &cliente, string username, string clave);
void registrarTransaccion (Transaccion &transaccion, Cliente &cliente, string username, int monto, string tipoTransaccion, int fecha);

// Analista de Transacciones
void mostrarMontoMaximo(Transaccion &transaccion, Cliente &cliente);
void eliminarTransaccion(Transaccion &transaccion);
void listarTransacciones(Transaccion &transaccion, string username);


void registrarCliente(Cliente &cliente) {
    string nombre, username, clave, dni;
    
    cout << endl << "============== Registrar nuevo cliente =============" << endl;
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
		cout << "====================================================" << endl;
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
			cout << "====================================================" << endl;
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
			} else {
				fclose(archivoClientes);
				return false;
			}	
		}
	}
	return false;
}

int main(){
	int opcion, opcionAnalista, opcionCliente;
	Cliente cliente;
	Transaccion transaccion;
	cout<<"- - - BIENVENIDO AL SISTEMA BANCARIO - - -" << endl;
	
	do {
		cout << endl << "========================";
		cout << "\n1. Acceso como GESTOR\n";
		cout << "2. Acceso como ANALISTA\n";
		cout << "3. Acceso como CLIENTE\n";
		cout << "4. Salir\n";
		cout << "Seleccione una opcion: ";
		cin >> opcion;
		cout << "========================" << endl;
		switch(opcion){
			case 1:{
				string contraGestor; 
				cout << endl << "Ingrese clave de GESTOR: ";
				cin >> contraGestor;
				
				if (contraGestor == "gestor123"){
					registrarCliente(cliente);
				} else {
					cout << "Clave incorrecta" << endl;
				}
				break;
			}
			case 2:{
				string contraAnalista;
				cout << endl << "Ingrese clave de ANALISTA: ";
				cin >> contraAnalista;
				
				if (contraAnalista == "analista123"){
					do {
					cout << endl << "====================== Ingreso como ANALISTA ======================";
					cout << "\n1. Listar transacciones de un cliente\n";
					cout << "2. Listar cantidad de ingresos y egreso por mes de un cliente\n";
					cout << "3. Listar transaccion de mayor monto de cada cliente\n";
					cout << "4. Mostrar el cliente que mas ingresos tuvo en los ultimos 30 dias\n";
					cout << "5. Volver al inicio\n";
					cout << "Seleccione una opcion: ";
					cin >> opcionAnalista;
					cout << "===================================================================" << endl;
					
					switch (opcionAnalista){
						case 1:{
							string username;
							cout << "Ingrese username: ";
							cin >> username;
							listarTransacciones(transaccion, username);
							break;
						}
						case 2:
							// Funcion listarIngresosYEgresos
							break;
						case 3:
							mostrarMontoMaximo(transaccion, cliente);
							break;
						case 4:
							// Funcion mostrarIngresos30dias
							break;
						case 5:
							cout << endl << "========================";
            				cout << endl << "Volviendo al inicio..." << endl;
            				cout << "========================" << endl;
            				break;
						default:
							cout << endl << "Opcion invalida" << endl;
							break;
						}
					} while (opcionAnalista != 5);
				} else {
					cout << "Clave incorrecta" << endl;
				}
				break;
			}
			case 3:{
				string username, clave;
				cout << endl << "Ingrese su username: ";
				cin >> username;
				cout << "Ingrese su clave: ";
				cin >> clave;
				
				if (!buscarCliente(cliente, username, clave)) {
				cout << endl << "Username o clave incorrectos.\n";
				} else {
					do {
						cout << "\n====== Bienvenido/a " << cliente.nombre << ". Saldo actual: $" << cliente.saldo << " ======" << endl;
						cout << "1. Realizar transaccion" << endl;
						cout << "2. Eliminar transaccion" << endl;
						cout << "3. Volver al inicio" << endl;
						cout << "Seleccione una opcion: ";
						cin >> opcionCliente;
						cout << "======================================================" << endl;
					
						switch (opcionCliente){
							case 1:{
								realizarTransaccion(transaccion, cliente, username, clave);
								break;
							}
							case 2:{
								eliminarTransaccion(transaccion);
								break;
							}
							case 3:
								cout << endl << "========================";
            					cout << endl << "Volviendo al inicio..." << endl;
            					cout << "========================" << endl;
            					break;
							default:
								cout << endl << "Opcion invalida" << endl;
								break;
						}	
					} while (opcionCliente != 3);
				}
				break;
			}
			case 4:
				cout << endl << "========================";
            	cout << endl << "Saliendo del sistema..." << endl;
            	cout << "========================" << endl;
				break;
			default:
				cout << endl << "Opcion invalida" << endl;
				break;
		}
		
	} while (opcion !=4);
	
	return 0;
}
