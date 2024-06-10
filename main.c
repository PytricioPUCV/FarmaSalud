/*
 * Pontificia Universidad Católica de Valparaiso
 *
 * https://github.com/gioahumada/farmaSalud
 *
 * Este archivo es parte del proyecto de la asignatura INF2223-1 - Estructuras de Datos
 * y a sido diseñado por estudiantes de la PUCV para el desarrollo de un sistema de
 * gestión de inventario y ventas para una farmacia.
 *
 * Autores:
 * - Giovanni Ahumada
 * - Bastian Mejias
 * - Simon Vera
 * - Benjamín Pizarro
 * - Patricio Hernandez
 *
 * Fecha de creación: 7 de junio de 2021
 *
 * EL PROGRAMA A SIDO ADAPTADO PARA FUNCIONAR EN TURBO C
 * COMPILADOR UTILIZADO: Turbo C 2.01 EJECUTADO EN WINDOWS XP
 * PROYECTO INICIALMENTE DESARROLLADO EN MACOS SONOMA UTILIZANDO EL COMPILADOR GCC
 * Y CLANG, ADEMAS DE VISUAL STUDIO CODE.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PRODUCTOS_POR_CLIENTE 10000
#define MAX_ENVIOS 30000

/* ----------
    Estructura Principal
*/

struct FarmaSalud
{
    struct NodoClientes *clientes;
    struct NodoSucursales *sucursales;
    struct NodoProveedor *proveedores;
};

/* ----------
    Izquierda
*/

// Lista doblemente Enlazada Nodo Fantasma
struct NodoClientes
{
    struct Clientes *datosClientes;
    struct NodoClientes *ant, *sig;
};

struct Clientes
{
    int id;
    char *rutCliente;
    int edadCliente;
    int afiliado;
    struct Producto *comprasCliente[MAX_PRODUCTOS_POR_CLIENTE]; // Array de punteros a Productos
    int numCompras; // Número de compras
};

/* ----------
    Centro
*/

// Lista Circular Doblemente Enlazada
struct NodoSucursales
{
    struct Sucursal *datosSucursal;
    struct NodoSucursales *ant, *sig;
};

struct Sucursal
{
    int id;
    char *nombre;
    char *direccion;
    int cantidadDeVentas;
    int capacidadAlmacenamiento;
    struct NodoProducto *productos;
    struct NodoProducto *productosVendidos;
    char *registrosEnvios[MAX_ENVIOS]; // Array de strings para los registros de envíos
    int numRegistros; // Número de registros
};


// Lista doblemente enlazada con nodo fantasma
struct NodoProducto
{
    struct Producto *datosProducto;
    struct NodoProducto *ant, *sig;
};

struct Producto
{
    char codigo[10];
    char *nombreProducto;
    char *descripcion;
    char *categoria;
    int precio;
    char *idProveedor;
    char *lote;
    char *fechaCaducidad; // 02
    int cantidad;
    int requiereReceta;
};

/* ----------
    Derecha
*/

// Lista doblemente enlazada nodo fantasma
struct NodoProveedor
{
    struct Proveedor *datosProveedor;
    struct NodoProveedor *ant, *sig;
};

struct Proveedor
{
    int id;
    char *nombre;
    char *direccion;
    char *telefono;
    struct NodoArbolProducto *productos;
};

// Arbol de busqueda binaria
struct NodoArbolProducto
{
    struct Producto *datosProducto;
    struct NodoArbolProducto *izq, *der;
};
struct NodoClientes* crearClienteConsole(int id, char* rutCliente, int edadCliente, int afiliado, struct Producto **comprasCliente, int numCompras) {
    struct NodoClientes* nuevoNodo;
    struct Clientes* nuevoCliente;
    char* rutClienteCopy;
    int i;

    nuevoNodo = (struct NodoClientes*)malloc(sizeof(struct NodoClientes));
    nuevoCliente = (struct Clientes*)malloc(sizeof(struct Clientes));

    nuevoCliente->id = id;

    // Implementación manual de strdup
    rutClienteCopy = (char*)malloc(strlen(rutCliente) + 1);
    strcpy(rutClienteCopy, rutCliente);
    nuevoCliente->rutCliente = rutClienteCopy;

    nuevoCliente->edadCliente = edadCliente;
    nuevoCliente->afiliado = afiliado;
    nuevoCliente->numCompras = numCompras;

    // Inicializar el array de compras con NULL
    for (i = 0; i < MAX_PRODUCTOS_POR_CLIENTE; i++) {
        nuevoCliente->comprasCliente[i] = NULL;
    }
    // Copiar los productos existentes
    for (i = 0; i < numCompras; i++) {
        nuevoCliente->comprasCliente[i] = comprasCliente[i];
    }

    nuevoNodo->datosClientes = nuevoCliente;
    nuevoNodo->ant = nuevoNodo->sig = NULL;

    return nuevoNodo;
}
void agregarClienteConsole(struct FarmaSalud *farmacia, struct NodoClientes *nuevoCliente) {
    if (farmacia->clientes == NULL) {
        farmacia->clientes = nuevoCliente;
    } else {
        struct NodoClientes *temp = farmacia->clientes;
        while (temp->sig != NULL) {
            temp = temp->sig;
        }
        temp->sig = nuevoCliente;
        nuevoCliente->ant = temp;
    }
}

char* my_strdup(const char* str) {
    char* dup = (char*)malloc(strlen(str) + 1);
    if (dup != NULL) {
        strcpy(dup, str);
    }
    return dup;
}

struct NodoSucursales* crearSucursalConsole(int id, char* nombre, char* direccion, int capacidadAlmacenamiento) {
    int i;
    struct NodoSucursales* nuevoNodo = (struct NodoSucursales*)malloc(sizeof(struct NodoSucursales));
    struct Sucursal* nuevaSucursal = (struct Sucursal*)malloc(sizeof(struct Sucursal));

    if (nuevoNodo == NULL || nuevaSucursal == NULL) {
        exit(1);
    }

    nuevaSucursal->id = id;
    nuevaSucursal->nombre = my_strdup(nombre);
    nuevaSucursal->direccion = my_strdup(direccion);
    nuevaSucursal->cantidadDeVentas = 0;
    nuevaSucursal->capacidadAlmacenamiento = capacidadAlmacenamiento;
    nuevaSucursal->productos = NULL;
    nuevaSucursal->productosVendidos = NULL;
    nuevaSucursal->numRegistros = 0;
    for (i = 0; i < MAX_ENVIOS; i++) {
        nuevaSucursal->registrosEnvios[i] = NULL;
    }

    nuevoNodo->datosSucursal = nuevaSucursal;
    nuevoNodo->ant = nuevoNodo->sig = nuevoNodo;

    return nuevoNodo;
}

void agregarSucursalConsole(struct FarmaSalud *farmacia, struct NodoSucursales *nuevaSucursal) {
    if (farmacia->sucursales == NULL) {
        farmacia->sucursales = nuevaSucursal;
    } else {
        struct NodoSucursales *temp = farmacia->sucursales;
        while (temp->sig != farmacia->sucursales) {
            temp = temp->sig;
        }
        temp->sig = nuevaSucursal;
        nuevaSucursal->ant = temp;
        nuevaSucursal->sig = farmacia->sucursales;
        farmacia->sucursales->ant = nuevaSucursal;
    }
}

struct NodoProveedor* crearProveedorConsole(int id, char* nombre, char* direccion, char* telefono) {
    struct NodoProveedor* nuevoNodo = (struct NodoProveedor*)malloc(sizeof(struct NodoProveedor));
    struct Proveedor* nuevoProveedor = (struct Proveedor*)malloc(sizeof(struct Proveedor));

    if (nuevoNodo == NULL || nuevoProveedor == NULL) {
        exit(1);
    }

    nuevoProveedor->id = id;
    nuevoProveedor->nombre = my_strdup(nombre);
    nuevoProveedor->direccion = my_strdup(direccion);
    nuevoProveedor->telefono = my_strdup(telefono);
    nuevoProveedor->productos = NULL;

    nuevoNodo->datosProveedor = nuevoProveedor;
    nuevoNodo->ant = nuevoNodo->sig = NULL;

    return nuevoNodo;
}

void agregarProveedorConsole(struct FarmaSalud *farmacia, struct NodoProveedor *nuevoProveedor) {
    if (farmacia->proveedores == NULL) {
        farmacia->proveedores = nuevoProveedor;
    } else {
        struct NodoProveedor *temp = farmacia->proveedores;
        while (temp->sig != NULL) {
            temp = temp->sig;
        }
        temp->sig = nuevoProveedor;
        nuevoProveedor->ant = temp;
    }
}

struct Producto* crearProductoFalso(char* codigo, char* nombre, char* descripcion, char* categoria, int precio, char* idProveedor, char* lote, char* fechaCaducidad, int cantidad, int requiereReceta) {
    struct Producto* nuevoProducto = (struct Producto*)malloc(sizeof(struct Producto));
    if (nuevoProducto == NULL) {
        exit(1);
    }

    strcpy(nuevoProducto->codigo, codigo);
    nuevoProducto->nombreProducto = strdup(nombre);
    nuevoProducto->descripcion = strdup(descripcion);
    nuevoProducto->categoria = strdup(categoria);
    nuevoProducto->precio = precio;
    nuevoProducto->idProveedor = strdup(idProveedor);
    nuevoProducto->lote = strdup(lote);
    nuevoProducto->fechaCaducidad = strdup(fechaCaducidad);
    nuevoProducto->cantidad = cantidad;
    nuevoProducto->requiereReceta = requiereReceta;

    return nuevoProducto;
}

void agregarProductoAProveedor(struct Proveedor* proveedor, struct Producto* producto) {
    struct NodoArbolProducto* nuevoNodo = (struct NodoArbolProducto*)malloc(sizeof(struct NodoArbolProducto));
    if (nuevoNodo == NULL) {
        exit(1);
    }
    nuevoNodo->datosProducto = producto;
    nuevoNodo->izq = nuevoNodo->der = NULL;

    if (proveedor->productos == NULL) {
        proveedor->productos = nuevoNodo;
    } else {
        struct NodoArbolProducto* actual = proveedor->productos;
        struct NodoArbolProducto* padre = NULL;

        while (actual != NULL) {
            padre = actual;
            if (strcmp(producto->codigo, actual->datosProducto->codigo) < 0) {
                actual = actual->izq;
            } else {
                actual = actual->der;
            }
        }

        if (strcmp(producto->codigo, padre->datosProducto->codigo) < 0) {
            padre->izq = nuevoNodo;
        } else {
            padre->der = nuevoNodo;
        }
    }
}

struct NodoProducto* crearNodoProducto(struct Producto* producto) {
    struct NodoProducto* nuevoNodo = (struct NodoProducto*)malloc(sizeof(struct NodoProducto));
    nuevoNodo->datosProducto = producto;
    nuevoNodo->ant = nuevoNodo->sig = NULL;
    return nuevoNodo;
}

void agregarProductoASucursal(struct Sucursal* sucursal, struct Producto* producto) {
    // Crear nodo producto
    struct NodoProducto* nuevoNodo = crearNodoProducto(producto);

    if (sucursal == NULL || producto == NULL) {
        return;
    }

    // Insertar al principio de la lista de productos de la sucursal
    if (sucursal->productos == NULL) {
        sucursal->productos = nuevoNodo;
    } else {
        nuevoNodo->sig = sucursal->productos;
        sucursal->productos->ant = nuevoNodo;
        sucursal->productos = nuevoNodo;
    }
}

