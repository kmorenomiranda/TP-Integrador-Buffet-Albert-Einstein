#include <iostream>
#include <cstdio>
#include <cstring>
using namespace std;

const int K = 5;

struct ComandaHistorica {
    char fecha[11];
    char nombreMozo[50];
    int producto;
    int cantidad;
    float comision;
};

struct Mozo {
    int idMozo;
    char nombre[50 + 1];
    char password[20];
    float totalComision;
};

struct NodoMozo {
    Mozo info;
    NodoMozo* sgte;
};

NodoMozo* listaMozos = NULL; // la vamos armando en procesarArchivo()

// busca un mozo por nombre en listaDeMozos. Si no esta, devuelve NULL.
NodoMozo* buscarMozo(NodoMozo* listaDeMozos, char nombre[]) {
    NodoMozo* p = listaDeMozos;
    while (p != NULL) {
        if (strcmp(p->info.nombre, nombre) == 0) {
            return p;
        }
        p = p->sgte;
    }
    return NULL;
}

// crea el nodo (con new) e inserta al final de listaDeMozos
NodoMozo* insertarFinal(NodoMozo* listaDeMozos, Mozo nuevoMozo) {
    NodoMozo* nodo = new NodoMozo;
    nodo->info = nuevoMozo;
    nodo->sgte = NULL;

    if (listaDeMozos == NULL) {
        return nodo;
    }
    NodoMozo* p = listaDeMozos;
    while (p->sgte != NULL) {
        p = p->sgte;
    }
    p->sgte = nodo;
    return listaDeMozos;
}

// arma la clave inicial del mozo y la deja "encriptada" en password
void generarPasswordInicial(int idMozo, char password[20]) {
    // 1) el id es un numero, pero la clave tiene que quedar como texto
    //    (password es char[]). Lo paso a texto sacando digitos de a uno
    //    con % 10 y / 10 (salen del ultimo al primero).
    // (no hace falta contemplar numero == 0: el primer idMozo es 1 y
    // proximoId solo crece, asi que nunca va a llegar un idMozo en 0)
    char texto[20];
    int cant = 0;
    int numero = idMozo;

    while (numero > 0) {
        int digito = numero % 10;
        texto[cant] = '0' + digito;
        numero = numero / 10;
        cant++;
    }
    // quedaron al reves (ej: idMozo=12 -> texto="21"), los doy vuelta
    for (int i = 0; i < cant / 2; i++) {
        char aux = texto[i];
        texto[i] = texto[cant - 1 - i];
        texto[cant - 1 - i] = aux;
    }

    // 2) "encripto": a cada caracter le sumo K. Es reversible (restando K
    //    del lado de ventas.cpp), por eso sirve para validar el login.
    for (int i = 0; i < cant; i++) {
        password[i] = texto[i] + K;
    }
    password[cant] = '\0';
}

void procesarArchivo() {
    FILE* f = fopen("comandas_historicas.dat", "rb");

    ComandaHistorica aux;
    int proximoId = 1; // el id que le voy a dar al PROXIMO mozo nuevo que aparezca

    while (fread(&aux, sizeof(ComandaHistorica), 1, f) == 1) {

        // buscar el mozo (si esta o no). "mozo" se recalcula aca, en cada
        // vuelta del while, no se arrastra el valor de la vuelta anterior.
        NodoMozo* mozo = buscarMozo(listaMozos, aux.nombreMozo);

        // Decidimos
        if (mozo != NULL) {
            //--> si esta: acumulamos la comision
            mozo->info.totalComision += aux.comision;
        } else {
            //--> si no esta: lo agregamos
            Mozo nuevo;
            nuevo.idMozo = proximoId;   // le doy el id que tocaba
            strcpy(nuevo.nombre, aux.nombreMozo);
            nuevo.totalComision = aux.comision;
            generarPasswordInicial(nuevo.idMozo, nuevo.password);

            listaMozos = insertarFinal(listaMozos, nuevo);
            proximoId++; // el que sigue ya tiene que agarrar el proximo numero
        }
    }

    fclose(f);

    FILE* q = fopen("mozos.dat", "wb");
    NodoMozo* p = listaMozos;
    while (p != NULL) {
        fwrite(&p->info, sizeof(Mozo), 1, q);
        p = p->sgte;
    }
    fclose(q);
}

int main() {
    procesarArchivo();

    // para probarlo: muestro lo que quedo armado
    NodoMozo* p = listaMozos;
    while (p != NULL) {
        cout << "id=" << p->info.idMozo
             << "  nombre=" << p->info.nombre
             << "  clave=" << p->info.password
             << "  comision=" << p->info.totalComision << endl;
        p = p->sgte;
    }

    return 0;
}