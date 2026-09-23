// TP de algoritmo y estructura de Datos, programa normalizacion.cpp
// Lista de mozos: Melina Salomé Cejas Guillen -> mail: Salomecejas.educacion@gmail.com -> Github: cejassalo
// Planillas x dia y stock: Pérez Griselda Soledad -> mail: griperez@frba.utn.edu.ar -> Github: griperez-sgp
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
	char nombre[50];
	char password[20];
	float totalComision;
};

struct NodoMozo {
	Mozo info;
	NodoMozo* sgte;
};
//--- Struct para Planilla x dia y stock
struct Producto {
	int codigo;
	char descripcion[50];
	float precio;
	int stockActual;
};

struct Comanda {
	int idMozo;
	int codigoProducto;
	int cantidad;
	float comision;
};

struct NodoComanda {
	Comanda info;
	NodoComanda* sig;
};

struct NodoFecha {
	char fecha[11];
	NodoComanda* listaComandas;
	NodoFecha* sig;
};
//---
NodoMozo* listaMozos = NULL; // la vamos armando en procesarArchivo()
//--- Lo armo en procesarPlanillayStock
NodoFecha* listaFechas = NULL;
//---
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

// --- Funciones para crear planilla del dia y act stock
// Crea nodo de comanda, modifique crear nodo de la catedra
NodoComanda* crearNodoComanda(Comanda valor) {
	NodoComanda* nuevoNodo = new NodoComanda();
	nuevoNodo->info = valor;
	nuevoNodo->sig = NULL;
	return nuevoNodo;
}
//Insertar comanda ordenada por id de mozo, modifique funcion insertar ordenado de la catedra
NodoComanda* insertarComandaOrdenada(NodoComanda* inicio, Comanda valor) {
	NodoComanda* nuevoNodo = crearNodoComanda(valor);
	
	if (inicio == NULL || valor.idMozo < inicio->info.idMozo) {
		nuevoNodo->sig = inicio;
		return nuevoNodo;
	}
	
	NodoComanda* actual = inicio;
	
	while (actual->sig != NULL && actual->sig->info.idMozo <= valor.idMozo) {
		actual = actual->sig;
	}
	
	nuevoNodo->sig = actual->sig;
	actual->sig = nuevoNodo;
	
	return inicio;
}
//Crear nodo para la fecha
NodoFecha* crearNodoFecha(char fecha[]) {
	NodoFecha* nuevoNodo = new NodoFecha();
	
	strcpy(nuevoNodo->fecha, fecha);
	nuevoNodo->listaComandas = NULL;
	nuevoNodo->sig = NULL;
	
	return nuevoNodo;
}
//Busca una fecha, modifique funcion de busqueda
NodoFecha* buscarFecha(NodoFecha* inicio, char fecha[]) {
	NodoFecha* actual = inicio;
	
	while (actual != NULL) {
		if (strcmp(actual->fecha, fecha) == 0) {
			return actual;
		}
		
		actual = actual->sig;
	}
	
	return NULL;
}
//Cargar fecha use funcion de cargar lista al final
NodoFecha* cargarFecha(NodoFecha* inicio, char fecha[]) {
	NodoFecha* nuevoNodo = crearNodoFecha(fecha);
	
	if (inicio == NULL) {
		inicio = nuevoNodo;
	} else {
		NodoFecha* actual = inicio;
		
		while (actual->sig != NULL) {
			actual = actual->sig;
		}
		
		actual->sig = nuevoNodo;
	}
	
	return inicio;
}
//Funcion para actualizar stock
void actualizarStock(int codigoProducto, int cantidad) { // ingresa codigo del producto y cantidad, no devuelve nada
	//abro el archivo inventario.dat para lectura y escritura
	FILE* inventario = fopen("inventario.dat", "rb+");
	//verifico si se pudo abrir
	if (inventario == NULL) {
		cout << "No se pudo abrir inventario.dat" << endl;
		return;
	}
	//creo una variable tipo producto, para ir guardando c/producto
	Producto producto;
	//leo producto x producto con while, fread devuelve 1 si puede leer correctamente
	while (fread(&producto, sizeof(Producto), 1, inventario) == 1) {
		// comparo por codigo de producto (el que lei con el que busco)
		if (producto.codigo == codigoProducto) {
			//Lo encontre, actualizo el stock restando la cantidad vendida 
			producto.stockActual = producto.stockActual - cantidad;
			//como fread avanza el puntero, retrocedo el tamaño de 1 producto
			fseek(inventario, -(long)sizeof(Producto), SEEK_CUR);
			//sobreescribo el producto con el stock actualizado
			fwrite(&producto, sizeof(Producto), 1, inventario);
			//cierro el archivo
			fclose(inventario);
			//retorno
			return;
		}
	}
	//Si termina de recorrer el archivo y no encuentra el producto tmb cierra el archivo
	fclose(inventario);
}
//Funcion que procesa la planilla y actualiza el stock
void procesarPlanillasYStock() {
	//abro el archivo de comandas historicas modo lectura
	FILE* f = fopen("comandas_historicas.dat", "rb");
	//verifico si abrio
	if (f == NULL) {
		cout << "No se pudo abrir comandas_historicas.dat" << endl;
		return;
	}
	//creo una veriable auxiliar para guardar cada comanda historica del archivo
	ComandaHistorica aux;
	//recorro comanda x comanda con unh while
	while (fread(&aux, sizeof(ComandaHistorica), 1, f) == 1) {
		
		//Busco el nombre del mozo en la lista que armo mi compañera
		NodoMozo* mozo = buscarMozo(listaMozos, aux.nombreMozo);
		//Si encuentra al mozo
		if (mozo != NULL) {
			
			//Busco si ya existe la fecha de la comanda
			NodoFecha* fecha = buscarFecha(listaFechas, aux.fecha);
			
			//Si no existe
			if (fecha == NULL) {
				//agrego una nueva fecha al final con la funcion cargarFecha
				listaFechas = cargarFecha(listaFechas, aux.fecha);
				//la vuelvo a buscar para tener el puntero al nodo en esa fecha
				fecha = buscarFecha(listaFechas, aux.fecha);
			}
			
			// Armo la nueva comanda
			//Creo la comanda nueva
			Comanda nueva;
			//Le cargo los datos que tengo en el auxiliar
			nueva.idMozo = mozo->info.idMozo;
			nueva.codigoProducto = aux.producto;
			nueva.cantidad = aux.cantidad;
			nueva.comision = aux.comision;
			
			// La inserto ordenada por numero de mozo
			fecha->listaComandas = insertarComandaOrdenada(fecha->listaComandas, nueva);
			
			// Descuento del inventario lo que se vendio
			actualizarStock(aux.producto, aux.cantidad);
		}
	}
	//cierro el archivo
	fclose(f);
	
	// Creo un archivo .dat por cada dia
	NodoFecha* fechaActual = listaFechas;
	//Recorro todas las fechas con un while
	while (fechaActual != NULL) {
		//creo un array donde guardo el nombre del archivo
		char nombreArchivo[150];
		//copio la ruta y el cmienzo del nombre
		strcpy(nombreArchivo, "comandas_");
		//concateno la fecha al nombre del archivo
		strcat(nombreArchivo, fechaActual->fecha);
		//concateno la extension .dat
		strcat(nombreArchivo, ".dat");
		//Creo el archivo correspondiente a esa fecha, wb abre para escritura
		FILE* archivoDia = fopen(nombreArchivo, "wb");
		//verifico si se creo
		if (archivoDia != NULL) {
			//Me posiciono al inicio de la lista de esa fecha
			NodoComanda* comandaActual = fechaActual->listaComandas;
			//recorro todas las comandas de ese dia
			while (comandaActual != NULL) {
				//escribo la informacion de la comanda
				fwrite(&comandaActual->info,sizeof(Comanda),1,archivoDia);
				//avanzo a la siguiente
				comandaActual = comandaActual->sig;
			}
			//si termina de leer y guardar cierro el archivo
			fclose(archivoDia);
		}
		//avanzo a la siguiente fecha
		fechaActual = fechaActual->sig;
	}
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
	
	// Proceso la planila y actualizo el stock
	procesarPlanillasYStock();
	
	
	// para probarlo: muestro las planillas
	//Creo un puntero y lo posiciono al principio de la lista de fechas
	NodoFecha* fechaActual = listaFechas;
	//recorro con un while las fechas
	while (fechaActual != NULL) {
		//imprimo fecha
		cout << endl;
		cout << "Fecha: " << fechaActual->fecha << endl;
		
		//Creo un puntero para recorrer las comandas lo posiciono al inicio de las comandas de ese dia
		NodoComanda* comandaActual = fechaActual->listaComandas;
		//recorro con while las comandas
		while (comandaActual != NULL) {
			//Imprimo los datos
			cout << "Mozo: " << comandaActual->info.idMozo
			<< "  Producto: " << comandaActual->info.codigoProducto
			<< "  Cantidad: " << comandaActual->info.cantidad
			<< "  Comision: " << comandaActual->info.comision
			<< endl;
			//apunto con el puntero a la sig comanda de ese fecha
			comandaActual = comandaActual->sig;
		}
		//apunto a la sig fecha
		fechaActual = fechaActual->sig;
	}
	
	
	return 0;
}
