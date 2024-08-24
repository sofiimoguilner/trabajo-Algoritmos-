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
void listarTransacciones(Transaccion &transaccion, string username);

void listarTransacciones(Transaccion &transaccion, string username){
    FILE* archivoTransacciones = fopen("Transacciones.txt", "rb");

    bool hayTransacciones = false;
    cout << "\n- - - Transacciones de " << username << " - - -\n";
    
    while (fread(&transaccion, sizeof(Transaccion), 1, archivoTransacciones)) {
        if (transaccion.username == username) {
            hayTransacciones = true;
            cout << "ID: " << transaccion.id << ", Fecha: " << transaccion.fecha << ", Monto: $" << transaccion.monto << ", Tipo: " << transaccion.tipo << endl;
        }
    }

    if (!hayTransacciones) {
        cout << "No se encontraron transacciones para este usuario.\n";
    }
	
    fclose(archivoTransacciones);
}
