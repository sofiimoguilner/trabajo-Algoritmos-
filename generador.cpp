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


// Declaracion de funciones
void realizarTransaccion(Transaccion &transaccion, Cliente &cliente);
Cliente buscarCliente(Cliente &cliente, string username, string clave);
void registrarTransaccion (Transaccion &transaccion, Cliente &cliente, string username, int monto, string tipoTransaccion, int fecha);


// Realizacion completa de la transaccion
void realizarTransaccion(Transaccion &transaccion, Cliente &cliente){
	string username, clave;
	cout << "Ingrese su username: ";
	cin >> username;
	
	cout << "Ingrese su clave: ";
	cin >> clave;
	
	// Buscar si el cliente ingresado está en el sistema
	cliente = buscarCliente(cliente, username, clave);
	if (cliente.saldo == -1) {
		cout << "Username o clave incorrectos.\n";
	} else {
		cout << "\nBienvenido/a " << cliente.nombre << ". Saldo actual: $" << cliente.saldo << endl;
		
		// Si el cliente se encuentra en el sistema, se procede con la transaccion
		int monto, fecha;
		string tipoTransaccion;
		cout << "Ingrese monto: ";
		cin >> monto;
		cout << "Escriba tipo de transaccion: EGRESO o INGRESO: ";
		cin >> tipoTransaccion;
		cout << "Ingrese fecha (aaaa/mm/dd): ";
		cin >> fecha;		
		
		if (tipoTransaccion != "EGRESO" && tipoTransaccion != "INGRESO"){
			cout << "Error: tipo de transaccion incorrecto.\n";
		} else {
			// Funcion para registrar correctamente la transaccion en el sistema
			registrarTransaccion(transaccion, cliente, username, monto, tipoTransaccion, fecha);
			cout << "Transaccion realizada con exito. Saldo actual: $" << cliente.saldo << endl;
		}
	}
}

// Funcion para buscar si el cliente ingresado se encuentra en el sistema
Cliente buscarCliente(Cliente &cliente, string username, string clave){
	FILE* archivoClientes = fopen("Clientes.txt", "r");
	
	while (fread(&cliente, sizeof(Cliente), 1, archivoClientes)){
		if ((cliente.username == username) && (cliente.clave == clave)){
			fclose(archivoClientes);
			return cliente;
		}
	}
	
	fclose(archivoClientes);
	Cliente clienteVacio = {"","","","", -1};
	return clienteVacio;
}


// Funcion para registrar correctamente la transaccion dentro del sistema
void registrarTransaccion(Transaccion &transaccion, Cliente &cliente, string username, int monto, string tipoTransaccion, int fecha){
	int ultimoID=0;
	FILE* archivoTransaccionesLectura = fopen("Transacciones.txt", "rb");
	
	// Encontrar el ultimo ID registrado
	while (fread(&transaccion, sizeof(Transaccion), 1, archivoTransaccionesLectura)){
		ultimoID = transaccion.id;
	}
	fclose(archivoTransaccionesLectura);
	
	// Registar la transaccion en el archivo "Transacciones.txt"
	transaccion.id = ++ultimoID;
	transaccion.username = username;
	transaccion.monto = monto;
	transaccion.tipo = tipoTransaccion;
	transaccion.fecha = fecha;
	
	FILE* archivoTransacciones = fopen("Transacciones.txt", "ab");
	fwrite(&transaccion, sizeof(Transaccion), 1, archivoTransacciones);
	fclose(archivoTransacciones);
	
	// Actualizar saldo del cliente
	FILE* archivoClientes = fopen("Clientes.txt", "rb+");
	if (archivoClientes != NULL){
		while (fread(&cliente, sizeof(Cliente), 1, archivoClientes)){
			if (cliente.username == username){
				fseek(archivoClientes, -sizeof(Cliente), SEEK_CUR);
				if (tipoTransaccion == "INGRESO"){
					cliente.saldo += monto;
				} else if (tipoTransaccion == "EGRESO"){
					cliente.saldo -= monto;
				}
				fwrite(&cliente, sizeof(Cliente), 1, archivoClientes);
				break;
			}
		}
		fclose(archivoClientes);
	}
}