void cls() {
    int i;
    for(i=0;i<20;i++) printf("\n");
}

void pause() {
    printf("\nPresione Enter para continuar...\n");
    while (getchar() != '\n'); // Limpiar el búfer de entrada
    getchar(); // Esperar a que se presione Enter
}

// Funciones Menu

// Benja v v v v v v
// Controlador

struct NodoClientes* obtenerClienteActual(struct FarmaSalud *farmacia) {
    return farmacia->clientes;
}

struct NodoClientes* obtenerClienteSiguiente(struct NodoClientes *nodoActual) {
    return nodoActual->sig;
}

// Vista
void imprimirCliente(struct NodoClientes *nodoActual) {
    int i;
    struct Producto *producto;

    printf("ID: %d\n", nodoActual->datosClientes->id);
    printf("RUT: %s\n", nodoActual->datosClientes->rutCliente);
    printf("Edad: %d\n", nodoActual->datosClientes->edadCliente);
    if((nodoActual->datosClientes->afiliado)==1)
    {
        printf("Afiliado: Si\n");
    }
    else
    {
        printf("Afiliado: No\n");
    }

    if (nodoActual->datosClientes->numCompras > 0) {
        printf("Compras:\n");
        for (i = 0; i < nodoActual->datosClientes->numCompras; i++) {
            producto = nodoActual->datosClientes->comprasCliente[i];
            if (producto != NULL) {
                printf("\t%s - %s\n", producto->codigo, producto->nombreProducto);
            }
        }
        printf("\n");
    } else {
        printf("Cliente sin compras\n\n");
    }
}

void leerClientes(struct FarmaSalud *farmacia) {
    struct NodoClientes *nodoActual;

    cls();
    nodoActual = obtenerClienteActual(farmacia);
    while (nodoActual != NULL) {
        imprimirCliente(nodoActual);
        nodoActual = obtenerClienteSiguiente(nodoActual);
    }
    pause();
}
void agregarCliente(struct FarmaSalud *farmacia)
{
    int maxId;
    int i;
    struct NodoClientes *temp = farmacia->clientes;
    struct NodoClientes *nuevoNodo = (struct NodoClientes *)malloc(sizeof(struct NodoClientes));
    maxId = 0;
    nuevoNodo->datosClientes = (struct Clientes *)malloc(sizeof(struct Clientes));
    cls();
    while (temp != NULL) {
        if (temp->datosClientes->id > maxId) {
            maxId = temp->datosClientes->id;
        }
        temp = temp->sig;
    }
    nuevoNodo->datosClientes->id = maxId + 1;

    printf("Ingrese RUT del cliente: ");
    nuevoNodo->datosClientes->rutCliente = (char *)malloc(20 * sizeof(char));
    scanf("%s", nuevoNodo->datosClientes->rutCliente);
    printf("Ingrese edad del cliente: ");
    scanf("%d", &nuevoNodo->datosClientes->edadCliente);
    printf("¿Es afiliado? (1-Si, 0-No): ");
    scanf("%d", &nuevoNodo->datosClientes->afiliado);

    for (i = 0; i < MAX_PRODUCTOS_POR_CLIENTE; i++) {
        nuevoNodo->datosClientes->comprasCliente[i] = NULL;
    }
    nuevoNodo->datosClientes->numCompras = 0;

    nuevoNodo->ant = NULL;
    nuevoNodo->sig = NULL;
    if (farmacia->clientes == NULL) {
        farmacia->clientes = nuevoNodo;
    } else {
        temp = farmacia->clientes;
        while (temp->sig != NULL) {
            temp = temp->sig;
        }
        temp->sig = nuevoNodo;
        nuevoNodo->ant = temp;
    }
    cls();
    printf("Cliente agregado con éxito.\n");
    pause();
}

void eliminarCliente(struct FarmaSalud *farmacia) {
    int idCliente;
    struct NodoClientes *clienteActual = farmacia->clientes;
    struct NodoClientes *clienteAnterior = NULL;
    cls();
    printf("Ingrese el ID del cliente que desea eliminar: ");
    scanf("%d", &idCliente);
    // Buscar el cliente por su ID
    while (clienteActual != NULL && clienteActual->datosClientes->id != idCliente) {
        clienteAnterior = clienteActual;
        clienteActual = clienteActual->sig;
    }

    if (clienteActual == NULL) {
        printf("Cliente no encontrado.\n");
        pause();
        return;
    }

    // Eliminar el cliente de la lista
    if (clienteAnterior == NULL) {
        // El cliente a eliminar es el primer nodo de la lista
        farmacia->clientes = clienteActual->sig;
    } else {
        clienteAnterior->sig = clienteActual->sig;
        if (clienteActual->sig != NULL) {
            clienteActual->sig->ant = clienteAnterior;
        }
    }

    // Liberar memoria asignada al cliente eliminado
    // En TurboC, no es necesario liberar memoria manualmente
    // Puedes simplemente sobrescribir la estructura con valores nulos o vacíos
    // No estamos utilizando malloc para crear las estructuras de datos, así que no necesitamos free.

    // Establecer valores nulos o vacíos en la estructura del cliente eliminado
    clienteActual->datosClientes->id = 0; // o cualquier otro valor que indique que el cliente ha sido eliminado
    // Puedes repetir este proceso para otros campos que necesiten ser limpiados

    cls();
    printf("Cliente eliminado con éxito.\n");
    pause();
}


// Simon v v v v v v
int espacioRestante(struct Sucursal *sucursal) {
    int totalProductos = 0;
    struct NodoProducto *nodoActual;

    if (sucursal == NULL) {
        return -1;
    }

    nodoActual = sucursal->productos;

    while (nodoActual != NULL) {
        totalProductos += nodoActual->datosProducto->cantidad;
        nodoActual = nodoActual->sig;
    }

    return 10000 - totalProductos;
}

struct NodoSucursales* obtenerSucursales(struct FarmaSalud *farmacia) {
    return farmacia->sucursales;
}

void mostrarSucursal(struct NodoSucursales *nodoActual) {
    int i;
    printf("ID: %d\n", nodoActual->datosSucursal->id);
    printf("Nombre: %s\n", nodoActual->datosSucursal->nombre);
    printf("Dirección: %s\n", nodoActual->datosSucursal->direccion);
    printf("Cantidad de Ventas: %d\n", nodoActual->datosSucursal->cantidadDeVentas);
    printf("Número de Registros de Envíos: %d\n", nodoActual->datosSucursal->numRegistros);
    for (i = 0; i < nodoActual->datosSucursal->numRegistros; i++) {
        printf("\tRegistro de Envío %d: %s\n", i + 1, nodoActual->datosSucursal->registrosEnvios[i]);
    }

    if (nodoActual->datosSucursal->productos != NULL) {
        struct NodoProducto *productoNodo = nodoActual->datosSucursal->productos;
        printf("Productos:\n");
        while (productoNodo != NULL) {
            printf("\t%s - %s", productoNodo->datosProducto->codigo, productoNodo->datosProducto->nombreProducto);
            printf("\tCantidad en stock: %d", productoNodo->datosProducto->cantidad);
            printf("\tFecha de caducidad: %s", productoNodo->datosProducto->fechaCaducidad);
            printf("\tLote: %s\n", productoNodo->datosProducto->lote);

            // Comprobar si el producto tiene bajo stock
            if (productoNodo->datosProducto->cantidad < 10) {
                printf("\t*** ALERTA: Bajo stock! ***\n");
            }

            productoNodo = productoNodo->sig;
        }
        printf("\n");
    } else {
        printf("Sucursal sin productos\n");
    }
}

void mostrarEspacioRestante(int espacio) {
    printf("Espacio restante en la sucursal: %d\n\n", espacio);
}

void leerSucursales(struct FarmaSalud *farmacia) {
    struct NodoSucursales *nodoActual = obtenerSucursales(farmacia);
    int espacio;

    cls();
    if (nodoActual == NULL) {
        printf("No hay sucursales registradas.\n");
        pause();
        return;
    }
    do {
        mostrarSucursal(nodoActual);
        espacio = espacioRestante(nodoActual->datosSucursal);
        mostrarEspacioRestante(espacio);
        nodoActual = nodoActual->sig;
    } while (nodoActual != farmacia->sucursales);
    pause();
}

void agregarSucursal(struct FarmaSalud *farmacia, struct NodoSucursales *nuevaSucursal) {
    if (farmacia->sucursales == NULL) {
        farmacia->sucursales = nuevaSucursal;
        nuevaSucursal->ant = nuevaSucursal->sig = nuevaSucursal;
    } else {
        struct NodoSucursales *ultimo = farmacia->sucursales->ant;
        ultimo->sig = nuevaSucursal;
        nuevaSucursal->ant = ultimo;
        nuevaSucursal->sig = farmacia->sucursales;
        farmacia->sucursales->ant = nuevaSucursal;
    }
}

void leerDatosSucursal(struct Sucursal *nuevaSucursal){
    int i;
    cls();
    printf("Ingrese ID de la sucursal: ");
    scanf("%d", &nuevaSucursal->id);
    getchar(); // Limpiar el buffer de entrada después de leer un entero

    nuevaSucursal->nombre = (char *)malloc(50 * sizeof(char));
    printf("Ingrese nombre de la sucursal: ");
    scanf(" %[^\n]", nuevaSucursal->nombre);
    getchar(); // Limpiar el buffer de entrada después de leer un string

    nuevaSucursal->direccion = (char *)malloc(100 * sizeof(char));
    printf("Ingrese dirección de la sucursal: ");
    scanf(" %[^\n]", nuevaSucursal->direccion);
    getchar(); // Limpiar el buffer de entrada después de leer un string

    nuevaSucursal->cantidadDeVentas = 0; // Inicializar a 0
    nuevaSucursal->capacidadAlmacenamiento = 0; // Cambiar si se necesita un valor específico
    nuevaSucursal->productos = NULL;
    nuevaSucursal->productosVendidos = NULL; // Inicializar a NULL
    nuevaSucursal->numRegistros = 0; // Inicializar a 0
    for (i = 0; i < MAX_ENVIOS; i++) {
        nuevaSucursal->registrosEnvios[i] = NULL; // Inicializar a NULL
    }
}

void crearSucursal(struct FarmaSalud *farmacia) {
    struct NodoSucursales *nuevoNodo = (struct NodoSucursales*)malloc(sizeof(struct NodoSucursales));
    struct Sucursal *nuevaSucursal = (struct Sucursal*)malloc(sizeof(struct Sucursal));

    nuevoNodo->datosSucursal = nuevaSucursal;
    nuevoNodo->ant = nuevoNodo->sig = NULL;

    leerDatosSucursal(nuevaSucursal);
    agregarSucursal(farmacia, nuevoNodo);

    cls();
    printf("Sucursal agregada con éxito.\n");
    pause();
}

