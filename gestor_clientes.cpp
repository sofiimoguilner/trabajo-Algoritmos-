#include <iostream>
#include <fstream>
#include <memory>

using namespace std;

struct Cliente{
    string nombre;
    int dni;
    string username;
    char clave;
};

struct Saldo_Cliente{
    Cliente cliente;
    float saldo;
};

//necesitamos archivo para comparar
void iniciar_sesion(Cliente cliente);

//necesitamos archivo para guardar lo que registramos
void registrar_usuario(Cliente cliente);
void generar_clave(Cliente cliente);

//necesitamos archivo para verificacion
void verificar_datos(Cliente cliente);


int main()
{
    int rta;
    Cliente c;
    Saldo_Cliente s;


    cout<<"Elija opcion: "<<endl;
    cout<<"1.Registrarse"<<endl;
    cout<<"2.Iniciar sesion"<<endl;
    cin>>rta;

    switch (rta)
    {
    case 1:
        
        registrar_usuario(c);
        
        break;
    
    case 2:

        iniciar_sesion(c);

        break;
    
    default:

        cout<<"opcion no valida";

        break;
    
    }
}

void iniciar_sesion(Cliente cliente){
    string usuario;
    char clave;
    cout<<"Ingrese username"<<endl;
    cin>>usuario;
    cout<<"Ingrese clave"<<endl;
    cin>>clave;
}

void generar_clave(Cliente cliente){
    cout<<"Genere su clave"<<endl;
    cin>>cliente.clave;
}

void registrar_usuario(Cliente cliente){
    
}
