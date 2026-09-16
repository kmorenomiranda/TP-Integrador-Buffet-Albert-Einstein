#include <iostream>
#include <cstdio> 
using namespace std;

struct Comanda{ // Struct sacado del enunciado
    int idMozo;
    int codigoProducto;
    int cantidad;
    float comision;
};

// Corte de Control porque el archivo semanal ya está ordenado por idMozo

void corteControl(const char* nombreArchivo) {
    FILE* f = fopen(nombreArchivo, "rb");
    if(f == NULL) { // Se activa cuando el usuario ejecuta resumen.cpp sin haber ejecutado cierre.cpp primero
        cout << "Error al intentar leer el archivo." << endl;
        cout << "Asegúrese de haber ejecutado cierre.cpp primero." << endl;
        return;
    }

    Comanda c;
    int leido = fread(&c, sizeof(Comanda), 1, f);
    int totalProductosBuffet = 0; // Para contar la cantidad de productos vendidos por el buffet en la semana 

    cout << "........................................." << endl;
    cout << "             RESUMEN SEMANAL             " << endl; // La pongo acá para que no se repita por cada mozo registrado
    cout << "........................................." << endl;

    while(leido == 1) {
        int mozoActual = c.idMozo; // ID del mozo actual
        int mozoProductos = 0; // Productos vendidos por el mozo actual
        float mozoComision = 0; // Comision del mozo actual
        
        while(leido == 1 && c.idMozo == mozoActual) {
            mozoProductos += c.cantidad; // -> mozo
            mozoComision += c.comision; // -> mozo
            totalProductosBuffet += c.cantidad; // -> buffet
            leido = fread(&c, sizeof(Comanda), 1, f);
        }

        cout << "- Mozo ID" << mozoActual << " -" << endl;
        cout << "Productos vendidos: " << mozoProductos << endl;
        cout << "Comision: $" << mozoComision << endl;
    }
    fclose(f);
}

// Agregar el total vendido por el buffet 