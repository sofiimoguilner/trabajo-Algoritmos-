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

struct Transaccion{
	double monto;
	string fecha;
	string id;
	string tipo; //Ingreso o egreso
	Cliente datosCliente;
};

void agregarTransaccion(Cliente &cliente); //Esta funcion esta en generador.cpp

bool existeCliente(Cliente &clientes, const string &dni, const string &username){
	FILE* archivo = fopen("Clientes.txt", "rb");
	if(archivo != NULL){
		while(fread(&clientes, sizeof(Cliente), 1, archivo)==1){
			if(clientes.dni == dni || clientes.username == username){
				return true;
			}	
		}
		fclose(archivo);
	}
	return false;
}

void registrarCliente(Cliente &clientes) {
	
	string nombre, username, clave, dni;
	
    cout << endl << "Ingrese nombre: ";
    cin >> nombre;

    cout << "Ingrese DNI: ";
    cin >> dni;

    cout << "Ingrese username: ";
	cin>> username;

    cout << "Ingrese clave: ";
	cin>> clave;
	
	if(existeCliente(clientes, dni, username)){
		cout << endl << "Error: Ya existe un cliente con ese DNI o username.\n" << endl;
	}
	else{
		clientes.nombre = nombre;
		clientes.dni = dni;
		clientes.username = username;
		clientes.clave = clave;
		clientes.saldo = 10000.0;
		
		FILE* archivo = fopen("Clientes.txt", "ab");
		if (archivo != NULL) {
			fwrite(&clientes, sizeof(Cliente), 1, archivo);
      	 	fclose(archivo);
       		cout << endl << "Registro exitoso. Saldo inicial asignado: $10.000.\n" << endl;
		} else{
			cout << endl << "Error al abrir el archivo." << endl;
		}
	}
}

bool iniciarSesion(Cliente &clientes){
	
	string username, clave;
	
	cout << endl << "Ingrese USERNAME: ";
	cin>>username;
	
	cout<<"Ingrese CLAVE: ";
	cin>>clave;
	
	FILE* archivo = fopen("Clientes.txt", "rb");
	if (archivo != NULL){
		while(fread(&clientes, sizeof(Cliente), 1, archivo) == 1){
			if (clientes.username == username && clientes.clave == clave){
				fclose(archivo);
				return true;
			}
		}
		fclose(archivo);
	}
	
	return false;
	
}


int main() {
    int opcion, opcionCliente;
    Cliente clientes;
    do {
    	cout<<"- - - BIENVENIDO AL SISTEMA BANCARIO - - -" << endl;
        cout << "\n1. Registrar nuevo cliente\n2. Iniciar sesion\n3. Salir\n\nSeleccione una opcion: ";
        cin >> opcion;
		
        switch (opcion) {
            case 1:
                registrarCliente(clientes);
                break;
            case 2:
            	if(iniciarSesion(clientes)){
            		cout<< "\nBienvenido/a "<< clientes.nombre << "\nSu saldo es de: $"<<clientes.saldo;
            		do{
            			cout << "\n\n1. Agregar transaccion de ingreso o egreso de dinero\n2. Eliminar transaccion\n3. Salir\n\nSeleccione una opcion: ";
						cin >> opcionCliente;
						
						switch (opcionCliente){
							case 1:
								//agregarTransaccion(clientes);
								break;
							case 2:
								cout << "a";
								break;
							case 3:
								cout << endl << "Saliendo..." << endl << endl;
								break;
							default:
								cout << "Opcion no valida." << endl;
								break;
						}
					}while(opcionCliente !=3);
				}else{
					cout << endl << "Error: datos incorrectos." << endl << endl;
				}
                break;
            case 3:
            	cout << endl << "========================";
            	cout << endl << "Saliendo del sistema..." << endl;
            	cout << "========================";
            	break;
            default:
                cout << endl <<"Opcion no valida." << endl << endl;
                break;
        }
    } while (opcion != 3);
	
    return 0;
}
