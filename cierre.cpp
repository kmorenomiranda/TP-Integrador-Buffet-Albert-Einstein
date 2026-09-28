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
    }
    else {
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
    cout << "Numero de semana a cerrar (X): ";
    cin >> numeroSemana;
    cout << "Mes (numerico, ej. 6 para junio): ";
    cin >> mes;
    cout << "Cuantos dias tiene la semana a cerrar: ";
    cin >> cantidadDias;

    char acumulador[50] = "";
    int temporal = 0;

    for (int i = 0; i < cantidadDias; i++) {  // Recorremos cada dia de la semana pedido por Alberto.
        char fecha[11];
        cout << "Fecha del dia " << i + 1 << " (DD-MM-AAAA): ";
        cin >> fecha;
        char nombreDia[50];
        snprintf(nombreDia, sizeof(nombreDia), "comandas_%s.dat", fecha);
        
        // Hay dias de la semana que pueden no tener planilla. Si no existe, se saltea sin cortar el programa
        FILE* f = fopen(nombreDia, "rb");
        if (f == NULL) {
            cout << "Aviso: no existe " << nombreDia << ", se salta." << endl;
            continue;
        }
        fclose(f);

        //Si copiarArchivo devuelve false (ej, no se pudo abrir el archivo de destino en wb)
        //se corta el programa ahí mismo con return 1.
        if (acumulador[0] == '\0'){ 
            strcpy(acumulador, "cierre_temp0.dat");
            if (!copiarArchivo(nombreDia, acumulador)){
                cout << "Error al copiar " << nombreDia << "." << endl;
                return 1;
            }
        }
        else {
            char nuevo[50];
            snprintf(nuevo, sizeof(nuevo), "cierre_temp%d.dat", ++temporal);
            if (!aparear(acumulador, nombreDia, nuevo)) {
                cout << "Error al aparear con " << nombreDia << "." << endl;
                return 1;
            }
            remove(acumulador);
            strcpy(acumulador, nuevo);
        }
    }
    char nombreSemana[50];
    snprintf(nombreSemana, sizeof(nombreSemana), "comandas_semana_s%d-%02d.dat", numeroSemana, mes);
    // Nombre del archivo semanal definitivo que se va a entregar a Alberto.

    // En caso de que ningun dia de la semana tenga planilla. Se genera un archivo semanal vacio para que en resumen.cpp se pueda abrir igual.
    if (acumulador[0] == '\0') {
        FILE* vacio = fopen(nombreSemana, "wb");
        if (vacio == NULL) { cout << "No se pudo crear el archivo semanal." << endl; return 1; }
        fclose(vacio);
        cout << "No habia planillas diarias. Se genero " << nombreSemana << " vacio." << endl;
        return 0;
    }

    // El acumulador final pasa a ser el archivo semanal definitivo.
    if (!copiarArchivo(acumulador, nombreSemana)) {
        cout << "No se pudo generar " << nombreSemana << "." << endl;
        return 1;
    }
    remove(acumulador); // Eliminamos el último temporal.

    cout << "Cierre realizado. Archivo generado: " << nombreSemana << endl;
    return 0;
}
