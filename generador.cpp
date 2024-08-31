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
bool buscarCliente(Cliente &cliente, string username, string clave);
void registrarTransaccion (Transaccion &transaccion, Cliente &cliente, string username, int monto, string tipoTransaccion, int fecha);
void eliminarTransaccion(Transaccion &transaccion);


// Realizacion completa de la transaccion
void realizarTransaccion(Transaccion &transaccion, Cliente &cliente){
	string username, clave;
	
	cout << endl << "========================" << endl;
	cout << "Ingrese su username: ";
	cin >> username;
	
	cout << "Ingrese su clave: ";
	cin >> clave;
	
	// Buscar si el cliente ingresado está en el sistema
	//cliente = buscarCliente(cliente, username, clave);
	if (!buscarCliente(cliente, username, clave)) {
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
			cout << "========================" << endl;
		}
	}
}

// Funcion para buscar si el cliente ingresado se encuentra en el sistema
bool buscarCliente(Cliente &cliente, string username, string clave){
	FILE* archivoClientes = fopen("Clientes.txt", "r");
	
	while (fread(&cliente, sizeof(Cliente), 1, archivoClientes)){
		if ((cliente.username == username) && (cliente.clave == clave)){
			fclose(archivoClientes);
			return true;
		}
	}
	
	fclose(archivoClientes);
	return false;
}

// Funcion para registrar correctamente la transaccion dentro del sistema
void registrarTransaccion(Transaccion &transaccion, Cliente &cliente, string username, int monto, string tipoTransaccion, int fecha){
	int ultimoID=0;
	// Abrimos el archivo "Transacciones.txt" en modo lectura para encontrar el ultimo ID registrado
	FILE* archivoTransaccionesLectura = fopen("Transacciones.txt", "rb");
	while (fread(&transaccion, sizeof(Transaccion), 1, archivoTransaccionesLectura)){
		ultimoID = transaccion.id;
	}
	fclose(archivoTransaccionesLectura);
	
	// Registramos los datos en la variable "transaccion"
	transaccion.id = ++ultimoID;
	transaccion.username = username;
	transaccion.monto = monto;
	transaccion.tipo = tipoTransaccion;
	transaccion.fecha = fecha;
	
	// Abrimos el archivo "Transacciones.txt" en modo adjuntar para añadir los datos en el sistema
	FILE* archivoTransacciones = fopen("Transacciones.txt", "ab");
	fwrite(&transaccion, sizeof(Transaccion), 1, archivoTransacciones);
	fclose(archivoTransacciones);
	
	// Abrimos el archivo "Clientes.txt" en modo lectura/escritura para actualizar el saldo del cliente
	FILE* archivoClientes = fopen("Clientes.txt", "rb+");
	if (archivoClientes != NULL){
		while (fread(&cliente, sizeof(Cliente), 1, archivoClientes)){
			if (cliente.username == username){
				// Movemos el puntero hacia atrás en cantidad del struct Cliente, para escribir sobre ese mismo cliente, y no el que le sigue
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

// Funcion para eliminar una transaccion por ID
void eliminarTransaccion(Transaccion &transaccion){
	// Abrimos dos archivos de Transacciones, una temporal (copia), y otra la original
	FILE* archivoTransacciones = fopen("Transacciones.txt","rb");
	FILE* archivoTemporal = fopen("TransaccionesTemp.txt", "wb");
	
	bool encontrado = false;
	int id;
	cout << "Ingrese ID de la transaccion a eliminar: ";
	cin >> id;
	
	while (fread(&transaccion, sizeof(Transaccion), 1, archivoTransacciones)){
		// Si no se encuentra el ID, se van copiando las transacciones en una copia
		if (transaccion.id != id){
			fwrite(&transaccion, sizeof(Transaccion), 1 , archivoTemporal);
		} else {
			// Una vez encontrado se cambia la variable "encontrado" a true. Pero sigue copiando las transacciones
			encontrado = true;
		}
	}
	
	fclose(archivoTransacciones);
	fclose(archivoTemporal);
	
	if (encontrado){
		// Si se encuentra el ID se elimina el archivo original de Transacciones, y se archiva la copia como la actual original
		remove("Transacciones.txt");
		rename("TransaccionesTemp.txt", "Transacciones.txt");
		cout << "Transaccion eliminada exitosamente." << endl;
	} else {
		// Si no se encuentra el ID, se elimina la copia de las transacciones, y se mantiene el archivo original
		remove("TransaccionesTemp.txt");
		cout << "Transaccion con ID " << id << " no encontrada. " << endl;
	}
}
