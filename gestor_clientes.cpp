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
    double saldo;
};

struct Transaccion {
	string monto;
	string fecha;
	string id;
	string tipo;//egreso o ingreso
	Cliente datosCliente;
};

bool iniciarSesion(Cliente &clientes);

bool existeCliente(Cliente &clientes, string &dni, string &username){
	FILE* archivo = fopen("Clientes.txt", "rb");
	if(archivo !=NULL){
		while(fread(&clientes, sizeof(Cliente), 1, archivo)==1){
			if(clientes.dni== dni || clientes.username== username){
				return true;
			}	
		}
	}
	return false;
}

void registrarCliente(Cliente &clientes) {
	string nombre, username, clave, dni;
	
    cout << "Ingrese nombre: ";
    cin >> nombre;

    cout << "Ingrese DNI: ";
    cin >> dni;

    cout << "Ingrese username: ";
	cin>> username;

    cout << "Ingrese clave: ";
	cin>> clave;
	
	if(existeCliente(clientes, dni, username)){
		cout<<"Error: Ya existe un cliente con ese DNI o username.\n";
	}
	else{
		clientes.nombre= nombre;
		clientes.dni= dni;
		clientes.username= username;
		clientes.clave= clave;
		clientes.saldo= 10000.0;
		
		FILE* archivo = fopen("Clientes.txt", "a");
		if (archivo != NULL) {
			fwrite(&clientes, sizeof(Cliente), 1, archivo);
      	 	fclose(archivo);
       		cout << "Registro exitoso. Saldo inicial asignado: $10.000.\n" << endl;
		}
	}
}

//void agregarTransaccion(Transaccion &transaccion);

int main() {
    int opcion, opcionCliente;
    Cliente clientes;
    Transaccion transaccion;
    do {
        cout << "\n1. Registrar nuevo cliente\n2. Iniciar sesion\n3. Salir\nSeleccione una opcion: ";
        cin >> opcion;
		
        switch (opcion) {
            case 1:
                registrarCliente(clientes);
                break;
            case 2:
            	if(iniciarSesion(clientes)){
            		cout<< "\nBienvenido/a "<< clientes.nombre << "\nSu saldo es de: $"<<clientes.saldo;
            		do{
            			cout << "\n\n1. Agregar transaccion de ingreso o egreso de dinero\n2. Eliminar transaccion\n3. Salir\nSleccione una opcion:";
						cin >> opcionCliente;
						
						switch (opcionCliente){
							case 1:
								//agregarTransaccion(transaccion);
								break;
							case 2:
								cout << "a";
								break;
							case 3:
								cout << "Saliendo..." << endl;
								break;
							default:
								cout << "Opcion no valida." << endl;
								break;
						}
					}while(opcionCliente !=3);
				}else{
					cout<< "Error: datos incorrectos.";
				}
                break;
            case 3:
            	cout<< "Saliendo del sistema..." << endl;
            	break;
            default:
                cout << "Opcion no valida." << endl;
                break;
        }
    } while (opcion != 3);
	
    return 0;
}
