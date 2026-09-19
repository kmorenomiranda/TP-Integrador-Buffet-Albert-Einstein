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
          fclose(archivoDestino);
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
//ALGORTIMO DE APAREO
//TODO ESTE CHOCLO junta dos archivos ordenados por idMozo en un solo archivo final
bool aparear(const char nomA[], const char nomB[], const char nomC[]) {
    FILE* archivoA = fopen(nomA, "rb"); 
    FILE* archivoB = fopen(nomB, "rb");
    FILE* archivoC = fopen(nomC, "wb"); 

    if (archivoA == NULL || archivoB == NULL || archivoC == NULL) {
        if (archivoA != NULL){
         fclose(archivoA);
        }
        if (archivoB != NULL){
         fclose(archivoB);
        }
        if (archivoC != NULL){
          fclose(archivoC); 
        }
        return false;
    }

   Comanda comandaA;
   Comanda comandaB;
   int leidoA = (int)fread(&comandaA, sizeof(Comanda), 1, archivoA);
   int leidoB = (int)fread(&comandaB, sizeof(Comanda), 1, archivoB); 

   //esta parte es para que mientras haya comandas en ambos archivos, se comparan y se guarda la menor por el idMozo
   while (leidoA == 1 && leidoB == 1) {  
    if (comandaA.idMozo <= comandaB.idMozo) {
      fwrite(&comandaA, sizeof(Comanda), 1, archivoC);
      leidoA = (int)fread(&comandaA, sizeof(Comanda), 1, archivoA);
    } else {
          fwrite(&comandaB, sizeof(Comanda), 1, archivoC);
          leidoB = (int)fread(&comandaB, sizeof(Comanda), 1, archivoB);
        }
    }
    while (leidoA == 1) {
       fwrite(&comandaA, sizeof(Comanda), 1, archivoC);
       leidoA = (int)fread(&comandaA, sizeof(Comanda), 1, archivoA);
    }
    while (leidoB == 1) {
        fwrite(&comandaB, sizeof(Comanda), 1, archivoC);
        leidoB = (int)fread(&comandaB, sizeof(Comanda), 1, archivoB);
    }

    fclose(archivoA);
    fclose(archivoB);
    fclose(archivoC);
    return true;
  }

int main(){
    //  Primero los datos que identifican la semana a cerrar para armar el nombre del archivo semanal final (comandas_semana_sX-mm.dat).
    int numeroSemana, mes, cantidadDias;
    cout << "Numero de semana (X): ";
    cin >> numeroSemana;
    cout << "Mes (numero, ej. 6 para junio): ";
    cin >> mes;
    cout << "Cuantos dias tiene la semana a cerrar: ";
    cin >> cantidadDias;

    cout << "Inicio del main" ;
    return 0;
}