int eliminarSucursal(struct FarmaSalud *farmacia, int idEliminar) {
    int i;
    struct NodoSucursales *temp, *prev;

    temp = farmacia->sucursales;
    if (temp == NULL) {
        return 0; // No hay sucursales
    }

    struct NodoSucursales *inicio = temp;

    do {
        if (temp->datosSucursal->id == idEliminar) {
            // Manejar los casos especiales para el primer y último nodo
            if (temp->ant == temp && temp->sig == temp) {
                farmacia->sucursales = NULL;
            } else {
                prev = temp->ant;
                prev->sig = temp->sig;
                temp->sig->ant = prev;
                if (temp == farmacia->sucursales) {
                    farmacia->sucursales = temp->sig;
                }
            }

            // Liberar la memoria asociada a la sucursal
            free(temp->datosSucursal->nombre);
            free(temp->datosSucursal->direccion);
            for (i = 0; i < temp->datosSucursal->numRegistros; i++) {
                free(temp->datosSucursal->registrosEnvios[i]);
            }
            free(temp->datosSucursal);
            free(temp);

            return 1; // Eliminación exitosa
        }
        temp = temp->sig;
    } while (temp != inicio);

    return 0; // No se encontró la sucursal
}

void leerIdSucursal(int *idEliminar) {
    cls();
    printf("Ingrese el ID de la sucursal que desea eliminar: ");
    scanf("%d", idEliminar);
    getchar(); // Limpiar el buffer de entrada después de leer un entero
}

void mostrarMensajeNoSucursales() {
    printf("No hay sucursales registradas.\n");
    pause();
}

void mostrarMensajeSucursalNoEncontrada() {
    cls();
    printf("Sucursal no encontrada.\n");
    pause();
}

void mostrarMensajeSucursalEliminada() {
    cls();
    printf("Sucursal eliminada con éxito.\n");
    pause();
}

void eliminarSucursalView(struct FarmaSalud *farmacia) {
    int idEliminar;

    leerIdSucursal(&idEliminar);

    struct NodoSucursales *temp = farmacia->sucursales;
    if (temp == NULL) {
        mostrarMensajeNoSucursales();
        return;
    }

    int resultado = eliminarSucursal(farmacia, idEliminar);

    if (resultado == 0) {
        mostrarMensajeSucursalNoEncontrada();
    } else {
        mostrarMensajeSucursalEliminada();
    }
}

void agregarProveedor(struct FarmaSalud *farmacia, struct NodoProveedor *nuevoProveedor) {
    if (farmacia->proveedores == NULL) {
        farmacia->proveedores = nuevoProveedor;
        nuevoProveedor->ant = nuevoProveedor->sig = NULL;
    } else {
        struct NodoProveedor *ultimo = farmacia->proveedores;
        while (ultimo->sig != NULL) {
            ultimo = ultimo->sig;
        }
        ultimo->sig = nuevoProveedor;
        nuevoProveedor->ant = ultimo;
        nuevoProveedor->sig = NULL;
    }
}

int existeProveedor(struct FarmaSalud *farmacia, int id) {
    struct NodoProveedor *actual = farmacia->proveedores;
    while (actual != NULL) {
        if (actual->datosProveedor->id == id) {
            return 1;  // Proveedor encontrado
        }
        actual = actual->sig;
    }
    return 0;  // Proveedor no encontrado
}


void leerDatosProveedor(int *id, char **nombre, char **direccion, char **telefono) {
    int i;
    char c;
    i = 0;
    cls();
    printf("Ingrese ID del proveedor: ");
    scanf("%d", id);
    getchar();  // Consumir el salto de línea que queda en el buffer

    *nombre = (char *)malloc(50 * sizeof(char));
    if (*nombre == NULL) {
        printf("Error al asignar memoria para el nombre del proveedor.\n");
        return;
    }
    printf("Ingrese nombre del proveedor: ");
    getchar(); // Consumir el carácter de nueva línea
    while ((c = getchar()) != '\n' && i < 49) {
        (*nombre)[i++] = c;
    }
    (*nombre)[i] = '\0';

    *direccion = (char *)malloc(100 * sizeof(char));
    if (*direccion == NULL) {
        printf("Error al asignar memoria para la dirección del proveedor.\n");
        free(*nombre);
        return;
    }
    printf("Ingrese dirección del proveedor: ");
    getchar(); // Consumir el carácter de nueva línea
    i = 0;
    while ((c = getchar()) != '\n' && i < 99) {
        (*direccion)[i++] = c;
    }
    (*direccion)[i] = '\0';

    *telefono = (char *)malloc(15 * sizeof(char));
    if (*telefono == NULL) {
        printf("Error al asignar memoria para el teléfono del proveedor.\n");
        free(*nombre);
        free(*direccion);
        return;
    }
    printf("Ingrese teléfono del proveedor: ");
    getchar(); // Consumir el carácter de nueva línea
    i = 0;
    while ((c = getchar()) != '\n' && i < 14) {
        (*telefono)[i++] = c;
    }
    (*telefono)[i] = '\0';
}

void mostrarMensajeProveedorAgregado() {
    cls();
    printf("Proveedor agregado con éxito.\n");
    pause();
}

void mostrarMensajeProveedorExiste() {
    cls();
    printf("Error: Ya existe un proveedor con el mismo ID.\n");
    pause();
}

void crearProveedorView(struct FarmaSalud *farmacia) {
    int id;
    char *nombre, *direccion, *telefono;

    leerDatosProveedor(&id, &nombre, &direccion, &telefono);

    if (nombre == NULL || direccion == NULL || telefono == NULL) {
        return;
    }

    if (existeProveedor(farmacia, id)) {
        mostrarMensajeProveedorExiste();
        free(nombre);
        free(direccion);
        free(telefono);
        return;
    }

    struct NodoProveedor *nuevoNodo = (struct NodoProveedor*)malloc(sizeof(struct NodoProveedor));
    if (!nuevoNodo) {
        printf("Error al asignar memoria para nuevo nodo proveedor.\n");
        free(nombre);
        free(direccion);
        free(telefono);
        return;
    }

    struct Proveedor *nuevoProveedor = (struct Proveedor*)malloc(sizeof(struct Proveedor));
    if (!nuevoProveedor) {
        free(nuevoNodo);
        printf("Error al asignar memoria para nuevo proveedor.\n");
        free(nombre);
        free(direccion);
        free(telefono);
        return;
    }

    nuevoProveedor->id = id;
    nuevoProveedor->nombre = nombre;
    nuevoProveedor->direccion = direccion;
    nuevoProveedor->telefono = telefono;
    nuevoProveedor->productos = NULL;

    nuevoNodo->datosProveedor = nuevoProveedor;
    nuevoNodo->ant = nuevoNodo->sig = NULL;

    agregarProveedor(farmacia, nuevoNodo);

    mostrarMensajeProveedorAgregado();
}
void mostrarProveedores(struct FarmaSalud *farmacia) {
    struct NodoProveedor *proveedorActual;
    proveedorActual = farmacia->proveedores;
    printf("Proveedores disponibles:\n");
    while (proveedorActual != NULL) {
        printf("ID: %d, Nombre: %s\n", proveedorActual->datosProveedor->id, proveedorActual->datosProveedor->nombre);
        proveedorActual = proveedorActual->sig;
    }
    printf("\n");
}

// Bastian v v v v v v
void mostrarProductosProveedor(struct NodoArbolProducto *nodoProducto) {
    if (nodoProducto == NULL) {
        return;
    }

    mostrarProductosProveedor(nodoProducto->izq);

    struct Producto *producto = nodoProducto->datosProducto;
    char requiereReceta[4];

    if (producto->requiereReceta) {
        strcpy(requiereReceta, "Sí");
    } else {
        strcpy(requiereReceta, "No");
    }

    printf("%-10s", producto->codigo);
    printf("%-40s", producto->nombreProducto);
    printf("$%-20d", producto->precio);
    printf("%-18s\n", requiereReceta);

    mostrarProductosProveedor(nodoProducto->der);
}

void mostrarProveedor(struct Proveedor *proveedor) {
    printf("ID: %d\n", proveedor->id);
    printf("Nombre: %s\n", proveedor->nombre);
    printf("Dirección: %s\n", proveedor->direccion);
    printf("Teléfono: %s\n", proveedor->telefono);

    // Mostrar productos del proveedor
    if (proveedor->productos == NULL) {
        printf("No hay productos asociados a este proveedor.\n\n");
    } else {
        printf("Productos:\n\n");
        printf("ID          Nombre                                    Precio                Requiere receta\n\n");
        mostrarProductosProveedor(proveedor->productos);
        printf("\n");
    }
}

void leerProveedores(struct FarmaSalud *farmacia) {
    cls();
    struct NodoProveedor *nodoActual = farmacia->proveedores;
    if (nodoActual == NULL) {
        printf("No hay proveedores registrados.\n");
        pause();
        return;
    }

    while (nodoActual != NULL) {
        mostrarProveedor(nodoActual->datosProveedor);
        nodoActual = nodoActual->sig;
    }
    pause();
}

void solicitarIdProveedorEliminar(int *idEliminar) {
    printf("Ingrese el ID del proveedor que desea eliminar: ");
    scanf("%d", idEliminar);
}

void mostrarProveedorNoEncontrado() {
    cls();
    printf("Proveedor no encontrado.\n");
    pause();
}

void mostrarProveedorEliminado() {
    cls();
    printf("Proveedor eliminado con éxito.\n");
    pause();
}

int buscarYEliminarProveedor(struct FarmaSalud *farmacia, int idEliminar) {
    struct NodoProveedor *temp = farmacia->proveedores;
    struct NodoProveedor *prev = NULL;

    // Buscar el proveedor con el ID proporcionado
    while (temp != NULL && temp->datosProveedor->id != idEliminar) {
        prev = temp;
        temp = temp->sig;
    }

    // Si no se encontró el proveedor con el ID proporcionado
    if (temp == NULL) {
        return 0; // No encontrado
    }

    // Manejar los casos especiales para el primer y último nodo
    if (prev == NULL) {
        farmacia->proveedores = temp->sig;
    } else {
        prev->sig = temp->sig;
    }
    if (temp->sig != NULL) {
        temp->sig->ant = prev;
    }

    // Liberar la memoria asociada al proveedor
    free(temp->datosProveedor->nombre);
    free(temp->datosProveedor->direccion);
    free(temp->datosProveedor->telefono);
    free(temp->datosProveedor);
    free(temp);

    return 1; // Eliminado con éxito
}

void eliminarProveedor(struct FarmaSalud *farmacia) {
    cls();
    int idEliminar;

    // Pedir al usuario que ingrese el ID del proveedor a eliminar
    solicitarIdProveedorEliminar(&idEliminar);

    // Intentar eliminar el proveedor
    if (!buscarYEliminarProveedor(farmacia, idEliminar)) {
        mostrarProveedorNoEncontrado();
        return;
    }
    mostrarProveedorEliminado();
}

void quitarNuevaLinea(char *cadena) {
    char *pos;
    if ((pos = strchr(cadena, '\n')) != NULL) {
        *pos = '\0';
    }
}


void solicitarDatosProducto(struct Producto *producto) {
    cls();
    printf("Ingrese código del producto: ");
    scanf("%s", producto->codigo);
    getchar();
    cls();
    printf("Ingrese nombre del producto: ");
    gets(producto->nombreProducto);
    quitarNuevaLinea(producto->nombreProducto);
    cls();
    printf("Ingrese descripción del producto: ");
    gets(producto->descripcion);
    quitarNuevaLinea(producto->descripcion);
    cls();
    printf("Ingrese categoría del producto: ");
    gets(producto->categoria);
    quitarNuevaLinea(producto->categoria);
    cls();
    printf("Ingrese precio del producto: ");
    scanf("%d", &producto->precio);
    getchar();
    cls();
    printf("\nIngrese ID del proveedor: ");
    scanf("%s", producto->idProveedor);
    cls();
    printf("¿Requiere receta? (1-Sí, 0-No): ");
    scanf("%d", &producto->requiereReceta);
}

