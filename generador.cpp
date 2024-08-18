#include <iostream>
#include <string>
#include <cstdio>
#include <fstream>

using namespace std;

struct Transaccion {
	double monto;
	int fecha;
	string id;
	string tipo;//egreso o ingreso
};

struct Cliente {
    string nombre;
    string dni;
    string username;
    string clave;
    double saldo;
    Transaccion transacciones;
};

bool iniciarSesion(Cliente &clientes){
	string username, clave;
	
	cout<< "Username: ";
	cin>> username;
	
	cout<< "clave: ";
	cin>> clave;
	
	FILE* archivo = fopen("Clientes.txt", "rb");
	if(archivo !=NULL){
		while(fread(&clientes, sizeof(Cliente), 1, archivo)==1){
			if(clientes.username== username && clientes.clave== clave){
				return true;
			}	
		}
	}
	return false;
}
/*
void agregarTransaccion(Cliente &clientes){
	Transaccion transacciones;
	int opcion, fecha;
	
	cout << "Ingrese monto: ";
	cin >> transacciones.monto;
	
	cout << "1. Egreso de dinero\n2. Ingreso de dinero\nEscriba una opcion: ";
	cin >> opcion;
	
	cout << "Ingrese fecha (ddmmaaa): ";
	cin >> transacciones.fecha;
	
	FILE* archivo = fopen("Clientes.txt", "a");
	if(archivo != NULL){
		fwrite(&transacciones, sizeof(Cliente), 1, archivo);
		fclose(archivo);
	}
	
}
*/
