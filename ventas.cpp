

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

// 1 Parte: structs base y lectura de la fecha
// ( todavia no hay login ni carga ventas )

#include <iostream>
#include <cstdio>
#include <cstring>
using namespace std;

struct Mozo {
    int   idMozo;
    char  nombre[50 + 1];  // porque en normalizacion.cpp aparece asi, consultar 
    char  password[20];    // clave ya ENCRIPTADA (corrimiento +K)
    float totalComision;
};

struct Producto {
    int   codigo;
    char  descripcion[50];
    float precio;
    int   stockActual;
};

struct Comanda {
    int   idMozo;
    int   codigoProducto;
    int   cantidad;
    float comision;
};

const float TASA_COMISION  = 0.10f;
const int   K               = 5;    // igual que K de normalizacion.cpp
const int   MAX_VENTAS_DIA  = 500;  

int main() {
    const char* archivoMozos      = "mozos.dat";
    const char* archivoInventario = "inventario.dat";

    char fecha[11];
    cout << "=== Carga de ventas del dia ===" << endl;
    cout << "Fecha (DD-MM-AAAA): ";
    cin >> fecha;

    char nombreArchivoDia[30];
    sprintf(nombreArchivoDia, "comandas_%s.dat", fecha);

    // El nombre se armo bien
    cout << "\nArchivo del dia que vamos a usar: " << nombreArchivoDia << endl;

    return 0;
}