struct Producto* crearProducto() {
    struct Producto *nuevoProducto = (struct Producto *)malloc(sizeof(struct Producto));
    nuevoProducto->nombreProducto = (char *)malloc(50 * sizeof(char));
    nuevoProducto->descripcion = (char *)malloc(100 * sizeof(char));
    nuevoProducto->categoria = (char *)malloc(50 * sizeof(char));
    nuevoProducto->idProveedor = (char *)malloc(20 * sizeof(char));
    nuevoProducto->lote = strdup("N/A");
    nuevoProducto->fechaCaducidad = strdup("N/A");
    nuevoProducto->cantidad = -1;
    return nuevoProducto;
}

int agregarProductoProveedorModelo(struct FarmaSalud *farmacia, struct Producto *nuevoProducto) {
    int idProveedorInt = atoi(nuevoProducto->idProveedor);
    struct NodoProveedor *proveedorActual = farmacia->proveedores;
    while (proveedorActual != NULL && proveedorActual->datosProveedor->id != idProveedorInt) {
        proveedorActual = proveedorActual->sig;
    }

    if (proveedorActual == NULL) {
        return 0;
    }

    struct NodoArbolProducto *nuevoNodoProducto = (struct NodoArbolProducto *)malloc(sizeof(struct NodoArbolProducto));
    nuevoNodoProducto->datosProducto = nuevoProducto;
    nuevoNodoProducto->izq = nuevoNodoProducto->der = NULL;

    if (proveedorActual->datosProveedor->productos == NULL) {
        proveedorActual->datosProveedor->productos = nuevoNodoProducto;
    } else {
        struct NodoArbolProducto *actualProducto = proveedorActual->datosProveedor->productos;
        struct NodoArbolProducto *padreProducto = NULL;

        while (actualProducto != NULL) {
            padreProducto = actualProducto;
            if (strcmp(nuevoProducto->codigo, actualProducto->datosProducto->codigo) < 0) {
                actualProducto = actualProducto->izq;
            } else {
                actualProducto = actualProducto->der;
            }
        }

        if (strcmp(nuevoProducto->codigo, padreProducto->datosProducto->codigo) < 0) {
            padreProducto->izq = nuevoNodoProducto;
        } else {
            padreProducto->der = nuevoNodoProducto;
        }
    }
    return 1;
}

void agregarProductoProveedor(struct FarmaSalud *farmacia) {
    struct Producto *nuevoProducto = crearProducto();
    solicitarDatosProducto(nuevoProducto);
    mostrarProveedores(farmacia);
    int resultado = agregarProductoProveedorModelo(farmacia, nuevoProducto);
    if (resultado) {
        cls();
        printf("Producto agregado con éxito al proveedor.\n");
    } else {
        cls();
        printf("Proveedor no encontrado.\n");
        free(nuevoProducto->nombreProducto);
        free(nuevoProducto->descripcion);
        free(nuevoProducto->categoria);
        free(nuevoProducto->idProveedor);
        free(nuevoProducto->lote);
        free(nuevoProducto->fechaCaducidad);
        free(nuevoProducto);
    }
    pause();
}

void printIngresarMes() {
    printf("Ingrese el mes (MM): ");
}

void printIngresarDia() {
    printf("Ingrese el dia (DD): ");
}

void printSucursalNoEncontrada() {
    printf("Sucursal no encontrada.\n");
}

void printErrorCapacidadMaxima() {
    cls();
    printf("Error: No se puede agregar más registros de envíos, capacidad máxima alcanzada.\n");
    pause();
}

void printRegistroEnvio(const char *registro) {
    printf("%s\n", registro);
}

void printNoSucursalesRegistradas() {
    cls();
    printf("No hay sucursales registradas.\n");
    pause();
}

void printSucursalInfo(struct Sucursal *sucursal) {
    int i;
    int espacio;
    espacio = 10000 - espacioRestante(sucursal);
    printf("ID: %d\n", sucursal->id);
    printf("Nombre: %s\n", sucursal->nombre);
    printf("Dirección: %s\n", sucursal->direccion);
    printf("Cantidad de Ventas: %d\n", sucursal->cantidadDeVentas);
    printf("Capacidad de Almacenamiento: %d / 10000\n", espacio);
    printf("Número de Registros de Envíos: %d\n", sucursal->numRegistros);
    for (i = 0; i < sucursal->numRegistros; i++) {
        printf("\tRegistro de Envío %d: %s\n", i + 1, sucursal->registrosEnvios[i]);
    }
    printf("--------------------------------------------------\n");
}

void printIngreseIDProveedor() {
    printf("Ingrese el ID del proveedor: ");
}

void printProveedorNoEncontrado(int idProveedor) {
    printf("Proveedor con ID %d no encontrado.\n", idProveedor);
}

void printIngreseIDSucursal() {
    printf("Ingrese el ID de la sucursal: ");
}

void printIngreseFechaCaducidad(const char *nombreProducto) {
    printf("Ingrese la fecha de vencimiento para el producto (formato MM/AAAA) [%s]: ", nombreProducto);
}

void printIngreseLote(const char *nombreProducto) {
    printf("Ingrese el lote para el producto [%s]: ", nombreProducto);
}

void printIngreseCantidad(const char *nombreProducto) {
    printf("Ingrese la cantidad para el producto [%s]: ", nombreProducto);
}

void printErrorAsignarMemoriaProducto() {
    cls();
    printf("Error al asignar memoria para el nuevo producto.\n");
    pause();
}

void printErrorAsignarMemoriaNodoProducto() {
    cls();
    printf("Error al asignar memoria para el nuevo nodo de producto.\n");
    pause();
}

void printProductosTransferidos(int idProveedor, int idSucursal) {
    printf("Productos transferidos del proveedor con ID %d a la sucursal con ID %d.\n", idProveedor, idSucursal);
}

// Funciones principales
void obtenerFecha(char *fecha) {
    int dia, mes;
    printIngresarMes();
    scanf("%d", &mes);
    printIngresarDia();
    scanf("%d", &dia);
    sprintf(fecha, "%02d/%02d", mes, dia);
}

void my_strcat(char *dest, const char *src) {
    while (*dest) {
        dest++;
    }
    while (*src) {
        *dest = *src;
        dest++;
        src++;
    }
    *dest = '\0';
}

void agregarRegistroEnvio(struct Sucursal *sucursal, const char *proveedorNombre) {
    if (sucursal->numRegistros >= MAX_ENVIOS) {
        printErrorCapacidadMaxima();
        return;
    }

    char fecha[6];
    obtenerFecha(fecha);

    char registro[200];
    strcpy(registro, "Productos enviados de Proveedor ");
    my_strcat(registro, proveedorNombre);
    my_strcat(registro, " a Sucursal ");
    my_strcat(registro, sucursal->nombre);
    my_strcat(registro, " el día ");
    my_strcat(registro, fecha);

    sucursal->registrosEnvios[sucursal->numRegistros] = (char *)malloc(strlen(registro) + 1);
    if (sucursal->registrosEnvios[sucursal->numRegistros] != NULL) {
        strcpy(sucursal->registrosEnvios[sucursal->numRegistros], registro);
        sucursal->numRegistros++;
    }
}

struct NodoSucursales* buscarSucursalPorID(struct FarmaSalud *farmacia, int idSucursal) {
    struct NodoSucursales *sucursalActual = farmacia->sucursales;
    if (sucursalActual == NULL) {
        return NULL;
    }

    struct NodoSucursales *inicio = sucursalActual;
    do {
        if (sucursalActual->datosSucursal->id == idSucursal) {
            return sucursalActual;
        }
        sucursalActual = sucursalActual->sig;
    } while (sucursalActual != inicio);

    return NULL;
}

struct NodoProveedor* buscarProveedorPorID(struct FarmaSalud *farmacia, int idProveedor) {
    struct NodoProveedor *proveedorActual = farmacia->proveedores;
    while (proveedorActual != NULL && proveedorActual->datosProveedor->id != idProveedor) {
        proveedorActual = proveedorActual->sig;
    }
    return proveedorActual;
}

void mostrarSucursales(struct FarmaSalud *farmacia) {
    struct NodoSucursales *sucursalActual = farmacia->sucursales;
    if (sucursalActual == NULL) {
        printNoSucursalesRegistradas();
        return;
    }

    do {
        printSucursalInfo(sucursalActual->datosSucursal);
        sucursalActual = sucursalActual->sig;
    } while (sucursalActual != farmacia->sucursales);
}

void transferirProductosProveedorASucursal(struct FarmaSalud *farmacia) {
    int idProveedor, idSucursal;
    char fechaCaducidad[11];
    char lote[50];
    int cantidad;

    cls();

    mostrarProveedores(farmacia);
    printIngreseIDProveedor();
    scanf("%d", &idProveedor);

    struct NodoProveedor *proveedorActual = buscarProveedorPorID(farmacia, idProveedor);
    if (proveedorActual == NULL) {
        printProveedorNoEncontrado(idProveedor);
        pause();
        return;
    }

    cls();
    mostrarSucursales(farmacia);
    printIngreseIDSucursal();
    scanf("%d", &idSucursal);

    struct NodoSucursales *sucursalActual = buscarSucursalPorID(farmacia, idSucursal);
    if (sucursalActual == NULL) {
        cls();
        printSucursalNoEncontrada();
        pause();
        return;
    }

    struct NodoArbolProducto *nodoProducto = proveedorActual->datosProveedor->productos;
    while (nodoProducto != NULL) {
        cls();
        printIngreseFechaCaducidad(nodoProducto->datosProducto->nombreProducto);
        scanf("%s", fechaCaducidad);
        cls();
        printIngreseLote(nodoProducto->datosProducto->nombreProducto);
        scanf("%s", lote);
        cls();
        printIngreseCantidad(nodoProducto->datosProducto->nombreProducto);
        scanf("%d", &cantidad);

        struct Producto *nuevoProducto = (struct Producto *)malloc(sizeof(struct Producto));
        if (nuevoProducto == NULL) {
            printErrorAsignarMemoriaProducto();
            pause();
            return;
        }

        strcpy(nuevoProducto->codigo, nodoProducto->datosProducto->codigo);

        // Asignar memoria para las cadenas y copiar
        nuevoProducto->nombreProducto = (char *)malloc(strlen(nodoProducto->datosProducto->nombreProducto) + 1);
        strcpy(nuevoProducto->nombreProducto, nodoProducto->datosProducto->nombreProducto);

        nuevoProducto->descripcion = (char *)malloc(strlen(nodoProducto->datosProducto->descripcion) + 1);
        strcpy(nuevoProducto->descripcion, nodoProducto->datosProducto->descripcion);

        nuevoProducto->categoria = (char *)malloc(strlen(nodoProducto->datosProducto->categoria) + 1);
        strcpy(nuevoProducto->categoria, nodoProducto->datosProducto->categoria);

        nuevoProducto->idProveedor = (char *)malloc(strlen(nodoProducto->datosProducto->idProveedor) + 1);
        strcpy(nuevoProducto->idProveedor, nodoProducto->datosProducto->idProveedor);

        nuevoProducto->lote = (char *)malloc(strlen(lote) + 1);
        strcpy(nuevoProducto->lote, lote);

        nuevoProducto->fechaCaducidad = (char *)malloc(strlen(fechaCaducidad) + 1);
        strcpy(nuevoProducto->fechaCaducidad, fechaCaducidad);

        nuevoProducto->precio = nodoProducto->datosProducto->precio;
        nuevoProducto->cantidad = cantidad;
        nuevoProducto->requiereReceta = nodoProducto->datosProducto->requiereReceta;

        struct NodoProducto *nuevoNodoProducto = (struct NodoProducto *)malloc(sizeof(struct NodoProducto));
        if (nuevoNodoProducto == NULL) {
            printErrorAsignarMemoriaNodoProducto();
            free(nuevoProducto);
            pause();
            return;
        }
        nuevoNodoProducto->datosProducto = nuevoProducto;
        nuevoNodoProducto->ant = nuevoNodoProducto->sig = NULL;

        if (sucursalActual->datosSucursal->productos == NULL) {
            sucursalActual->datosSucursal->productos = nuevoNodoProducto;
        } else {
            struct NodoProducto *temp = sucursalActual->datosSucursal->productos;
            while (temp->sig != NULL) {
                temp = temp->sig;
            }
            temp->sig = nuevoNodoProducto;
            nuevoNodoProducto->ant = temp;
        }

        nodoProducto = nodoProducto->der;
    }

    agregarRegistroEnvio(sucursalActual->datosSucursal, proveedorActual->datosProveedor->nombre);

    cls();
    printProductosTransferidos(idProveedor, idSucursal);
    pause();
    cls();
}
//FUNCION HECHA POR MI/
void printPromptFechaVencimiento(char* nombreProducto) {
    printf("Ingrese la fecha de vencimiento para el producto (formato MM/AAAA) [%s]: ", nombreProducto);
}

