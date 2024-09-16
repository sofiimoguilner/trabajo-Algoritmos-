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
void mostrarMontoMaximo(Transaccion &transaccion, Cliente &cliente);
void enteroACad(int n, char* fecha);
void ordenarPorFecha(Transaccion &transaccion, int contadorTransacciones);
void listarTransacciones(Transaccion &transaccion, string username);
void mostrar5(int contadorTransacciones, Transaccion transaccionesOrden[]);


// Listado de transacciones del cliente ingresado
void listarTransacciones(Transaccion &transaccion, string username){
	FILE* archivoTransacciones = fopen ("Transacciones.txt", "rb");
	FILE* archivoTemp = fopen("Temporal.txt", "ab");
	
	bool hayTransacciones = false;
	int contadorTransacciones = 0;
	
	// Guardamos todas las transacciones del username ingresado en un archivo temporal ("Temporal.txt")
	while (fread(&transaccion, sizeof(Transaccion), 1, archivoTransacciones)){
		if(transaccion.username == username){
			hayTransacciones = true;
			fwrite(&transaccion, sizeof(Transaccion), 1, archivoTemp);
			contadorTransacciones ++;
		}
	}
	fclose(archivoTemp);
	
	// Si no hay transacciones, salta error y se vuele al inicio
	if (!hayTransacciones){
		cout << "\n- - - No se encontraron transacciones registrados con este usuario - - -\n";
		return;
	}
	
	// Ordenamos las transacciones, luego las mostramos
	cout << "\n- - - Transacciones de " << username << " - - -\n\n";
	ordenarPorFecha(transaccion, contadorTransacciones);
	
	remove("Temporal.txt"); // Eliminamos el archivo temporal
	fclose(archivoTransacciones);
}

// Ordenar las transacciones por fecha (más reciente a más antigua)
void ordenarPorFecha(Transaccion &transaccion, int contadorTransacciones){
	FILE* archivoTemp = fopen("Temporal.txt", "rb");
	
	Transaccion transaccionesOrden[contadorTransacciones]; // Creamos array con máximo de transacciones posibles
	Transaccion temp;
	int contador = 0;
	
	// Guardamos las transacciones del archivo "Temporal" en el array "transaccionesOrden"
	while (fread(&transaccion, sizeof(Transaccion), 1, archivoTemp)){
		transaccionesOrden[contador] = transaccion;
		contador ++;
	}
	
	// Realizamos ordenamiento burbuja en el array "transaccionesOrden"
	for (int i = 0; i < contadorTransacciones; i++){
		for (int j = 0; j < contadorTransacciones - i - 1; j++){
			if (transaccionesOrden[j].fecha < transaccionesOrden[j+1].fecha){
				temp = transaccionesOrden[j];
				transaccionesOrden[j] = transaccionesOrden[j+1];
				transaccionesOrden[j+1] = temp;
			}
		}
	}
	
	mostrar5(contadorTransacciones, transaccionesOrden); // Mostramos las transacciones
	
	fclose(archivoTemp);
}

// Mostrar en pantalla las transacciones (previamente ordenadas) de a 5 por pagina
void mostrar5(int contadorTransacciones, Transaccion transaccionesOrden[]){
	int pagina = 1;
	cout << "============Pagina " << pagina << " ============" << endl;
	for (int i=0; i < contadorTransacciones; i++){
		// Una vez que se mostraron 5 transacciones, se le pregunta si quiere pasar a la siguiente pagina
		if (i % 5 == 0 && i!=0){
			char rta;
			cout<<"\nDesea ver la siguiente pagina? (S/N): ";
			cin>>rta;
			if( rta != 'S' && rta!= 's'){
				break;
			} else {
				pagina++;
				cout << "\n============Pagina " << pagina << " ============" << endl;
			}
		}
		
		char fecha[11];
        enteroACad(transaccionesOrden[i].fecha, fecha);
        cout << "ID: " << transaccionesOrden[i].id
			 << ", Fecha: " << fecha 
			 << ", Monto: $" << transaccionesOrden[i].monto
			 << ", Tipo: " << transaccionesOrden[i].tipo << endl;
	}
}

// Mostrar transaccion de mayor monto de cada cliente
void mostrarMontoMaximo(Transaccion &transaccion, Cliente &cliente){
	FILE* archivoTransacciones = fopen("Transacciones.txt", "rb");
	FILE* archivoClientes = fopen("Clientes.txt", "rb");
	
	// Leemos cantidad de Clientes, para saber el máximo de transacciones a mostrar
	int contadorCliente = 0;
	while(fread(&cliente, sizeof(Cliente), 1, archivoClientes)){
		contadorCliente++;
	}
	fclose(archivoClientes);
	
	Transaccion max[contadorCliente]; // Array para almacenar el monto máximo por cada cliente
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
		if (!usuarioEncontrado && contador < contadorCliente){
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
		
		char fecha[11];
		enteroACad(max[i].fecha, fecha);
		cout << "Fecha: " << fecha << endl;
		
		cout << "Monto: " << max[i].monto << endl;
		cout << "========================" << endl;
	}
	
	fclose(archivoTransacciones);
}

// Pasar la fecha entera a cadena. Ayuda para mostrar en pantalla correctamente la fecha
void enteroACad(int n, char* fecha){
	int aux = n;
	
	int dia = aux % 100;
	aux = aux / 100;
	int mes = aux % 100;
	int anio = aux / 100;
	
	fecha[0] = (anio / 1000) + '0';
    fecha[1] = ((anio / 100) % 10) + '0';
    fecha[2] = ((anio / 10) % 10) + '0';
    fecha[3] = (anio % 10) + '0';
    fecha[4] = '/';
    fecha[5] = (mes / 10) + '0';
    fecha[6] = (mes % 10) + '0';
    fecha[7] = '/';
    fecha[8] = (dia / 10) + '0';
    fecha[9] = (dia % 10) + '0';
    fecha[10] = '\0';
}
