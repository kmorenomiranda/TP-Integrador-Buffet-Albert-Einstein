# TP-Integrador-Buffet-Albert-Einstein
Trabajo Práctico grupal de Algoritmos y Estructuras de Datos (UTN FRBA) — unidad de archivos binarios en C/C++.
Integrantes del grupo:
1. Katherine Valeria Moreno Miranda -> mail: kmorenomiranda@frba.utn.edu.ar --> github: kmorenomiranda
2. Ayelen Romina Alcon Huayta -> mail: aalconhuayta@frba.utn.edu.ar 
3. Melina Salomé Cejas Guillen -> mail: Salomecejas.educacion@gmail.com
4. Pérez Griselda Soledad -> mail: griperez@frba.utn.edu.ar --> github: griperez-sgp
5. Karen Jazmin Marca Villalobos -> mail: kmarcavillalobos@frba.utn.edu.ar

### Planillas por día y actualización de stock
**Responsable:** Pérez Griselda Soledad

Esta parte se desarrolló dentro de `normalizacion.cpp`.

Se recorre el archivo `comandas_historicas.dat` y, para cada venta, se busca al mozo correspondiente en la lista de mozos para obtener su número identificador.

Las comandas se agrupan por fecha mediante una lista enlazada de fechas. Cada fecha posee su propia lista de comandas, las cuales se insertan ordenadas por número de mozo.

Mientras se procesan las ventas históricas también se actualiza `inventario.dat`, descontando del stock de cada producto la cantidad vendida.

Finalmente, se recorre la lista de fechas y se genera un archivo binario por cada día con el formato:

`comandas_DD-MM-AAAA.dat`

Cada archivo contiene las comandas correspondientes a ese día, ordenadas por número de mozo.

### Ejemplo de salida

Al ejecutar `normalizacion.cpp`, se genera:
```
mozos.dat
```
Y se muestran los mozos generados:

```
id=1  nombre=Sofia Ramirez  clave=6  comision=1600
id=2  nombre=Bruno Diaz     clave=7  comision=1870
id=3  nombre=Carla Lopez    clave=8  comision=1810
id=4  nombre=Diego Sosa     clave=9  comision=1800
id=5  nombre=Emma Ruiz      clave=:  comision=1690
```

Luego se muestran las comandas agrupadas por fecha y ordenadas por número de mozo:
```
Fecha: 02-06-2025
Mozo: 1  Producto: 101  Cantidad: 2  Comision: 300
Mozo: 1  Producto: 112  Cantidad: 1  Comision: 210
Mozo: 2  Producto: 105  Cantidad: 3  Comision: 240
Mozo: 3  Producto: 140  Cantidad: 2  Comision: 320
Mozo: 4  Producto: 120  Cantidad: 2  Comision: 240
Mozo: 5  Producto: 110  Cantidad: 1  Comision: 320

Fecha: 03-06-2025
Mozo: 1  Producto: 130  Cantidad: 1  Comision: 450
Mozo: 2  Producto: 112  Cantidad: 2  Comision: 420
Mozo: 3  Producto: 110  Cantidad: 1  Comision: 320
Mozo: 4  Producto: 125  Cantidad: 1  Comision: 180
Mozo: 5  Producto: 105  Cantidad: 4  Comision: 320
```

Además, se generan los archivos binarios correspondientes a cada fecha:
```
comandas_02-06-2025.dat
comandas_03-06-2025.dat
comandas_04-06-2025.dat
comandas_05-06-2025.dat
comandas_06-06-2025.dat
comandas_07-06-2025.dat
```
-------------------------------------------------
## cierre.cpp
**responsable:** Katherine Moreno

**Descripcion:** Programa que une las planillas diarias de comandas (`comandas_DD-MM-AAAA.dat`) de una semana en una única planilla semanal (`comandas_semana_sX-mm.dat`), ordenada por `idMozo`.

## Cómo correr

El programa va a pedir, en este orden:

1. **Número de semana** (ej. `1`).
2. **Mes** (Escrito en formato numerico).
3. **Cantidad de días** que tiene la semana a cerrar.
4. **Fecha de cada día de la semana a cerrar**, en formato `DD-MM-AAAA` (una por vez, tantas veces como días se indicó en el paso anterior).

Los archivos `comandas_DD-MM-AAAA.dat` correspondientes a esas fechas tienen que existir en el mismo directorio (los genera `ventas.cpp`). Si algún día no tiene planilla, el programa avisa y continúa sin cortarse.

**Resultado:** genera `comandas_semana_sX-mm.dat` con todos los registros de la semana, ordenados por `idMozo`.

## Cómo se resolvió

Las planillas diarias ya llegan ordenadas por `idMozo` (esta parte se realiza en `ventas.cpp`). Por lo que, utilice el algoritmo de **apareo (intercalación)**: *Procesa dos archivos ordenados por la misma clave en paralelo y los combina. En cada paso se procesa la clave menor y se avanza ese
archivo; cuando uno se agota, se vuelca el resto del otro.*

El patrón de apareo combina *dos* archivos, pero la semana a cerrar puede tener más de dos días. La solución es un **acumulador**: el primer día encontrado se copia directamente (no hay nada previo con qué compararlo); cada día siguiente se aparea con el acumulador actual, y el resultado pasa a ser el nuevo acumulador.

Los archivos temporales intermedios (`cierre_temp0.dat`, `cierre_temp1.dat`, ...) se borran con `remove()` apenas dejan de ser necesarios.

### Casos contemplados

- **Día sin planilla:** se detecta con `fopen` en modo `"rb"` devolviendo `NULL`; el programa avisa y sigue con el próximo día.
- **Ninguna planilla en toda la semana:** se genera un `comandas_semana_sX-mm.dat` vacío, para que `resumen.cpp` lo pueda abrir.
- **Cualquier posible error de apertura**: se corta el programa sin que se rompa.