void printPromptLote(char* nombreProducto) {
    printf("Ingrese el lote para el producto [%s]: ", nombreProducto);
}

void printPromptCantidad(char* nombreProducto) {
    printf("Ingrese la cantidad para el producto [%s]: ", nombreProducto);
}

void printPromptProveedor(void) {
    printf("Ingrese el ID del proveedor: ");
}

void printPromptSucursal(void) {
    printf("Ingrese el ID de la sucursal: ");
}

void printPromptCodigoProducto(void) {
    printf("Ingrese el codigo del producto a transferir: ");
}

void printProductoCodigoNoEncontrado(char* codigoProducto) {
    printf("Producto con codigo %s no encontrado.\n", codigoProducto);
}

void printTransferenciaExitosa(char* codigoProducto, int idProveedor, int idSucursal) {
    printf("Producto %s transferido del proveedor con ID %d a la sucursal con ID %d.\n", codigoProducto, idProveedor, idSucursal);
}

//void printCamion() {
//    printf("──────▄▌▐▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀​▀▀▀▀▀▀▌\n");
//    printf("───▄▄██▌█ beep beep\n");
//    printf("▄▄▄▌▐██▌█ FarmaSalud Truck \n");
//    printf("███████▌█▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄​▄▄▄▄▄▄▌\n");
//    printf("▀(@)▀▀▀▀▀▀▀(@)(@)▀▀▀▀▀▀▀▀▀▀▀▀▀​▀▀▀▀(@)▀\n");
//}

void obtenerDetallesProducto(char* fechaCaducidad, char* lote, int* cantidad, char* nombreProducto) {
    printPromptFechaVencimiento(nombreProducto);
    scanf("%s", fechaCaducidad);
    printPromptLote(nombreProducto);
    scanf("%s", lote);
    printPromptCantidad(nombreProducto);
    scanf("%d", cantidad);
}

int getch(void) {
    int ch;
    ch = getchar();
    return ch;
}

void mostrarYSeleccionarProveedor(struct FarmaSalud *farmacia, int *idProveedor, struct NodoProveedor **proveedorActual) {
    mostrarProveedores(farmacia);
    printPromptProveedor();
    scanf("%d", idProveedor);

    *proveedorActual = buscarProveedorPorID(farmacia, *idProveedor);
    if (*proveedorActual == NULL) {
        printProveedorNoEncontrado(*idProveedor);
        getch();
    }
}

void mostrarYSeleccionarSucursal(struct FarmaSalud *farmacia, int *idSucursal, struct NodoSucursales **sucursalActual) {
    mostrarSucursales(farmacia);
    printPromptSucursal();
    scanf("%d", idSucursal);

    *sucursalActual = buscarSucursalPorID(farmacia, *idSucursal);
    if (*sucursalActual == NULL) {
        printSucursalNoEncontrada();
        getch();
    }
}

void mostrarYSeleccionarProducto(struct NodoProveedor *proveedorActual, char *codigoProducto, struct Producto **productoSeleccionado) {
    printf("Productos del proveedor:\n");
    struct NodoArbolProducto *nodoProducto = proveedorActual->datosProveedor->productos;
    while (nodoProducto != NULL) {
        printf("Codigo: %s, Nombre: %s\n", nodoProducto->datosProducto->codigo, nodoProducto->datosProducto->nombreProducto);
        nodoProducto = nodoProducto->der;
    }
    printPromptCodigoProducto();
    scanf("%s", codigoProducto);

    nodoProducto = proveedorActual->datosProveedor->productos;
    *productoSeleccionado = NULL;
    while (nodoProducto != NULL) {
        if (strcmp(nodoProducto->datosProducto->codigo, codigoProducto) == 0) {
            *productoSeleccionado = nodoProducto->datosProducto;
            break;
        }
        nodoProducto = nodoProducto->der;
    }

    if (*productoSeleccionado == NULL) {
        printProductoCodigoNoEncontrado(codigoProducto);
        getch();
    }
}

void transferirProductoProveedorASucursal(struct FarmaSalud *farmacia)
{
    int idProveedor, idSucursal;
    char fechaCaducidad[11];
    char lote[50];
    int cantidad;
    char codigoProducto[10];

    cls(); // Limpia la pantalla

    struct NodoProveedor *proveedorActual = NULL;
    mostrarYSeleccionarProveedor(farmacia, &idProveedor, &proveedorActual);
    if (proveedorActual == NULL) {
        return;
    }

    cls();
    struct NodoSucursales *sucursalActual = NULL;
    mostrarYSeleccionarSucursal(farmacia, &idSucursal, &sucursalActual);
    if (sucursalActual == NULL) {
        return;
    }

    cls();
    struct Producto *productoSeleccionado = NULL;
    mostrarYSeleccionarProducto(proveedorActual, codigoProducto, &productoSeleccionado);
    if (productoSeleccionado == NULL) {
        return;
    }

    obtenerDetallesProducto(fechaCaducidad, lote, &cantidad, productoSeleccionado->nombreProducto);

    // Crear una copia completa del producto
    struct Producto *nuevoProducto = (struct Producto *)malloc(sizeof(struct Producto));
    if (nuevoProducto == NULL) {
        printf("Error al asignar memoria para el nuevo producto.\n");
        getch();
        return;
    }
    // Copiar datos del producto original al nuevo producto
    strcpy(nuevoProducto->codigo, productoSeleccionado->codigo);
    nuevoProducto->nombreProducto = strdup(productoSeleccionado->nombreProducto);
    nuevoProducto->descripcion = strdup(productoSeleccionado->descripcion);
    nuevoProducto->categoria = strdup(productoSeleccionado->categoria);
    nuevoProducto->precio = productoSeleccionado->precio;
    nuevoProducto->idProveedor = strdup(productoSeleccionado->idProveedor);
    nuevoProducto->lote = strdup(lote);  // Usar el valor ingresado por el usuario
    nuevoProducto->fechaCaducidad = strdup(fechaCaducidad);  // Usar el valor ingresado por el usuario
    nuevoProducto->cantidad = cantidad;  // Usar el valor ingresado por el usuario
    nuevoProducto->requiereReceta = productoSeleccionado->requiereReceta;

    // Crear un nuevo nodo producto
    struct NodoProducto *nuevoNodoProducto = (struct NodoProducto *)malloc(sizeof(struct NodoProducto));
    if (nuevoNodoProducto == NULL) {
        printf("Error al asignar memoria para el nuevo nodo de producto.\n");
        free(nuevoProducto);
        pause();
        return;
    }
    nuevoNodoProducto->datosProducto = nuevoProducto;
    nuevoNodoProducto->ant = nuevoNodoProducto->sig = NULL;

    // Insertar el nuevo nodo producto en la lista de productos de la sucursal
    if (sucursalActual->datosSucursal->productos == NULL) {
        sucursalActual->datosSucursal->productos = nuevoNodoProducto;
    } else {
        struct NodoProducto *temp = sucursalActual->datosSucursal->productos;
        while (temp->sig != NULL) {
            temp = temp->sig;
        }
        temp->sig = nuevoNodoProducto;
        nuevoNodoProducto->ant = temp;
    }

    cls();
    printTransferenciaExitosa(codigoProducto, idProveedor, idSucursal);
    //printCamion();
    pause();
}
// FUNCION HECHA POR BASTIAN

// Patricio v v v v v v
void extraerFecha(const char *fecha, int *mes, int *anio) {
    sscanf(fecha, "%d/%d", mes, anio);
}

void imprimirSucursalOProductosInvalidos() {
    printf("Sucursal o productos no válidos.\n");
}

void imprimirFechaCaducidadInvalida() {
    printf("Fecha de caducidad inválida.\n");
}

void imprimirNoHaySucursalesRegistradas() {
    printf("No hay sucursales registradas.\n");
}

void imprimirProductosVencidosEliminadosDeTodasLasSucursales() {
    printf("Productos vencidos eliminados de todas las sucursales correctamente.\n");
}

void imprimirSucursalNoEncontrada() {
    cls();
    printf("Sucursal no encontrada.\n");
    pause();
}

void imprimirProductosVencidosEliminadosCorrectamente() {
    printf("Productos vencidos eliminados correctamente.\n");
}

void imprimirIngreseIDSucursal() {
    printf("Ingrese el ID de la sucursal (o -1 para todas las sucursales): ");
}

void imprimirIngreseFechaVencimiento() {
    printf("Ingrese la fecha de vencimiento (MM/AAAA): ");
}

void imprimirFechaCaducidadInvalidaMenu() {
    printf("Fecha de caducidad invalida. Por favor, ingrese una fecha válida.\n");
}


int compararFechasVencidas(const char *fecha1, const char *fecha2) {
    int mes1, anio1, mes2, anio2;

    extraerFecha(fecha1, &mes1, &anio1);
    extraerFecha(fecha2, &mes2, &anio2);

    if (anio1 < anio2) return -1;
    if (anio1 > anio2) return 1;
    if (mes1 < mes2) return -1;
    if (mes1 > mes2) return 1;
    return 0;
}

int validarFecha(const char *fecha) {
    int mes, anio;
    extraerFecha(fecha, &mes, &anio);
    if (mes < 1 || mes > 12) {
        return 0;
    }
    return 1;
}

void eliminarProductosVencidos(struct Sucursal *sucursal, const char *fechaLimite) {
    if (sucursal == NULL || sucursal->productos == NULL) {
        cls();
        imprimirSucursalOProductosInvalidos();
        pause();
        return;
    }
    if (!validarFecha(fechaLimite)) {
        cls();
        imprimirFechaCaducidadInvalida();
        pause();
        return;
    }

    struct NodoProducto *productoActual = sucursal->productos;
    while (productoActual != NULL) {
        struct NodoProducto *siguienteProducto = productoActual->sig;
        if (compararFechasVencidas(productoActual->datosProducto->fechaCaducidad, fechaLimite) <= 0) {
            // Actualizar punteros de la lista doblemente enlazada
            if (productoActual->ant != NULL) {
                productoActual->ant->sig = productoActual->sig;
            } else {
                sucursal->productos = productoActual->sig;
            }

            if (productoActual->sig != NULL) {
                productoActual->sig->ant = productoActual->ant;
            }

            // No liberar memoria del producto, solo desvincular de la lista
            productoActual->ant = NULL;
            productoActual->sig = NULL;
        }
        productoActual = siguienteProducto;
    }
}

