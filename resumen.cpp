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
        return;
    }

    Comanda c;
    int leido = fread(&c, sizeof(Comanda), 1, f);
    int totalProductosBuffet = 0; // Para contar la cantidad de productos vendidos por el buffet en la semana 

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
        
        // El resumen del último mozo del cierre (que no tiene otro mozo después)
        // Cuando se termina de leer todo el archivo, la funcion fread devuelve 0 y se sale del while, pero se muestran todos los mozos incluyendo el último
        cout << "- Mozo ID " << mozoActual << " -" << endl;
        cout << "Productos vendidos: " << mozoProductos << endl;
        cout << "Comision: $" << mozoComision << endl;
        cout << "_____________________________________________" << endl;

    }
    cout << "- - - - - - - - - - - - - - - - - - - - - - -" << endl;
    cout << "Total de productos vendidos por el buffet: " << totalProductosBuffet << endl; // Muestra el total vendido por el buffet
    cout << "- - - - - - - - - - - - - - - - - - - - - - -" << endl;

    fclose(f);
}

// Como se van generando un "comandas_semana_sX-mm.dat" por cada semana, se le pide al usuario que ingrese el numero de semana y mes del archivo semanal que desee ver en pantalla
int main() {
    int semana, mes;
    char nombreArchivo[30];

    cout << "RESUMEN SEMANAL" << endl;
    cout << "Ingrese el numero de la semana (del 1 al 5): " << endl;
    cin >> semana;
    cout << "Ingrese el numero del mes (del 1 al 12): " << endl;
    cin >> mes;

    // Para armar el nombre del archivo semanal "comandas_semana_sX-mm.dat" donde X es la semana y mm es el mes
    sprintf(nombreArchivo, "comandas_semana_s%d-%02d.dat", semana, mes);

    FILE* f = fopen(nombreArchivo, "rb");
    
    // En caso de que el usuario ingrese un archivo que no existe, se le pedira que ingrese nuevamente los datos hasta que se encuentre el archivo
    while(f == NULL) {
        cout << "No se encontro el archivo " << nombreArchivo << endl;
        cout << "Verifique los datos ingresados e intente nuevamente." << endl;
        cout << "Si el problema persiste, asegurese de haber ejecutado cierre.cpp primero." << endl;

        cout << "Ingrese el numero de semana nuevamente(del 1 al 5): " << endl;
        cin >> semana;
        cout << "Ingrese el numero del mes nuevamente(del 1 al 12): " << endl;
        cin >> mes;

        // Cambia el primer nombre ingresado por el nuevo
        sprintf(nombreArchivo, "comandas_semana_s%d-%02d.dat", semana, mes);
        f = fopen(nombreArchivo, "rb");
    }

    fclose(f);

    cout << "............................................." << endl;
    cout << "          RESUMEN SEMANA " << semana << " - MES " << mes << endl;
    cout << "............................................." << endl;

    corteControl(nombreArchivo); // Si el archivo existe, llama a la funcion 
    return 0;
}