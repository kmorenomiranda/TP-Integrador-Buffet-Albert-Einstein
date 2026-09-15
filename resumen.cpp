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

void corteControl(const char* nombre) {
    FILE* f = fopen(nombre, "rb");
    if(f == NULL) return;
    Comanda c;
    int leido = fread(&c, sizeof(Comanda), 1, f);
    while(leido == 1) {
        int claveGrupo = c.idMozo;
        float total = 0;
        while(leido == 1 && c.idMozo == claveGrupo) {
            total += c.comision;
            leido = fread(&c, sizeof(Comanda), 1, f);
        }
        cout << "Grupo " << claveGrupo << ": total = " << total << endl;
    }
    fclose(f);
}

// Adaptar el corte de control sacado del pdf Archivo v2 al enunciado