void eliminarProductosVencidosDeTodasLasSucursales(struct FarmaSalud *farmacia, const char *fechaLimite) {
    if (farmacia == NULL || farmacia->sucursales == NULL) {
        imprimirNoHaySucursalesRegistradas();
        return;
    }

    if (!validarFecha(fechaLimite)) {
        imprimirFechaCaducidadInvalida();
        return;
    }

    struct NodoSucursales *sucursalActual = farmacia->sucursales;
    do {
        eliminarProductosVencidos(sucursalActual->datosSucursal, fechaLimite);
        sucursalActual = sucursalActual->sig;
    } while (sucursalActual != farmacia->sucursales);
    cls();
    imprimirProductosVencidosEliminadosDeTodasLasSucursales();
    pause();
}


void menuEliminarProductosVencidos(struct FarmaSalud *farmacia) {
    cls();
    int idSucursal;
    char fechaLimite[8];

    mostrarSucursales(farmacia);
    imprimirIngreseIDSucursal();
    scanf("%d", &idSucursal);

    do {
        imprimirIngreseFechaVencimiento();
        scanf("%s", fechaLimite);

        if (!validarFecha(fechaLimite)) {
            imprimirFechaCaducidadInvalidaMenu();
        }
    } while (!validarFecha(fechaLimite));

    if (idSucursal == -1) {
        eliminarProductosVencidosDeTodasLasSucursales(farmacia, fechaLimite);
    } else {
        struct NodoSucursales *sucursalNodo = buscarSucursalPorID(farmacia, idSucursal);
        if (sucursalNodo == NULL) {
            imprimirSucursalNoEncontrada();
            return;
        }
        eliminarProductosVencidos(sucursalNodo->datosSucursal, fechaLimite);
        cls();
        imprimirProductosVencidosEliminadosCorrectamente();
        pause();
    }
}
void printClientesRegistrados() {
    printf("Clientes registrados:\n");
    printf("ID\tNombre\t\tRUT\n");
    printf("-----------------------------------\n");
}

void printClienteInfo(int id, char* rutCliente) {
    printf("%d\t%s\t\n", id, rutCliente);
}

void printNoHayClientesRegistrados() {
    printf("No hay clientes registrados.\n");
}

void printEntradaInvalida() {
    printf("Entrada inválida.\n");
}

void printClienteRegistrado() {
    printf("El cliente está registrado (1-Sí, 0-No): ");
}

void printIngreseIDCliente() {
    printf("Ingrese el ID del cliente: ");
}

void printClienteNoEncontrado() {
    printf("Cliente no encontrado.\n");
}

void printIngreseCantidadProductos() {
    printf("Ingrese la cantidad de productos a comprar: ");
}

void printCantidadProductosInvalida() {
    printf("Cantidad de productos inválida.\n");
}

void printIngreseCodigoProducto(int index) {
    printf("Ingrese el código del producto %d: ", index + 1);
}

void printIngreseCantidadProducto(int index) {
    printf("Ingrese la cantidad del producto %d: ", index + 1);
}

void printProductoNoEncontrado() {
    printf("Producto no encontrado en la sucursal.\n");
}

void printStockInsuficiente(char* nombreProducto) {
    printf("No hay suficiente stock del producto %s.\n", nombreProducto);
}

void printProductoRequiereReceta(char* nombreProducto) {
    printf("El producto %s requiere receta. El cliente tiene la receta (1-Sí, 0-No): ", nombreProducto);
}

void printCompraSinReceta(char* nombreProducto) {
    printf("No se puede completar la compra del producto %s sin receta.\n", nombreProducto);
}

void printBoletaFisicaHeader() {
    printf("\n=======================================\n");
    printf("                 FarmaSalud\n");
    printf("           La farmacia del pueblo\n");
    printf("               Boleta Física\n");
    printf("=======================================\n");
}

void printSucursalNombre(char* nombre) {
    printf("Sucursal: %s\n", nombre);
}

void printClienteNombre(char* rutCliente) {
    printf("Cliente: %s\n", rutCliente);
}

void printCompraAnonima() {
    printf("Cliente: Compra anónima\n");
}

void printProductosHeader() {
    printf("---------------------------------------\n");
    printf("PRODUCTOS                CANTIDAD   PRECIO\n");
    printf("---------------------------------------\n");
}

void printProductoInfo(char* nombreProducto, int cantidad, int precio) {
    printf("%-20s %5d    $%d\n", nombreProducto, cantidad, precio);
}

void printTotal(int total) {
    printf("---------------------------------------\n");
    printf("Total:                           $%d\n", total);
    printf("=======================================\n");
}

struct NodoClientes* buscarClientePorID(struct FarmaSalud *farmacia, int idCliente) {
    struct NodoClientes *clienteActual = farmacia->clientes;
    while (clienteActual != NULL && clienteActual->datosClientes->id != idCliente) {
        clienteActual = clienteActual->sig;
    }
    return clienteActual;
}

struct Producto* crearProductoVendido(struct Producto *producto) {
    struct Producto *nuevoProducto = (struct Producto*)malloc(sizeof(struct Producto));
    strcpy(nuevoProducto->codigo, producto->codigo);
    nuevoProducto->nombreProducto = strdup(producto->nombreProducto);
    nuevoProducto->descripcion = strdup(producto->descripcion);
    nuevoProducto->categoria = strdup(producto->categoria);
    nuevoProducto->precio = producto->precio;
    nuevoProducto->idProveedor = strdup(producto->idProveedor);
    nuevoProducto->lote = strdup("N/A");
    nuevoProducto->fechaCaducidad = strdup("N/A");
    nuevoProducto->cantidad = 1;
    nuevoProducto->requiereReceta = producto->requiereReceta;
    return nuevoProducto;
}

struct Producto* buscarProductoEnSucursal(struct Sucursal *sucursal, char *codigoProducto) {
    struct NodoProducto *productoNodo = sucursal->productos;
    while (productoNodo != NULL) {
        if (strcmp(productoNodo->datosProducto->codigo, codigoProducto) == 0) {
            return productoNodo->datosProducto;
        }
        productoNodo = productoNodo->sig;
    }
    return NULL;
}

void agregarProductoAVendidos(struct Sucursal *sucursal, struct Producto *producto) {
    struct NodoProducto *nuevoNodo = (struct NodoProducto*)malloc(sizeof(struct NodoProducto));
    nuevoNodo->datosProducto = producto;
    nuevoNodo->sig = sucursal->productosVendidos;
    nuevoNodo->ant = NULL;
    if (sucursal->productosVendidos != NULL) {
        sucursal->productosVendidos->ant = nuevoNodo;
    }
    sucursal->productosVendidos = nuevoNodo;
}

struct Producto* buscarProductoEnVendidos(struct NodoProducto *productosVendidos, char *codigoProducto) {
    struct NodoProducto *productoNodo = productosVendidos;
    while (productoNodo != NULL) {
        if (strcmp(productoNodo->datosProducto->codigo, codigoProducto) == 0) {
            return productoNodo->datosProducto;
        }
        productoNodo = productoNodo->sig;
    }
    return NULL;
}

void mostrarClientes(struct FarmaSalud *farmacia) {
    struct NodoClientes *actual = farmacia->clientes;

    if (actual == NULL) {
        printNoHayClientesRegistrados();
        return;
    }

    printClientesRegistrados();

    while (actual != NULL) {
        printClienteInfo(actual->datosClientes->id, actual->datosClientes->rutCliente);
        actual = actual->sig;
    }
}

void realizarCompra(struct FarmaSalud *farmacia) {
    int espacio;
    int idSucursal, idCliente, cantidadCompras, cantidadProducto, i, j;
    char codigoProducto[10];
    struct NodoSucursales *sucursal = NULL;
    struct NodoClientes *cliente = NULL;
    int esClienteRegistrado = 0;
    int tieneReceta = 0;

    cls();
    mostrarSucursales(farmacia);

    printIngreseIDSucursal();
    if (scanf("%d", &idSucursal) != 1) {
        printEntradaInvalida();
        pause();
        return;
    }
    cls();
    sucursal = buscarSucursalPorID(farmacia, idSucursal);
    if (sucursal == NULL) {
        printSucursalNoEncontrada();
        pause();
        return;
    }

    printClienteRegistrado();
    if (scanf("%d", &esClienteRegistrado) != 1) {
        printEntradaInvalida();
        pause();
        return;
    }

    if (esClienteRegistrado) {
        cls();
        mostrarClientes(farmacia);
        printIngreseIDCliente();
        if (scanf("%d", &idCliente) != 1) {
            printEntradaInvalida();
            pause();
            return;
        }
        cls();
        cliente = buscarClientePorID(farmacia, idCliente);
        if (cliente == NULL) {
            printClienteNoEncontrado();
            pause();
            return;
        }
    }

    cls();
    printIngreseCantidadProductos();
    espacio = 10000 - espacioRestante(sucursal -> datosSucursal);
    if (scanf("%d", &cantidadCompras) != 1 || cantidadCompras <= 0 || cantidadCompras > espacio) {
        printCantidadProductosInvalida();
        pause();
        return;
    }
    cls();

    struct Producto *productosComprados[cantidadCompras];
    int cantidades[cantidadCompras];
    int productosRequierenReceta[cantidadCompras];

    for (i = 0; i < cantidadCompras; i++) {
        printIngreseCodigoProducto(i);
        if (scanf("%s", codigoProducto) != 1) {
            printEntradaInvalida();
            pause();
            return;
        }

        printIngreseCantidadProducto(i);
        if (scanf("%d", &cantidadProducto) != 1 || cantidadProducto <= 0) {
            printCantidadProductosInvalida();
            pause();
            return;
        }
        cls();

        struct Producto *producto = buscarProductoEnSucursal(sucursal->datosSucursal, codigoProducto);
        if (producto == NULL) {
            printProductoNoEncontrado();
            pause();
            return;
        }

        if (producto->cantidad < cantidadProducto) {
            printStockInsuficiente(producto->nombreProducto);
            pause();
            return;
        }

        if (producto->requiereReceta) {
            printProductoRequiereReceta(producto->nombreProducto);
            if (scanf("%d", &tieneReceta) != 1 || (tieneReceta != 0 && tieneReceta != 1)) {
                printEntradaInvalida();
                pause();
                return;
            }

            if (!tieneReceta) {
                printCompraSinReceta(producto->nombreProducto);
                productosRequierenReceta[i] = 0;
            } else {
                productosRequierenReceta[i] = 1;
            }
        } else {
            productosRequierenReceta[i] = 1;
        }

        productosComprados[i] = producto;
        cantidades[i] = cantidadProducto;
    }

    for (i = 0; i < cantidadCompras; i++) {
        if (productosRequierenReceta[i]) {
            productosComprados[i]->cantidad -= cantidades[i];

            struct Producto *productoVendido = buscarProductoEnVendidos(sucursal->datosSucursal->productosVendidos, productosComprados[i]->codigo);
            if (productoVendido == NULL) {
                productoVendido = crearProductoVendido(productosComprados[i]);
                agregarProductoAVendidos(sucursal->datosSucursal, productoVendido);
            } else {
                productoVendido->cantidad += cantidades[i];
            }

            if (esClienteRegistrado) {
                for (j = 0; j < cantidades[i]; j++) {
                    cliente->datosClientes->comprasCliente[cliente->datosClientes->numCompras] = productosComprados[i];
                    cliente->datosClientes->numCompras++;
                }
            }
        }
    }

    sucursal->datosSucursal->cantidadDeVentas++;

    printBoletaFisicaHeader();
    printSucursalNombre(sucursal->datosSucursal->nombre);
    if (esClienteRegistrado) {
        printClienteNombre(cliente->datosClientes->rutCliente);
    } else {
        printCompraAnonima();
    }
    printProductosHeader();
    for (i = 0; i < cantidadCompras; i++) {
        if (productosRequierenReceta[i]) {
            printProductoInfo(productosComprados[i]->nombreProducto, cantidades[i], productosComprados[i]->precio * cantidades[i]);
        }
    }
    int total = 0;
    for (i = 0; i < cantidadCompras; i++) {
        if (productosRequierenReceta[i]) {
            total += productosComprados[i]->precio * cantidades[i];
        }
    }
    printTotal(total);

    pause();
}

