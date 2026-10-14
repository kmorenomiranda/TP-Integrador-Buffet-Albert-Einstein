TP-Integrador-Buffet-Albert-Einstein
Trabajo Práctico grupal de Algoritmos y Estructuras de Datos (UTN FRBA) — unidad de archivos binarios en C/C++.
Integrantes del grupo:
1. Katherine Valeria Moreno Miranda -> mail: kmorenomiranda@frba.utn.edu.ar --> github: kmorenomiranda
2. Ayelen Romina Alcon Huayta -> mail: aalconhuayta@frba.utn.edu.ar --> github: AyelenAlcon
3. Melina Salomé Cejas Guillen -> mail: Salomecejas.educacion@gmail.com --> github: cejassalo
4. Pérez Griselda Soledad -> mail: griperez@frba.utn.edu.ar --> github: griperez-sgp
5. Karen Jazmin Marca Villalobos -> mail: kmarcavillalobos@frba.utn.edu.ar --> gitHub: Kmarca-vil

## normalizacion.cpp (Carga inicial)

**Responsable:** Melina Salomé Cejas Guillén

*Descripción:* Programa encargado de procesar el archivo histórico de comandas (`comandas_historicas.dat`) para identificar a los mozos únicos, asignarles un número de identificador, calcular su comisión histórica acumulada y generar su contraseña inicial encriptada. Finalmente, exporta el archivo binario `mozos.dat'.

## Cómo correr
1. Debe estar disponible en el directorio el archivo binario de origen `comandas_historicas.dat`.
2. Compilar y ejecutar `normalizacion.cpp`.
3. Al finalizar, el programa imprime por consola el listado de mozos procesados con su respectivo ID, nombre, clave encriptada y comisión total, y genera automáticamente el archivo `mozos.dat`.

*Ejemplo de salida:*

id=1  nombre=Sofia Ramirez  clave=6  comision=1600
id=2  nombre=Bruno Diaz     clave=7  comision=1870
id=3  nombre=Carla Lopez    clave=8  comision=1810
id=4  nombre=Diego Sosa     clave=9  comision=1800
id=5  nombre=Emma Ruiz      clave=:  comision=1690


## Cómo se resolvió

* **Gestión dinámica en memoria:**
* Se implementó una lista enlazada simple (`NodoMozo`) para almacenar y manipular los mozos a medida que se leen del archivo histórico, ya que no se conoce de antemano la cantidad total de mozos únicos.
* **Búsqueda e inserción:**
* Por cada registro leído de `comandas_historicas.dat`, se realiza una búsqueda secuencial por nombre mediante la función `buscarMozo` utilizando `strcmp`.


* Si el mozo ya figura en la lista, se acumula su comisión (`mozo->info.totalComision += aux.comision`).


* Si el mozo no se encuentra, se crea un nodo nuevo, se le asigna un ID incremental (`proximoId`), se copia su nombre, se inicializa su comisión y se genera su clave llamando a `generarPasswordInicial`, insertándolo finalmente al final de la lista con `insertarFinal`.


* **Generación y encriptación de clave (`generarPasswordInicial`):**
* Se toma el `idMozo` numérico y se convierte a texto extrayendo sus dígitos uno a uno mediante operaciones aritméticas de módulo y división (`% 10` y `/ 10`), invirtiendo el resultado para restaurar el orden correcto de los caracteres.
* Se aplica un cifrado por desplazamiento sumando una constante fija (`K = 5`) a cada carácter del texto. Esto permite que el módulo de ventas posterior (`ventas.cpp`) valide la contraseña aplicando el corrimiento inverso.


* **Persistencia de datos:** Una vez procesado todo el archivo histórico, se recorre la lista enlazada desde el inicio y se graban todas las estructuras en el archivo binario `mozos.dat` usando `fwrite`.

## Casos contemplados

* **Mozos repetidos en el historial:** Se identifican de forma unívoca por su nombre para sumar sus comisiones sin duplicar registros en la estructura ni alterar su identificador asignado.
* **IDs de múltiples dígitos:** La conversión numérica a cadena de texto invierte correctamente los residuos para garantizar que los números de más de una cifra mantengan su integridad antes de aplicar el cifrado.
* **Sobreescritura limpia del binario:** El archivo binario de salida se abre en modo binario de escritura (`"wb"`), asegurando que cada ejecución reemplace la versión anterior de forma consistente.

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
### ventas.cpp
**Responsable:** Karen Marca Villalobos

**Descripción:** Programa que realiza la carga de las ventas del día. Valida a los mozos con su clave encriptada, registra cada venta con su comisión, descuenta el stock vendido del inventario y deja la planilla del día ordenadas por el numero del mozo.

## Cómo correr
Necesita los mismos archivos que genera normalizacion.cpp: "mozos.dat" y "inventario.dat"

El programa corre de la siguiente manera:

**1. Fecha del día -->** Ingresar la fecha correspondiente, en formato 'DD-MM-AAAA', una sola vez al principio 

**2. Por cada venta:** 

        - Numero de mozo y para terminar la carga 0 
    
        - Clave del mozo 
    
        - Si el login es válido: pedirá el código del producto y la cantidad vendida.
     
**-->** Si la planilla del día ya existía, es decir, si el programa se corre mas de una vez en la misma jornada, el programa la carga primero y agrega las ventas nuevas sin perder las anteriores.

**- Resultado:** se creará o actualizará "comandas_DD-MM-AAAA.dat" con todas las ventas ordenadas por el ID del mozo y descontará el stock ventido en "inventario.dat".

*Ejemplo de uso*

=== Carga de ventas del dia ===

Fecha (DD-MM-AAAA): 01-10-2026

Ingrese numero de mozo (0 para terminar): 1

Clave: 1

Bienvenido/a, Ana Lopez ! 


Codigo de producto: 101

Cantidad: 2

 -Venta cargada- 

 Comision: 300


Ingrese el numero del mozo (0 para terminar): 0


Planilla del dia 01-10-2026 grabada con 1 ventas en total 


## Cómo se resolvió
- **Login:** la clave que tipea el mozo se encripta con el mismo corrimiento (K = 5) que usa "normalizacion.cpp" y se compara contra la guardada en "mozos.dat". El mozo se busca con **búsqueda secuencial** ('buscarMozo'), ya que "mozos.dat" es chico.
- **Producto:** "inventario.dat" está ordenado por código pero con huecos, se aplica **búsqueda binaria** ('buscarProducto') sobre el archivo.
- **Comisión:** 10% del precio del producto por la cantidad vendida ('TASA_COMISION').
- **Stock:** se descuenta con **edición in situ** ("rb+" , función "descontarStock"): se lee el producto, se resta la cantidad vendida, se retrocede con 'fseek' y se regraba solo ese registro.
- **Planilla del día:** las ventas se acumulan en un array en memoria (tam.físico 'MAX_VENTAS_DIA', tam.lógico 'len'). Si ya existía una planilla de ese día, "cargarExistente" la carga primero. Al terminar, "ordenarPorMozo" ordena por inserción y "grabarPlanilla" graba el archivo completo.

### Casos contemplados
- **Mozo inexistente o clave incorrecta:** avisa ("No existe un mozo con ese numero." / "Clave incorrecta.") y vuelve a pedir el número de mozo, sin cortar el programa.
- **Producto inexistente:** avisa y no registra la venta.
- **Stock insuficiente:** avisa cuántas unidades quedan y no registra la venta ni descuenta nada.
- **Planilla del día que todavía no existe:** se crea desde cero.
- **Planilla del día que ya existía:** se leen las ventas previas y se agregan las nuevas, sin perder lo ya cargado.

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
  
-------------------------------------------------
## resumen.cpp
**Responsable:** Ayelen Alcon 

**Descripción:** Programa que genera el resumen del cierre a partir del archivo generado por `cierre.cpp` (`comandas_semana_sX-mm.dat`). Muestra por pantalla la cantidad de productos vendidos y la comisión correspondiente a cada mozo, junto con el total de productos vendidos por el buffet.

### Cómo correr
Se ejecuta de forma estricta después de `cierre.cpp`. El programa le va a pedir al usuario que ingrese los siguientes datos:
1. Número de **semana** (del 1 al 5).
2. Número de **mes** (de 1 al 12).

**- Resultado:** genera un reporte en pantalla con la información detallada de las ventas y comisiones de cada mozo junto con el total del buffet

### Cómo se resolvió
Para evitar que el usuario tenga que escribir manualmente el nombre completo del archivo (`comandas_semana_sX-mm.dat`), el mismo se construye dinámicamente utilizando `sprintf` con el especificador `%02d` para asegurar que el mes siempre mantenga el formato de dos dígitos (ej. 06 para junio).

Se implementa un bucle `while` para verificar que el archivo exista mediante `fopen` en modo lectura binaria (`"rb"`). Si el archivo semanal no existe (por ejemplo, si no se ejecutó previamente `cierre.cpp` o se ingresaron mal los datos), el programa informa el error y vuelve a solicitar los datos sin interrumpir ni romper la ejecución.

El procesamiento del archivo semanal se realiza aplicando un **corte de control** mediante la clave `idMozo`, lo que permite, por cada mozo, acumular el total de productos vendidos y la comisión, imprimiendo un subtotal al cambiar de mozo e incrementando la cantidad de productos vendidos por el buffet.

### Casos contemplados
- **Archivo inexistente o datos mal ingresados:** Si ingresa una semana o un mes que no existe, o si no se ejecutó previamente `cierre.cpp`, advierte el error en pantalla y vuelve a pedir los datos.
- **Archivo semanal vacío:** Si `cierre.cpp` generó un archivo semanal con cero ventas, el corte de control no se ejecuta e imprime directamente en pantalla el total del buffet: 0

