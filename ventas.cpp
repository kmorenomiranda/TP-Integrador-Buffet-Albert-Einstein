
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
    for (int i = 0; clave[i] != '\0'; i++) {
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


long buscarProducto(const char* archivoInventario, int codigo, Producto& p) {
	FILE* f = fopen(archivoInventario, "rb");
	if (f == NULL) {
		return -1;
	}
	fseek(f, 0, SEEK_END);
	long n = ftell(f) / sizeof(Producto); // cant. de registros
	
	long primero = 0, ultimo = n-1, pos = -1;
	while (primero <= ultimo && pos == -1){
		long medio = (primero + ultimo) / 2;
		fseek(f, medio * sizeof(Producto), SEEK_SET);
		fread(&p, sizeof(Producto), 1, f);
		if (p.codigo == codigo) {
			pos = medio;
		} else if (codigo > p.codigo){
			primero = medio + 1;
		} else {
			ultimo = medio - 1;
		}
	}
	
	fclose(f);
	return pos;
	
	}
		
void descontarStock(const char* archivoInventario, long pos, int cantidad) {
    FILE* f = fopen(archivoInventario, "rb+");
    if (f == NULL) return;
    Producto p;
    fseek(f, pos * sizeof(Producto), SEEK_SET);
    fread(&p, sizeof(Producto), 1, f);          
    p.stockActual -= cantidad;
    fseek(f, pos * sizeof(Producto), SEEK_SET); 
    fwrite(&p, sizeof(Producto), 1, f);
    fclose(f);
}


	void mostrarVentas(Comanda ventas[], int len) {              // solo para ver si anda o no
		for (int i = 0; i < len; i++) { 
			cout << " Mozo: " << ventas[i].idMozo;
			cout << " Producto: " << ventas[i].codigoProducto;
			cout << " Cantidad: " << ventas[i].cantidad;
			cout << " Comision: " << ventas[i].comision << endl;
        	}
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
    
    Comanda ventas[MAX_VENTAS_DIA]; 
    int len = 0;                      // arranca vacio
    
    
    int idMozo;
    cout << "Ingrese numero de mozo (0 para terminar): ";
    cin >> idMozo;
    while (idMozo != 0) {
        char clave [20];
        Mozo mozo;
        cout << "Clave: ";
        cin >> clave;

        if (loginMozo(archivoMozos, idMozo,clave,mozo)) {
            cout << "Bienvenido/a, " << mozo.nombre << " ! \n" << endl;
            
            int codigoProducto, cantidad;
            cout << "Codigo de producto: ";
            cin >> codigoProducto;
            cout << "Cantidad: ";
            cin >> cantidad;
 
            Producto prod;
            long pos = buscarProducto(archivoInventario, codigoProducto, prod);
            if (pos == -1) {
                cout << "Ese producto no existe.\n" << endl;
            } else if (prod.stockActual < cantidad) {
                cout << "No hay stock suficiente (quedan " << prod.stockActual << ") \n" << endl;
            } else {
                Comanda c;
                c.idMozo         = idMozo;
                c.codigoProducto = codigoProducto;
                c.cantidad       = cantidad;
                c.comision       = prod.precio * cantidad * TASA_COMISION;
 
                ventas[len] = c;   // agrego al final del array
                len++;             // y aumento el tamaño
                descontarStock(archivoInventario, pos, cantidad); 
 
                cout << "Venta cargada. Comision: " << c.comision << "\n" << endl;
            }
            
            } else {
                cout << " \nIntente nuevamente \n" << endl;
            }
            cout << "Ingrese el numero del mozo (0 para terminar): ";
            cin >> idMozo;
    }
    cout << "\nVentas cargadas en esta sesion (" << len << "):" << endl;
    mostrarVentas(ventas, len);
    // El nombre se armo bien
    cout << "\nArchivo del dia que vamos a usar: " << nombreArchivoDia << endl;

    return 0;
    
}
