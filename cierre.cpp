#include <iostream>
#include <cstdio>
using namespace std;

struct Comanda { //Lo necesitamos para realizar el apareamiento luego 
    int idMozo;
    int codigoProducto;
    int cantidad;
    float comision;
};
// El bool lo uso para clonar los archivos del dia 1 dentro de un borrador
// luego se copia el siguiente hasta guardar el resultado final de la semana.
bool copiarArchivo(const char origen[], const char destino[]){ 
    FILE* archivoOrigen = fopen(origen, "rb");
    FILE* archivoDestino = fopen(destino, "wb");

    if (archivoOrigen == NULL || archivoDestino == NULL){
        if (archivoOrigen != NULL){
          fclose(archivoOrigen);
        }
        if (archivoDestino != NULL){
          fclose(archivoDestino)
            }
        return false;
    }

    Comanda comanda;  // Variable temporal para guardar una comanda en la ram mientras la leemos y luego se sobreescribe.

    while (fread(&comanda, sizeof(Comanda), 1, archivoOrigen) == 1){
        fwrite(&comanda, sizeof(Comanda), 1, archivoDestino);
    }

    fclose(archivoOrigen);
    fclose(archivoDestino);

    return true;
}

int main(){
  // emm :3 still working on it jeje PERO quiero realizar un apareamiento porque las planillas ya van a estar ordenadas
  // asi que fusionar los días manteniendo el orden por mozo va a ser mas facil. luego sigo
}