// Giovanni v v v v v v
// Controlador
void eliminarProducto(struct NodoProducto **productoActual, struct NodoSucursales *sucursalActual) {
    struct NodoProducto *siguienteProducto = (*productoActual)->sig;
    if ((*productoActual)->ant != NULL) {
        (*productoActual)->ant->sig = (*productoActual)->sig;
    } else {
        // Si es el primer producto, actualizamos el puntero inicial de productos
        sucursalActual->datosSucursal->productos = (*productoActual)->sig;
    }

    if ((*productoActual)->sig != NULL) {
        (*productoActual)->sig->ant = (*productoActual)->ant;
    }

    // Liberar memoria del producto
    free((*productoActual)->datosProducto->nombreProducto);
    free((*productoActual)->datosProducto->descripcion);
    free((*productoActual)->datosProducto->categoria);
    free((*productoActual)->datosProducto->idProveedor);
    free((*productoActual)->datosProducto->lote);
    free((*productoActual)->datosProducto->fechaCaducidad);
    free((*productoActual)->datosProducto);
    free(*productoActual);

    *productoActual = siguienteProducto;
}

// Vista
void imprimirActualizacionInventario() {
    printf("Inventarios actualizados. Productos con stock 0 eliminados.\n");
}

// Función principal
void actualizarInventariosSucursales(struct FarmaSalud *farmacia) {
    struct NodoSucursales *sucursalActual;

    if (farmacia == NULL || farmacia->sucursales == NULL) {
        printf("No hay sucursales registradas.\n");
        return;
    }

    sucursalActual = farmacia->sucursales;

    // Iterar sobre cada sucursal
    do {
        struct NodoProducto *productoActual;

        if (sucursalActual->datosSucursal == NULL || sucursalActual->datosSucursal->productos == NULL) {
            sucursalActual = sucursalActual->sig;
            continue;
        }

        productoActual = sucursalActual->datosSucursal->productos; // Iniciar en el primer nodo (sin nodo fantasma)
        while (productoActual != NULL) {
            if (productoActual->datosProducto != NULL && productoActual->datosProducto->cantidad == 0) {
                // Eliminar producto con cantidad 0
                eliminarProducto(&productoActual, sucursalActual);
            } else {
                productoActual = productoActual->sig;
            }
        }
        sucursalActual = sucursalActual->sig;
    } while (sucursalActual != farmacia->sucursales);

    cls();
    imprimirActualizacionInventario();
    pause();
}
int contarVentasProducto(struct NodoProducto *productosVendidos, char *codigoProducto) {
    int totalVentas = 0;
    struct NodoProducto *productoActual;

    productoActual = productosVendidos->sig;

    while (productoActual != productosVendidos) {
        if (strcmp(productoActual->datosProducto->codigo, codigoProducto) == 0) {
            totalVentas += productoActual->datosProducto->cantidad;
        }
        productoActual = productoActual->sig;
    }
    return totalVentas;
}

struct Producto *productoMasVendido(struct FarmaSalud *farma) {
    struct Producto *productoMasVendido = NULL;
    struct NodoSucursales *sucursalActual;
    struct NodoProducto *productoActual;
    struct Producto *producto;
    int maxVentas = 0;
    int ventasProducto;

    if (!farma || !farma->sucursales) return NULL;

    sucursalActual = farma->sucursales->sig;
    while (sucursalActual != farma->sucursales) {
        if (sucursalActual && sucursalActual->datosSucursal && sucursalActual->datosSucursal->productosVendidos) {
            productoActual = sucursalActual->datosSucursal->productosVendidos->sig;
            while (productoActual != sucursalActual->datosSucursal->productosVendidos) {
                if (productoActual && productoActual->datosProducto) {
                    producto = productoActual->datosProducto;
                    ventasProducto = contarVentasProducto(sucursalActual->datosSucursal->productosVendidos,producto->codigo);
                    if (ventasProducto > maxVentas) {
                        maxVentas = ventasProducto;
                        productoMasVendido = producto;
                    }
                }
                productoActual = productoActual->sig;
            }
        }
        sucursalActual = sucursalActual->sig;
    }
    return productoMasVendido;
}

struct Sucursal* sucursalConMasVentas(struct FarmaSalud *farmaSalud) {
    struct NodoSucursales *nodoActual;
    struct Sucursal *sucursalConMasVentas = NULL;
    int maxVentas = -1;

    if (!farmaSalud || !farmaSalud->sucursales) return NULL;

    nodoActual = farmaSalud->sucursales->sig;

    while (nodoActual != farmaSalud->sucursales) {
        if (nodoActual->datosSucursal->cantidadDeVentas > maxVentas) {
            maxVentas = nodoActual->datosSucursal->cantidadDeVentas;
            sucursalConMasVentas = nodoActual->datosSucursal;
        }
        nodoActual = nodoActual->sig;
    }

    return sucursalConMasVentas;
}

struct Clientes* clienteConMasCompras(struct FarmaSalud *farmaSalud) {
    struct NodoClientes *nodoActual;
    struct Clientes *clienteConMasCompras = NULL;
    int maxCompras = -1;

    if (!farmaSalud || !farmaSalud->clientes) return NULL;

    nodoActual = farmaSalud->clientes->sig;

    while (nodoActual && nodoActual != farmaSalud->clientes) {
        if (nodoActual->datosClientes && nodoActual->datosClientes->numCompras > maxCompras) {
            maxCompras = nodoActual->datosClientes->numCompras;
            clienteConMasCompras = nodoActual->datosClientes;
        }
        nodoActual = nodoActual->sig;
    }

    return clienteConMasCompras;
}


int contarProductos(struct NodoArbolProducto *raiz) {
    int contador = 0;
    struct NodoArbolProducto *pila[1000];
    int top = 0;
    struct NodoArbolProducto *nodoActual;

    if (!raiz) return 0;

    pila[top++] = raiz;
    while (top > 0) {
        nodoActual = pila[--top];
        contador++;
        if (nodoActual->izq) pila[top++] = nodoActual->izq;
        if (nodoActual->der) pila[top++] = nodoActual->der;
    }
    return contador;
}

struct Proveedor* proveedorConMasProductos(struct FarmaSalud *farmaSalud) {
    struct NodoProveedor *nodoActual;
    struct Proveedor *proveedorConMasProductos = NULL;
    int maxProductos = -2;
    int numProductos;

    if (!farmaSalud || !farmaSalud->proveedores) return NULL;

    nodoActual = farmaSalud->proveedores->sig;
    while (nodoActual && nodoActual != farmaSalud->proveedores) {
        if (nodoActual->datosProveedor && nodoActual->datosProveedor->productos) {
            numProductos = contarProductos(nodoActual->datosProveedor->productos);
            if (numProductos > maxProductos) {
                maxProductos = numProductos;
                proveedorConMasProductos = nodoActual->datosProveedor;
            }
        }
        nodoActual = nodoActual->sig;
    }
    return proveedorConMasProductos;
}

void mostrarInforme(struct FarmaSalud *farmacia) {
    struct Sucursal *sucursalMasVentas;
    struct Producto *productoMasVendidoResult;
    struct Proveedor *proveedorMasProductos;
    struct Clientes *clienteMasCompras;

    if (!farmacia) {
        printf("La farmacia no está inicializada.\n");
        pause();
        return;
    }
    cls();

    sucursalMasVentas = sucursalConMasVentas(farmacia);
    productoMasVendidoResult = productoMasVendido(farmacia);
    proveedorMasProductos = proveedorConMasProductos(farmacia);
    clienteMasCompras = clienteConMasCompras(farmacia);

    printf("     Informe De FarmaSalud\n");
    printf("===============================\n");
    if (sucursalMasVentas)
        printf("-Sucursal con más ventas: %s\n\n", sucursalMasVentas->nombre);
    else
        printf("-Sucursal con más ventas: N/A\n\n");

    if (productoMasVendidoResult)
        printf("-Producto más vendido: %s\n\n", productoMasVendidoResult->nombreProducto);
    else
        printf("-Producto más vendido: N/A\n\n");

    if (proveedorMasProductos)
        printf("-Proveedor con más productos: %s\n\n", proveedorMasProductos->nombre);
    else
        printf("-Proveedor con más productos: N/A\n\n");

    if (clienteMasCompras)
        printf("-Cliente con más compras: %s\n\n", clienteMasCompras->rutCliente);
    else
        printf("-Cliente con más compras: N/A\n\n");

    pause();
}

void menuPrincipal(struct FarmaSalud *farmacia) {
    int opcion;
    char input[50];
    do {
        cls();
        printf("\n--- FarmaSalud ---\n");
        printf("1. leerClientes\n");
        printf("2. crearCliente\n");
        printf("3. eliminarCliente\n");
        printf("4. actualizarSucursales\n");
        printf("5. leerSucursales\n");
        printf("6. crearSucursal\n");
        printf("7. eliminarSucursal\n");
        printf("8. crearProveedor\n");
        printf("9. leerProveedor\n");
        printf("10. eliminarProveedor\n");
        printf("11. agregarProductoProveedor\n");
        printf("12. transferirProductosProveedorASucursal\n");
        printf("13. agregarProductoASucursal\n");
        printf("14. eliminarProductosVencidos\n");
        printf("15. realizarCompra\n");
        printf("16. Ver Informes\n");
        printf("\n17. Salir\n");
        printf("\nSeleccione una opcion: ");
        gets(input);
        if (sscanf(input, "%d", &opcion) != 1 && opcion > 17 || opcion < 1) {
            cls();
            printf("Entrada no valida. Por favor, ingrese un número del 1 al 17.\n");
            pause();
            continue;
        }

        switch (opcion) {
            case 1:
                leerClientes(farmacia);
                break;
            case 2:
                agregarCliente(farmacia);
                break;
            case 3:
                eliminarCliente(farmacia);
                break;
            case 4:
                actualizarInventariosSucursales(farmacia);
                break;
            case 5:
                leerSucursales(farmacia);
                break;
            case 6:
                crearSucursal(farmacia);
                break;
            case 7:
                eliminarSucursalView(farmacia);
                break;
            case 8:
                crearProveedorView(farmacia);
                break;
            case 9:
                leerProveedores(farmacia);
                break;
            case 10:
                eliminarProveedor(farmacia);
                break;
            case 11:
                agregarProductoProveedor(farmacia);
                break;
            case 12:
                transferirProductosProveedorASucursal(farmacia);
                break;
            case 13:
                transferirProductoProveedorASucursal(farmacia);
                break;
            case 14:
                menuEliminarProductosVencidos(farmacia);
                break;
            case 15:
                realizarCompra(farmacia);
                actualizarInventariosSucursales(farmacia);
                break;
            case 16:
                mostrarInforme(farmacia);
                break;
            case 17: // Actualizar opción de salida
                printf("Saliendo...\n");
                exit(0);
                break;
            default:
                printf("Opcion no valida. Intente nuevamente.\n");
                break;
        }
    } while (opcion != 17);
}

