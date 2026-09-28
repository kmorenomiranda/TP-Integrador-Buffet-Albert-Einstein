
/* run this program using the console pauser or add your own getch, system("pause") or input loop */

// 1 Parte: structs base y lectura de la fecha

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

void encriptar (char clave [], int k) {
    for (int i = 0; clave[i] != '/0'; i++) {
        clave[i] = clave[i] + k;
    }
}

bool buscarMozo(const char* nombreArchivo, int idBuscado, Mozo& m) { //Busq. Secuencial 
    FILE* f = fopen(nombreArchivo, "rb");
    if (f == NULL) return false;
    bool encontrado = false;
    while (!encontrado && fread(&m, sizeof(Mozo), 1, f) == 1) {
        if (m.idMozo == idBuscado) {
            encontrado = true;
        }
    }
    fclose(f);
    return encontrado;
}

bool loginMozo(const char* archivoMozos, int idMozo, const char* claveTipeada, Mozo& mozoOut) {
    if (!buscarMozo(archivoMozos, idMozo, mozoOut)) {
        cout << "No existe un mozo con ese numero." << endl;
        return false;
    }

    char claveEncriptada[20];
    strcpy(claveEncriptada, claveTipeada);
    encriptar(claveEncriptada, K);

    if (strcmp(claveEncriptada, mozoOut.password) != 0) {
        cout << "Clave incorrecta." << endl;
        return false;
    }

    return true;
}


int main() {
    const char* archivoMozos      = "mozos.dat";
    const char* archivoInventario = "inventario.dat";

    char fecha[11];
    cout << "=== Carga de ventas del dia ===" << endl;
    cout << "Fecha (DD-MM-AAAA): ";
    cin >> fecha;

    char nombreArchivoDia[30];
    sprintf(nombreArchivoDia, "comandas_%s.dat", fecha);

    int idMozo;
    cout << "Ingrese numero de mozo (0 para terminar): ";
    cin >> idMozo;
    while (idMozo != 0) {
        char clave [20];
        Mozo mozo;
        cout << "Clave: ";
        cin >> clave;

        if (loginMozo(archivoMozos, idMozo,clave,mozo)) {
            cout << "Bienvenido/a, " << mozo.nombre << "\n" << endl;
            } else {
                cout << " \nIntente nuevamente \n" << endl;
            }
            cout << "Ingrese el numero del mozo (0 para terminar): ";
            cin >> idMozo;
    }
    
    // El nombre se armo bien
    cout << "\nArchivo del dia que vamos a usar: " << nombreArchivoDia << endl;

    return 0;
}