int main() {
    struct FarmaSalud *farmacia;
    farmacia = (struct FarmaSalud *)malloc(sizeof(struct FarmaSalud));

    farmacia->clientes = NULL;
    farmacia->sucursales = NULL;
    farmacia->proveedores = NULL;

    // Inicialización de datos de prueba
    struct Producto prod1 = {"1", "Ibuprofeno", "Recomendado para el dolor de garganta", "Anti-inflamatorio", 100, "Bioequivalente", "Lote A", "10/2020", 8, 0};
    struct Producto prod2 = {"2", "Condones", "Ultra resistente y ultra delgado", "Anticonceptivo", 200, "Condoneria Nacional", "Lote B", "06/2019", 10, 1};
    struct Producto prod3 = {"3", "Paracetamol", "Recomendado para el dolor de cabeza", "Analgesico", 300, "Lab. de Chile", "Lote C", "12/2022", 9, 0}; // Producto vencido
    struct Producto prod4 = {"4", "Tapsin", "Perfecto Para los resfriados", "Analgésico", 400, "Tapsin Chile", "Lote D", "02/2017", 40, 1};
    struct Producto prod5 = {"5", "Amoxicilina", "Antibiótico de amplio espectro", "Antibiótico", 500, "Lab. de Chile", "Lote E", "11/2025", 50, 1};
    struct Producto prod6 = {"6", "Loratadina", "Para aliviar la alergia", "Antihistamínico", 600, "PharmaCorp", "Lote F", "10/2024", 60, 0};
    struct Producto prod7 = {"7", "Diclofenaco", "Alivia el dolor e inflamación", "Analgesico", 700, "Lab. de Chile", "Lote G", "09/2026", 70, 0};
    struct Producto prod8 = {"8", "Aspirina", "Reduce el dolor y la fiebre", "Analgesico", 800, "Bioequivalente", "Lote H", "08/2029", 80, 0};
    struct Producto prod9 = {"9", "Omeprazol", "Para el tratamiento de la acidez", "Antiácido", 900, "PharmaCorp", "Lote I", "07/2016", 90, 1};
    struct Producto prod10 = {"10", "Vitamina C", "Suplemento vitamínico", "Suplemento", 1000, "Lab. Maver", "Lote J", "06/2026", 100, 0};

    // Creación de compras de prueba
    struct Producto* compras1[] = {&prod1, &prod2};
    struct Producto* compras2[] = {&prod3, &prod4};
    struct Producto* compras3[] = {&prod5, &prod6};


    struct Producto* compras4[] = {&prod7, &prod8};
    struct Producto* compras5[] = {&prod9, &prod10};
    struct Producto* compras6[] = {&prod1, &prod3};
    struct Producto* compras7[] = {&prod2, &prod4};
    struct Producto* compras8[] = {&prod5, &prod7};
    struct Producto* compras9[] = {&prod6, &prod8};
    struct Producto* compras10[] = {&prod9, &prod1};

    // Creación de clientes
    struct NodoClientes* cliente1 = crearClienteConsole(1, "8032837-5", 25, 1, compras1, 2);
    agregarClienteConsole(farmacia, cliente1);
    struct NodoClientes* cliente2 = crearClienteConsole(2, "22222222-2", 25, 0, compras2, 2);
    agregarClienteConsole(farmacia, cliente2);
    struct NodoClientes* cliente3 = crearClienteConsole(3, "33333333-3", 30, 1, compras3, 2);
    agregarClienteConsole(farmacia, cliente3);
    struct NodoClientes* cliente4 = crearClienteConsole(4, "44444444-4", 35, 0, compras4, 2);
    agregarClienteConsole(farmacia, cliente4);
    struct NodoClientes* cliente5 = crearClienteConsole(5, "55555555-5", 40, 1, compras5, 2);
    agregarClienteConsole(farmacia, cliente5);
    struct NodoClientes* cliente6 = crearClienteConsole(6, "66666666-6", 45, 0, compras6, 2);
    agregarClienteConsole(farmacia, cliente6);
    struct NodoClientes* cliente7 = crearClienteConsole(7, "77777777-7", 50, 1, compras7, 2);
    agregarClienteConsole(farmacia, cliente7);
    struct NodoClientes* cliente8 = crearClienteConsole(8, "88888888-8", 55, 0, compras8, 2);
    agregarClienteConsole(farmacia, cliente8);
    struct NodoClientes* cliente9 = crearClienteConsole(9, "99999999-9", 60, 1, compras9, 2);
    agregarClienteConsole(farmacia, cliente9);
    struct NodoClientes* cliente10 = crearClienteConsole(10, "10101010-1", 65, 0, compras10, 2);
    agregarClienteConsole(farmacia, cliente10);

    // Creación de sucursales
    struct NodoSucursales* sucursal1 = crearSucursalConsole(1, "Kennedy", "Apoquindo 234, Santiago", 100);
    agregarSucursalConsole(farmacia, sucursal1);
    struct NodoSucursales* sucursal2 = crearSucursalConsole(2, "Los Heroes", "Metro Pedro de Valdivia, Santiago", 200);
    agregarSucursalConsole(farmacia, sucursal2);
    struct NodoSucursales* sucursal3 = crearSucursalConsole(3, "La Florida", "Vicuña Mackenna 4500, Santiago", 150);
    agregarSucursalConsole(farmacia, sucursal3);
    struct NodoSucursales* sucursal4 = crearSucursalConsole(4, "Providencia", "Providencia 1234, Santiago", 180);
    agregarSucursalConsole(farmacia, sucursal4);

    // Creación de proveedores
    struct NodoProveedor* proveedor1 = crearProveedorConsole(1, "Lab. de Chile", "Av. Marathon 1315", "90322000-7");
    agregarProveedorConsole(farmacia, proveedor1);
    struct NodoProveedor* proveedor2 = crearProveedorConsole(2, "Lab. Maver", "Las Encinas 1777", "98765432-1");
    agregarProveedorConsole(farmacia, proveedor2);
    struct NodoProveedor* proveedor3 = crearProveedorConsole(3, "PharmaCorp", "Av. Providencia 123", "90321000-8");
    agregarProveedorConsole(farmacia, proveedor3);
    struct NodoProveedor* proveedor4 = crearProveedorConsole(4, "Bioequivalente", "Av. Apoquindo 456", "90323000-9");
    agregarProveedorConsole(farmacia, proveedor4);

    // Creación de productos de prueba
    struct Producto* producto1 = crearProductoFalso("1", "Sertralina 100mg 30 Comp", "Antidepresivo en forma de Cápsulas", "Antidepresivo", 6327, "Lab. de Chile", "N/A", "N/A", -1, 1);
    struct Producto* producto2 = crearProductoFalso("2", "Clonazepam 2 Mg 30 Comp", "Actúa sobre el sistema nervioso central, con propiedades ansiolíticas.", "Ansiolítico", 5007, "Lab. de Chile", "N/A", "N/A", -1, 1);
    struct Producto* producto3 = crearProductoFalso("3", "Tapsin Día-Noche Plus 18 Comp", "Comprimidos Recubiertos rápido alivio de los síntomas de la gripe", "Antigripal", 2575, "Lab. Maver", "N/A", "N/A", -1, 0);
    struct Producto* producto4 = crearProductoFalso("4", "Tapsín Día Limón 6 Un.", "Para el alivio sintomático de las molestias del resfrío y la gripe.", "Antigripal", 2500, "Lab. Maver", "N/A", "N/A", -1, 0);
    struct Producto* producto5 = crearProductoFalso("5", "Amoxicilina 500mg", "Antibiótico de amplio espectro", "Antibiótico", 5800, "Lab. de Chile", "N/A", "N/A", -1, 1);
    struct Producto* producto6 = crearProductoFalso("6", "Loratadina 10mg", "Para aliviar la alergia", "Antihistamínico", 1200, "PharmaCorp", "N/A", "N/A", -1, 0);
    struct Producto* producto7 = crearProductoFalso("7", "Omeprazol 20mg", "Para el tratamiento de la acidez", "Antiácido", 2700, "PharmaCorp", "N/A", "N/A", -1, 1);
    struct Producto* producto8 = crearProductoFalso("8", "Aspirina 500mg", "Reduce el dolor y la fiebre", "Analgesico", 2500, "Bioequivalente", "N/A", "N/A", -1, 0);
    struct Producto* producto9 = crearProductoFalso("9", "Vitamina C 1000mg", "Suplemento vitamínico", "Suplemento", 4500, "Lab. Maver", "N/A", "N/A", -1, 0);
    struct Producto* producto10 = crearProductoFalso("10", "Paracetamol 500mg", "Analgésico y antipirético", "Analgésico", 3500, "Lab. de Chile", "N/A", "N/A", -1, 0);

    // Agregar productos a proveedores
    agregarProductoAProveedor(proveedor1->datosProveedor, producto1);
    agregarProductoAProveedor(proveedor1->datosProveedor, producto2);
    agregarProductoAProveedor(proveedor1->datosProveedor, producto5);
    agregarProductoAProveedor(proveedor1->datosProveedor, producto10);

    agregarProductoAProveedor(proveedor2->datosProveedor, producto3);
    agregarProductoAProveedor(proveedor2->datosProveedor, producto4);
    agregarProductoAProveedor(proveedor2->datosProveedor, producto9);

    agregarProductoAProveedor(proveedor3->datosProveedor, producto6);
    agregarProductoAProveedor(proveedor3->datosProveedor, producto7);

    agregarProductoAProveedor(proveedor4->datosProveedor, producto8);

// Agregar productos de prueba a las sucursales
    agregarProductoASucursal(sucursal1->datosSucursal, &prod1);
    agregarProductoASucursal(sucursal1->datosSucursal, &prod2);
    agregarProductoASucursal(sucursal1->datosSucursal, &prod3);

    agregarProductoASucursal(sucursal2->datosSucursal, &prod4);
    agregarProductoASucursal(sucursal2->datosSucursal, &prod5);
    agregarProductoASucursal(sucursal2->datosSucursal, &prod6);

    agregarProductoASucursal(sucursal3->datosSucursal, &prod7);
    agregarProductoASucursal(sucursal3->datosSucursal, &prod8);
    agregarProductoASucursal(sucursal3->datosSucursal, &prod9);

    agregarProductoASucursal(sucursal4->datosSucursal, &prod10);
    agregarProductoASucursal(sucursal4->datosSucursal, &prod1);
    agregarProductoASucursal(sucursal4->datosSucursal, &prod2);

    while (1) {
        cls();
        menuPrincipal(farmacia);
    }
    return 0;
}