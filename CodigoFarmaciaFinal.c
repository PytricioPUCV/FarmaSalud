/*
 * Pontificia Universidad Catolica de Valparaiso
 *
 * https://github.com/gioahumada/farmaSalud
 *
 * Este archivo es parte del proyecto de la asignatura INF2223-1 - Estructuras de Datos
 * y a sido diseñado por estudiantes de la PUCV para el desarrollo de un sistema de
 * gestion de inventario y ventas para una farmacia.
 *
 * Autores:
 * - Giovanni Ahumada
 * - Bastian Mejias
 * - Simon Vera
 * - Benjamin Pizarro
 * - Patricio Hernandez
 *
 * Fecha de creacion: 1 de junio de 2024
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
    int numCompras; // Numero de compras
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
    char *registrosEnvios[MAX_ENVIOS]; // Array de strings para los registros de envios
    int numRegistros; // Numero de registros
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
    int mesVenta;
    int anioVenta;
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

    // Implementacion manual de strdup
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
    struct NodoClientes *temp;
    if (farmacia->clientes == NULL) {
        farmacia->clientes = nuevoCliente;
    } else {
        temp = farmacia->clientes;
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
    struct NodoSucursales *temp;
    if (farmacia->sucursales == NULL) {
        farmacia->sucursales = nuevaSucursal;
    } else {
        temp = farmacia->sucursales;
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
    struct NodoProveedor *temp;
    if (farmacia->proveedores == NULL) {
        farmacia->proveedores = nuevoProveedor;
    } else {
        temp = farmacia->proveedores;
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
    nuevoProducto->nombreProducto = my_strdup(nombre);
    nuevoProducto->descripcion = my_strdup(descripcion);
    nuevoProducto->categoria = my_strdup(categoria);
    nuevoProducto->precio = precio;
    nuevoProducto->idProveedor = my_strdup(idProveedor);
    nuevoProducto->lote = my_strdup(lote);
    nuevoProducto->fechaCaducidad = my_strdup(fechaCaducidad);
    nuevoProducto->cantidad = cantidad;
    nuevoProducto->requiereReceta = requiereReceta;

    return nuevoProducto;
}

void agregarProductoAProveedor(struct Proveedor* proveedor, struct Producto* producto) {
    struct NodoArbolProducto* actual;
    struct NodoArbolProducto* padre;
    struct NodoArbolProducto* nuevoNodo = (struct NodoArbolProducto*)malloc(sizeof(struct NodoArbolProducto));
    if (nuevoNodo == NULL) {
        exit(1);
    }
    nuevoNodo->datosProducto = producto;
    nuevoNodo->izq = nuevoNodo->der = NULL;

    if (proveedor->productos == NULL) {
        proveedor->productos = nuevoNodo;
    } else {
        actual = proveedor->productos;
        padre = NULL;

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
    struct NodoProducto* nuevoNodo;
    if (sucursal == NULL || producto == NULL) {
        return;
    }

    // Crear nodo producto
    nuevoNodo = crearNodoProducto(producto);

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

/* QUITAR */

void pause() {
    char dummy;
    printf("\nPresione Enter para continuar...\n");
    fflush(stdin); // Limpiar el bufer de entrada
    scanf("%c", &dummy); // Esperar a que se presione Enter
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

void agregarCliente(struct FarmaSalud *farmacia) {
    int i, maxId, afiliadoValido;
    char rutCliente[20];
    char edadClienteStr[4];
    struct NodoClientes *temp;
    struct NodoClientes *nuevoNodo = (struct NodoClientes *)malloc(sizeof(struct NodoClientes));
    nuevoNodo->datosClientes = (struct Clientes *)malloc(sizeof(struct Clientes));

    cls();

    maxId = 0;
    temp = farmacia->clientes;
    while (temp != NULL) {
        if (temp->datosClientes->id > maxId) {
            maxId = temp->datosClientes->id;
        }
        temp = temp->sig;
    }
    nuevoNodo->datosClientes->id = maxId + 1;

    while (1) {
        printf("Ingrese RUT del cliente (Sin puntos ni gion, pero debe incluir digito verificador): ");
        scanf("%s", rutCliente);
        if (strlen(rutCliente) == 9 && strspn(rutCliente, "0123456789") == 9) {
            nuevoNodo->datosClientes->rutCliente = (char *)malloc((strlen(rutCliente) + 1) * sizeof(char));
            strcpy(nuevoNodo->datosClientes->rutCliente, rutCliente);
            break;
        } else {
            printf("RUT no valido. Intente nuevamente.\n");
            pause();
            cls();
        }
    }

    while (1) {
        printf("Ingrese edad del cliente (solo numeros, maximo 3 digitos): ");
        scanf("%s", edadClienteStr);
        if (strlen(edadClienteStr) <= 3 && strspn(edadClienteStr, "0123456789") == strlen(edadClienteStr)) {
            nuevoNodo->datosClientes->edadCliente = atoi(edadClienteStr);
            break;
        } else {
            printf("Edad no valida. Intente nuevamente.\n");
            pause();
            cls();
        }
    }

    afiliadoValido = 0;
    while (!afiliadoValido) {
        printf("¿Es afiliado? (1-Si, 0-No): ");
        scanf("%d", &nuevoNodo->datosClientes->afiliado);
        if (nuevoNodo->datosClientes->afiliado == 1 || nuevoNodo->datosClientes->afiliado == 0) {
            afiliadoValido = 1;
        } else {
            printf("Opcion no valida. Intente nuevamente.\n");
            pause();
            cls();
        }
    }

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
    printf("Cliente agregado con exito.\n");
    pause();
}

// Funcion para eliminar un cliente del sistema
void eliminarCliente(struct FarmaSalud *farmacia) {
    int i;
    char idClienteStr[10];
    int idCliente;
    struct NodoClientes *clienteActual, *clienteAnterior;
    int idValido;

    cls();

    // Solicitar y validar el ID del cliente
    printf("Ingrese el ID del cliente que desea eliminar: ");
    scanf("%s", idClienteStr);

    // Validar que el ID sea un numero
    idValido = 1;
    for (i = 0; idClienteStr[i] != '\0'; i++) {
        if (idClienteStr[i] < '0' || idClienteStr[i] > '9') {
            idValido = 0;
            break;
        }
    }

    if (!idValido) {
        printf("ID no valido. Intente nuevamente.\n");
        pause();
        cls();
        return; // Retornar al menu principal
    }

    idCliente = atoi(idClienteStr);
    clienteActual = farmacia->clientes;
    clienteAnterior = NULL;

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

    clienteActual->datosClientes->id = 0;

    cls();
    printf("Cliente eliminado con exito.\n");
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
    struct NodoProducto *productoNodo;
    int i;
    printf("ID: %d\n", nodoActual->datosSucursal->id);
    printf("Nombre: %s\n", nodoActual->datosSucursal->nombre);
    printf("Direccion: %s\n", nodoActual->datosSucursal->direccion);
    printf("Cantidad de Ventas: %d\n", nodoActual->datosSucursal->cantidadDeVentas);
    printf("Numero de Registros de Envios: %d\n", nodoActual->datosSucursal->numRegistros);
    for (i = 0; i < nodoActual->datosSucursal->numRegistros; i++) {
        printf("\tRegistro de Envio %d: %s\n", i + 1, nodoActual->datosSucursal->registrosEnvios[i]);
    }

    if (nodoActual->datosSucursal->productos != NULL) {
        printf("Productos:\n");
        productoNodo = nodoActual->datosSucursal->productos;
        while (productoNodo != NULL) {
            printf("\tCodigo: %s\n", productoNodo->datosProducto->codigo);
            printf("\tNombre: %s\n", productoNodo->datosProducto->nombreProducto);
            printf("\tCantidad en stock: %d\n", productoNodo->datosProducto->cantidad);
            printf("\tFecha de caducidad: %s\n", productoNodo->datosProducto->fechaCaducidad);
            printf("\tLote: %s\n", productoNodo->datosProducto->lote);

            // Comprobar si el producto tiene bajo stock
            if (productoNodo->datosProducto->cantidad < 10) {
                printf("\t*** ALERTA: Bajo stock! ***\n\n");
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
    int espacio;
    struct NodoSucursales *nodoActual;
    nodoActual = obtenerSucursales(farmacia);

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

int existeSucursal(struct FarmaSalud *farmacia, int id) {
    struct NodoSucursales *actual;
    actual = farmacia->sucursales;
    if (actual != NULL) {
        do {
            if (actual->datosSucursal->id == id) {
                return 1; // ID encontrado
            }
            actual = actual->sig;
        } while (actual != farmacia->sucursales);
    }
    return 0; // ID no encontrado
}

void agregarSucursal(struct FarmaSalud *farmacia, struct NodoSucursales *nuevaSucursal) {
    struct NodoSucursales *ultimo;
    if (existeSucursal(farmacia, nuevaSucursal->datosSucursal->id)) {
        printf("Error: Ya existe una sucursal con el ID %d.\n", nuevaSucursal->datosSucursal->id);
        return;
    }

    if (farmacia->sucursales == NULL) {
        farmacia->sucursales = nuevaSucursal;
        nuevaSucursal->ant = nuevaSucursal->sig = nuevaSucursal;
    } else {
        ultimo = farmacia->sucursales->ant;
        ultimo->sig = nuevaSucursal;
        nuevaSucursal->ant = ultimo;
        nuevaSucursal->sig = farmacia->sucursales;
        farmacia->sucursales->ant = nuevaSucursal;
    }
}

void leerDatosSucursal(struct Sucursal *nuevaSucursal) {
    int i;
    cls();
    printf("Ingrese ID de la sucursal: ");
    scanf("%d", &nuevaSucursal->id);

    nuevaSucursal->nombre = (char *)malloc(50 * sizeof(char));
    printf("Ingrese nombre de la sucursal: ");
    scanf(" %[^\n]", nuevaSucursal->nombre);

    nuevaSucursal->direccion = (char *)malloc(100 * sizeof(char));
    printf("Ingrese direccion de la sucursal: ");
    scanf(" %[^\n]", nuevaSucursal->direccion);

    nuevaSucursal->cantidadDeVentas = 0; // Inicializar a 0
    nuevaSucursal->capacidadAlmacenamiento = 0; // Cambiar si se necesita un valor especifico
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
    printf("Sucursal agregada con exito.\n");
    pause();
}

int eliminarSucursal(struct FarmaSalud *farmacia, int idEliminar) {
    struct NodoSucursales *temp, *prev, *inicio;

    temp = farmacia->sucursales;
    if (temp == NULL) {
        return 0; // No hay sucursales
    }

    inicio = temp;

    do {
        if (temp->datosSucursal->id == idEliminar) {
            // Manejar los casos especiales para el primer y ultimo nodo
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

            return 1; // Eliminacion exitosa
        }
        temp = temp->sig;
    } while (temp != inicio);

    return 0; // No se encontro la sucursal
}

void leerIdSucursal(int *idEliminar) {
    cls();
    printf("Ingrese el ID de la sucursal que desea eliminar: ");
    scanf("%d", idEliminar);
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
    printf("Sucursal eliminada con exito.\n");
    pause();
}

// Funcion para eliminar una sucursal desde la vista
void eliminarSucursalView(struct FarmaSalud *farmacia) {
    struct NodoSucursales *temp;
    int i;
    char idEliminarStr[10];
    int idEliminar, resultado;
    int idValido;

    cls();
    printf("Ingrese el ID de la sucursal que desea eliminar: ");
    scanf("%s", idEliminarStr);

    // Validar que el ID sea un numero
    idValido = 1;
    for (i = 0; idEliminarStr[i] != '\0'; i++) {
        if (idEliminarStr[i] < '0' || idEliminarStr[i] > '9') {
            idValido = 0;
            break;
        }
    }

    if (!idValido) {
        cls();
        printf("ID no valido, devolviendo al menu.\n");
        pause();
        cls();
        return; // Retornar al menu principal
    }

    idEliminar = atoi(idEliminarStr);

    temp = farmacia->sucursales;
    if (temp == NULL) {
        mostrarMensajeNoSucursales();
        return;
    }

    resultado = eliminarSucursal(farmacia, idEliminar);

    if (resultado == 0) {
        mostrarMensajeSucursalNoEncontrada();
    } else {
        mostrarMensajeSucursalEliminada();
    }
}

void imprimirFechaCaducidadInvalidaMenu() {
    printf("Fecha de caducidad invalida. Por favor, ingrese una fecha valida.\n");
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
    struct NodoProveedor *actual;
    actual = farmacia->proveedores;
    while (actual != NULL) {
        if (actual->datosProveedor->id == id) {
            return 1;  // Proveedor encontrado
        }
        actual = actual->sig;
    }
    return 0;  // Proveedor no encontrado
}

// Funcion para validar si una cadena contiene solo numeros
int validarSoloNumeros(char *str) {
    int i;
    for (i = 0; str[i] != '\0'; i++) {
        if (str[i] < '0' || str[i] > '9') {
            return 0;  // No es un numero
        }
    }
    return 1;  // Es un numero
}

void leerDatosProveedor(int *id, char **nombre, char **direccion, char **telefono) {
    char idStr[20];  // Asumiendo que el id no excedera los 19 digitos
    cls();
    printf("Ingrese ID del proveedor: ");
    scanf("%s", idStr);

    // Validar que el ID solo contenga numeros
    if (!validarSoloNumeros(idStr)) {
        cls();
        printf("El ID debe contener solo numeros.\n");
        *id = -1;  // Asignar un valor invalido a id para indicar error
        pause();
        return;
    }

    *id = atoi(idStr);  // Convertir la cadena a numero una vez validado

    *nombre = (char *)malloc(50 * sizeof(char));
    if (*nombre == NULL) {
        printf("Error al asignar memoria para el nombre del proveedor.\n");
        return;
    }
    printf("Ingrese nombre del proveedor: ");
    scanf(" %[^\n]", *nombre);

    *direccion = (char *)malloc(100 * sizeof(char));
    if (*direccion == NULL) {
        printf("Error al asignar memoria para la direccion del proveedor.\n");
        return;
    }
    printf("Ingrese direccion del proveedor: ");
    scanf(" %[^\n]", *direccion);

    *telefono = (char *)malloc(15 * sizeof(char));
    if (*telefono == NULL) {
        printf("Error al asignar memoria para el telefono del proveedor.\n");
        return;
    }
    printf("Ingrese telefono del proveedor: +569");
    scanf("%s", *telefono);

    // Validar que el telefono solo contenga numeros y tenga exactamente 8 digitos
    if (!validarSoloNumeros(*telefono) || strlen(*telefono) != 8) {
        cls();
        printf("El telefono debe contener solo numeros y ser de 8 digitos.\n");
        free(*telefono);
        *telefono = NULL;  // Asignar un valor invalido a telefono para indicar error
        pause();
        return;
    }
}

void mostrarMensajeProveedorExiste() {
    cls();
    printf("Error: Ya existe un proveedor con el mismo ID.\n");
    pause();
}

void mostrarMensajeProveedorAgregado() {
    cls();
    printf("Proveedor agregado con exito.\n");
    pause();
}

void crearProveedorView(struct FarmaSalud *farmacia) {
    int id;
    char *nombre, *direccion, *telefono;
    struct NodoProveedor *nuevoNodo;
    struct Proveedor *nuevoProveedor;

    leerDatosProveedor(&id, &nombre, &direccion, &telefono);

    if (id == -1 || nombre == NULL || direccion == NULL || telefono == NULL) {
        return;
    }

    if (existeProveedor(farmacia, id)) {
        mostrarMensajeProveedorExiste();
        return;
    }

    nuevoNodo = (struct NodoProveedor*)malloc(sizeof(struct NodoProveedor));
    if (!nuevoNodo) {
        printf("Error al asignar memoria para nuevo nodo proveedor.\n");
        return;
    }

    nuevoProveedor = (struct Proveedor*)malloc(sizeof(struct Proveedor));
    if (!nuevoProveedor) {
        printf("Error al asignar memoria para nuevo proveedor.\n");
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
    char requiereReceta[4];
    struct Producto *producto;
    if (nodoProducto == NULL) {
        return;
    }

    mostrarProductosProveedor(nodoProducto->izq);

    producto = nodoProducto->datosProducto;

    if (producto->requiereReceta) {
        strcpy(requiereReceta, "Si");
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
    printf("Direccion: %s\n", proveedor->direccion);
    printf("Telefono: +569%s\n", proveedor->telefono);

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
    struct NodoProveedor *nodoActual;
    cls();
    nodoActual = farmacia->proveedores;
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
    char idStr[20];  // Asumiendo que el id no excedera los 19 digitos
    cls();
    printf("Ingrese el ID del proveedor que desea eliminar: ");
    scanf("%s", idStr);

    // Validar que el ID solo contenga numeros
    if (!validarSoloNumeros(idStr)) {
        cls();
        printf("El ID debe contener solo numeros.\n");
        pause();
        solicitarIdProveedorEliminar(idEliminar);  // Llamar nuevamente a la funcion para solicitar el ID
        return;
    }

    *idEliminar = atoi(idStr);  // Convertir la cadena a numero una vez validado
}

void mostrarProveedorNoEncontrado() {
    cls();
    printf("Proveedor no encontrado.\n");
    pause();
}

void mostrarProveedorEliminado() {
    cls();
    printf("Proveedor eliminado con exito.\n");
    pause();
}

int buscarYEliminarProveedor(struct FarmaSalud *farmacia, int idEliminar) {
    struct NodoProveedor *temp;
    struct NodoProveedor *prev;
    temp = farmacia->proveedores;
    prev = NULL;

    // Buscar el proveedor con el ID proporcionado
    while (temp != NULL && temp->datosProveedor->id != idEliminar) {
        prev = temp;
        temp = temp->sig;
    }

    // Si no se encontro el proveedor con el ID proporcionado
    if (temp == NULL) {
        return 0; // No encontrado
    }

    // Manejar los casos especiales para el primer y ultimo nodo
    if (prev == NULL) {
        farmacia->proveedores = temp->sig;
    } else {
        prev->sig = temp->sig;
    }
    if (temp->sig != NULL) {
        temp->sig->ant = prev;
    }

    // Liberar la memoria asociada al proveedor

    return 1; // Eliminado con exito
}

void eliminarProveedor(struct FarmaSalud *farmacia) {
    int idEliminar;
    cls();

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

int existeProducto(struct FarmaSalud *farmacia, const char *codigo) {
    struct NodoProveedor *proveedorActual;
    proveedorActual = farmacia->proveedores;
    while (proveedorActual != NULL) {
        struct NodoArbolProducto *productoActual = proveedorActual->datosProveedor->productos;
        while (productoActual != NULL) {
            if (strcmp(productoActual->datosProducto->codigo, codigo) == 0) {
                return 1;  // Producto encontrado
            }
            if (strcmp(codigo, productoActual->datosProducto->codigo) < 0) {
                productoActual = productoActual->izq;
            } else {
                productoActual = productoActual->der;
            }
        }
        proveedorActual = proveedorActual->sig;
    }
    return 0;  // Producto no encontrado
}

void solicitarDatosProducto(struct FarmaSalud *farmacia, struct Producto *producto) {
    int idProveedorInt;
    char precioStr[20];  // Para almacenar la cadena de precio temporalmente
    char requiereRecetaStr[2];  // Para almacenar la cadena de requiereReceta temporalmente

    cls();

    while (1) {
        printf("Ingrese ID del proveedor: ");
        scanf(" %49[^\n]", producto->idProveedor);

        // Validar que el ID solo contenga numeros
        if (!validarSoloNumeros(producto->idProveedor)) {
            cls();
            printf("El ID debe contener solo numeros.\n");
            continue;
        }

        idProveedorInt = atoi(producto->idProveedor);

        if (existeProveedor(farmacia, idProveedorInt)) {
            break;
        } else {
            cls();
            printf("ID del proveedor no encontrado. Ingrese un ID valido.\n");
        }
    }

    while (1) {
        printf("Ingrese codigo del producto: ");
        scanf(" %49[^\n]", producto->codigo);

        // Validar que el codigo solo contenga numeros
        if (!validarSoloNumeros(producto->codigo)) {
            cls();
            printf("El codigo debe contener solo numeros.\n");
            continue;
        }

        if (!existeProducto(farmacia, producto->codigo)) {
            break;
        } else {
            cls();
            printf("Codigo del producto ya existe. Ingrese un codigo valido.\n");
        }
    }

    cls();
    printf("Ingrese nombre del producto: ");
    scanf(" %99[^\n]", producto->nombreProducto);
    
    cls();
    printf("Ingrese descripcion del producto: ");
    scanf(" %199[^\n]", producto->descripcion);
    
    cls();
    printf("Ingrese categoria del producto: ");
    scanf(" %49[^\n]", producto->categoria);
    
    while (1) {
        cls();
        printf("Ingrese precio del producto: ");
        scanf(" %19[^\n]", precioStr);

        // Validar que el precio solo contenga numeros
        if (!validarSoloNumeros(precioStr)) {
            cls();
            printf("El precio debe contener solo numeros.\n");
            continue;
        }

        producto->precio = atoi(precioStr);
        break;
    }

    while (1) {
        cls();
        printf("¿Requiere receta? (1-Si, 0-No): ");
        scanf(" %1[^\n]", requiereRecetaStr);

        // Validar que solo sea '1' o '0'
        if (strcmp(requiereRecetaStr, "1") != 0 && strcmp(requiereRecetaStr, "0") != 0) {
            cls();
            printf("Debe ingresar 1 para Si o 0 para No.\n");
            continue;
        }

        producto->requiereReceta = atoi(requiereRecetaStr);
        break;
    }
}

struct Producto* crearProducto() {
    struct Producto *nuevoProducto = (struct Producto *)malloc(sizeof(struct Producto));
    nuevoProducto->nombreProducto = (char *)malloc(50 * sizeof(char));
    nuevoProducto->descripcion = (char *)malloc(100 * sizeof(char));
    nuevoProducto->categoria = (char *)malloc(50 * sizeof(char));
    nuevoProducto->idProveedor = (char *)malloc(20 * sizeof(char));
    nuevoProducto->lote = my_strdup("N/A");
    nuevoProducto->fechaCaducidad = my_strdup("N/A");
    nuevoProducto->cantidad = -1;
    return nuevoProducto;
}

int agregarProductoProveedorModelo(struct FarmaSalud *farmacia, struct Producto *nuevoProducto) {
    struct NodoProveedor *proveedorActual;
    int idProveedorInt;
    struct NodoArbolProducto *nuevoNodoProducto;
    struct NodoArbolProducto *actualProducto;
    struct NodoArbolProducto *padreProducto;

    idProveedorInt = atoi(nuevoProducto->idProveedor);
    proveedorActual = farmacia->proveedores;
    while (proveedorActual != NULL && proveedorActual->datosProveedor->id != idProveedorInt) {
        proveedorActual = proveedorActual->sig;
    }

    if (proveedorActual == NULL) {
        return 0;
    }

    nuevoNodoProducto = (struct NodoArbolProducto *)malloc(sizeof(struct NodoArbolProducto));
    nuevoNodoProducto->datosProducto = nuevoProducto;
    nuevoNodoProducto->izq = nuevoNodoProducto->der = NULL;

    if (proveedorActual->datosProveedor->productos == NULL) {
        proveedorActual->datosProveedor->productos = nuevoNodoProducto;
    } else {
        actualProducto = proveedorActual->datosProveedor->productos;
        padreProducto = NULL;

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
    struct Producto *nuevoProducto;
    int resultado;
    nuevoProducto = crearProducto();
    solicitarDatosProducto(farmacia, nuevoProducto);  // Pasamos farmacia para la validacion de ID y producto
    resultado = agregarProductoProveedorModelo(farmacia, nuevoProducto);
    if (resultado) {
        cls();
        printf("Producto agregado con exito al proveedor.\n");
    } else {
        cls();
        printf("Proveedor no encontrado.\n");
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
    printf("Error: No se puede agregar mas registros de envios, capacidad maxima alcanzada.\n");
    pause();
}

void printRegistroEnvio(const char *registro) {
    printf("%s\n", registro);
}

void printNoSucursalesRegistradas() {
    printf("No hay sucursales registradas.\n");
}

void printSucursalInfo(struct Sucursal *sucursal) {
    int i;
    int espacio;
    espacio = 10000 - espacioRestante(sucursal);
    printf("ID: %d\n", sucursal->id);
    printf("Nombre: %s\n", sucursal->nombre);
    printf("Direccion: %s\n", sucursal->direccion);
    printf("Cantidad de Ventas: %d\n", sucursal->cantidadDeVentas);
    printf("Capacidad de Almacenamiento: %d / 10000\n", espacio);
    printf("Numero de Registros de Envios: %d\n", sucursal->numRegistros);
    for (i = 0; i < sucursal->numRegistros; i++) {
        printf("\tRegistro de Envio %d: %s\n", i + 1, sucursal->registrosEnvios[i]);
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

// Funcion para obtener la fecha
void obtenerFecha(char *fecha) {
    int dia, mes;
    printIngresarMes();
    scanf("%d", &mes);
    printIngresarDia();
    scanf("%d", &dia);
    sprintf(fecha, "%02d/%02d", mes, dia);
}

// Funcion para concatenar dos cadenas
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

// Funcion para agregar un registro de envio a una sucursal
void agregarRegistroEnvio(struct Sucursal *sucursal, const char *proveedorNombre) {
    char fecha[6];
    char registro[200];

    if (sucursal->numRegistros >= MAX_ENVIOS) {
        printErrorCapacidadMaxima();
        return;
    }


    obtenerFecha(fecha);
    strcpy(registro, "Productos enviados de Proveedor ");
    my_strcat(registro, proveedorNombre);
    my_strcat(registro, " a Sucursal ");
    my_strcat(registro, sucursal->nombre);
    my_strcat(registro, " el dia ");
    my_strcat(registro, fecha);

    sucursal->registrosEnvios[sucursal->numRegistros] = (char *)malloc(strlen(registro) + 1);
    if (sucursal->registrosEnvios[sucursal->numRegistros] != NULL) {
        strcpy(sucursal->registrosEnvios[sucursal->numRegistros], registro);
        sucursal->numRegistros++;
    }
}

// Funcion para buscar una sucursal por ID
struct NodoSucursales* buscarSucursalPorID(struct FarmaSalud *farmacia, int idSucursal) {
    struct NodoSucursales *sucursalActual;
    struct NodoSucursales *inicio;

    sucursalActual = farmacia->sucursales;
    if (sucursalActual == NULL) {
        return NULL;
    }

    inicio = sucursalActual;
    do {
        if (sucursalActual->datosSucursal->id == idSucursal) {
            return sucursalActual;
        }
        sucursalActual = sucursalActual->sig;
    } while (sucursalActual != inicio);

    return NULL;
}

// Funcion para buscar un proveedor por ID
struct NodoProveedor* buscarProveedorPorID(struct FarmaSalud *farmacia, int idProveedor) {
    struct NodoProveedor *proveedorActual;
    proveedorActual = farmacia->proveedores;
    while (proveedorActual != NULL && proveedorActual->datosProveedor->id != idProveedor) {
        proveedorActual = proveedorActual->sig;
    }
    return proveedorActual;
}

void mostrarSucursales(struct FarmaSalud *farmacia) {
    struct NodoSucursales *sucursalActual;
    sucursalActual = farmacia->sucursales;
    if (sucursalActual == NULL) {
        cls();
        printNoSucursalesRegistradas();
        pause();
        return;
    }

    do {
        printSucursalInfo(sucursalActual->datosSucursal);
        sucursalActual = sucursalActual->sig;
    } while (sucursalActual != farmacia->sucursales);
}

// Funcion para transferir productos de un proveedor a una sucursal
void transferirProductosProveedorASucursal(struct FarmaSalud *farmacia) {
    struct NodoProveedor *proveedorActual;
    struct NodoSucursales *sucursalActual;
    struct NodoArbolProducto *nodoProducto;
    struct Producto *nuevoProducto;
    struct NodoProducto *nuevoNodoProducto;
    int idProveedor, idSucursal;
    char fechaCaducidad[11];
    char lote[50];
    int cantidad;

    cls();

    mostrarProveedores(farmacia);
    printIngreseIDProveedor();
    scanf("%d", &idProveedor);

    proveedorActual = buscarProveedorPorID(farmacia, idProveedor);
    if (proveedorActual == NULL) {
        printProveedorNoEncontrado(idProveedor);
        pause();
        return;
    }

    cls();
    mostrarSucursales(farmacia);
    printIngreseIDSucursal();
    scanf("%d", &idSucursal);

    sucursalActual = buscarSucursalPorID(farmacia, idSucursal);
    if (sucursalActual == NULL) {
        cls();
        printSucursalNoEncontrada();
        pause();
        return;
    }

    nodoProducto = proveedorActual->datosProveedor->productos;
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

        nuevoProducto = (struct Producto *)malloc(sizeof(struct Producto));
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

        nuevoNodoProducto = (struct NodoProducto *)malloc(sizeof(struct NodoProducto));
        if (nuevoNodoProducto == NULL) {
            printErrorAsignarMemoriaNodoProducto();
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

void mostrarYSeleccionarProveedor(struct FarmaSalud *farmacia, int *idProveedor, struct NodoProveedor **proveedorActual) {
    mostrarProveedores(farmacia);
    printPromptProveedor();
    scanf("%d", idProveedor);

    *proveedorActual = buscarProveedorPorID(farmacia, *idProveedor);
    if (*proveedorActual == NULL) {
        printProveedorNoEncontrado(*idProveedor);
    }
}

void mostrarYSeleccionarSucursal(struct FarmaSalud *farmacia, int *idSucursal, struct NodoSucursales **sucursalActual) {
    mostrarSucursales(farmacia);
    printPromptSucursal();
    scanf("%d", idSucursal);

    *sucursalActual = buscarSucursalPorID(farmacia, *idSucursal);
    if (*sucursalActual == NULL) {
        printSucursalNoEncontrada();
    }
}

void mostrarYSeleccionarProducto(struct NodoProveedor *proveedorActual, char *codigoProducto, struct Producto **productoSeleccionado) {
    struct NodoArbolProducto *nodoProducto;
    printf("Productos del proveedor:\n");
    nodoProducto = proveedorActual->datosProveedor->productos;
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
        cls();
        printProductoCodigoNoEncontrado(codigoProducto);
        pause();
    }
}

// funcion tranferir producto de proveedor a sucursal
void transferirProductoProveedorASucursal(struct FarmaSalud *farmacia)
{
    struct NodoProveedor *proveedorActual;
    struct NodoSucursales *sucursalActual;
    struct Producto *productoSeleccionado;
    struct Producto *nuevoProducto;
    struct NodoProducto *nuevoNodoProducto;
    int idProveedor, idSucursal;
    char fechaCaducidad[11];
    char lote[50];
    int cantidad;
    char codigoProducto[10];

    cls(); // Limpia la pantalla

    proveedorActual = NULL;
    mostrarYSeleccionarProveedor(farmacia, &idProveedor, &proveedorActual);
    if (proveedorActual == NULL) {
        return;
    }

    cls();
    sucursalActual = NULL;
    mostrarYSeleccionarSucursal(farmacia, &idSucursal, &sucursalActual);
    if (sucursalActual == NULL) {
        return;
    }

    cls();
    productoSeleccionado = NULL;
    mostrarYSeleccionarProducto(proveedorActual, codigoProducto, &productoSeleccionado);
    if (productoSeleccionado == NULL) {
        return;
    }

    obtenerDetallesProducto(fechaCaducidad, lote, &cantidad, productoSeleccionado->nombreProducto);

    // Crear una copia completa del producto
    nuevoProducto = (struct Producto *)malloc(sizeof(struct Producto));
    if (nuevoProducto == NULL) {
        printf("Error al asignar memoria para el nuevo producto.\n");
        return;
    }
    // Copiar datos del producto original al nuevo producto
    strcpy(nuevoProducto->codigo, productoSeleccionado->codigo);
    nuevoProducto->nombreProducto = my_strdup(productoSeleccionado->nombreProducto);
    nuevoProducto->descripcion = my_strdup(productoSeleccionado->descripcion);
    nuevoProducto->categoria = my_strdup(productoSeleccionado->categoria);
    nuevoProducto->precio = productoSeleccionado->precio;
    nuevoProducto->idProveedor = my_strdup(productoSeleccionado->idProveedor);
    nuevoProducto->lote = my_strdup(lote);  // Usar el valor ingresado por el usuario
    nuevoProducto->fechaCaducidad = my_strdup(fechaCaducidad);  // Usar el valor ingresado por el usuario
    nuevoProducto->cantidad = cantidad;  // Usar el valor ingresado por el usuario
    nuevoProducto->requiereReceta = productoSeleccionado->requiereReceta;

    // Crear un nuevo nodo producto
    nuevoNodoProducto = (struct NodoProducto *)malloc(sizeof(struct NodoProducto));
    if (nuevoNodoProducto == NULL) {
        printf("Error al asignar memoria para el nuevo nodo de producto.\n");
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

// Funcion para extraer el mes y anio de una fecha en formato MM/AAAA
void extraerFecha(const char *fecha, int *mes, int *anio) {
    sscanf(fecha, "%d/%d", mes, anio);
}

// Funcion para comparar dos fechas en formato MM/AAAA
void imprimirSucursalOProductosInvalidos() {
    printf("Sucursal o productos no validos.\n");
}

void imprimirFechaCaducidadInvalida() {
    printf("Fecha de caducidad invalida.\n");
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

void checkImprimirVencidos() {
    printf("Productos vencidos eliminados correctamente.\n");
}

void imprimirIngreseIDSucursal() {
    printf("Ingrese el ID de la sucursal (o -1 para todas las sucursales): ");
}

void imprimirIngreseFechaVencimiento() {
    printf("Ingrese la fecha de vencimiento (MM/AAAA): ");
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
    struct NodoProducto *productoActual;
    struct NodoProducto *siguienteProducto;
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

    productoActual = sucursal->productos;
    while (productoActual != NULL) {
        siguienteProducto = productoActual->sig;
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
    struct NodoSucursales *sucursalActual;
    if (farmacia == NULL || farmacia->sucursales == NULL) {
        imprimirNoHaySucursalesRegistradas();
        return;
    }

    if (!validarFecha(fechaLimite)) {
        imprimirFechaCaducidadInvalida();
        return;
    }

    sucursalActual = farmacia->sucursales;
    do {
        eliminarProductosVencidos(sucursalActual->datosSucursal, fechaLimite);
        sucursalActual = sucursalActual->sig;
    } while (sucursalActual != farmacia->sucursales);
    cls();
    imprimirProductosVencidosEliminadosDeTodasLasSucursales();
    pause();
}

void menuEliminarProductosVencidos(struct FarmaSalud *farmacia) {
    int idSucursal;
    char fechaLimite[8];

    cls();

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
        checkImprimirVencidos();
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
    printf("Entrada invalida.\n");
}

void printClienteRegistrado() {
    printf("El cliente esta registrado (1-Si, 0-No): ");
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
    printf("Cantidad de productos invalida.\n");
}

void printIngreseCodigoProducto(int index) {
    printf("Ingrese el codigo del producto %d: ", index + 1);
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
    printf("El producto %s requiere receta. El cliente tiene la receta (1-Si, 0-No): ", nombreProducto);
}

void printCompraSinReceta(char* nombreProducto) {
    printf("No se puede completar la compra del producto %s sin receta.\n", nombreProducto);
}

void printBoletaFisicaHeader() {
    printf("\n=======================================\n");
    printf("                 FarmaSalud\n");
    printf("           La farmacia del pueblo\n");
    printf("               Boleta Fisica\n");
    printf("=======================================\n");
}

void printSucursalNombre(char* nombre) {
    printf("Sucursal: %s\n", nombre);
}

void printClienteNombre(char* rutCliente) {
    printf("Cliente: %s\n", rutCliente);
}

void printCompraAnonima() {
    printf("Cliente: Compra anonima\n");
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
    struct NodoClientes *clienteActual;
    clienteActual = farmacia->clientes;
    while (clienteActual != NULL && clienteActual->datosClientes->id != idCliente) {
        clienteActual = clienteActual->sig;
    }
    return clienteActual;
}

struct Producto* crearProductoVendido(struct Producto *producto) {
    struct Producto *nuevoProducto = (struct Producto*)malloc(sizeof(struct Producto));
    strcpy(nuevoProducto->codigo, producto->codigo);
    nuevoProducto->nombreProducto = my_strdup(producto->nombreProducto);
    nuevoProducto->descripcion = my_strdup(producto->descripcion);
    nuevoProducto->categoria = my_strdup(producto->categoria);
    nuevoProducto->precio = producto->precio;
    nuevoProducto->idProveedor = my_strdup(producto->idProveedor);
    nuevoProducto->lote = my_strdup("N/A");
    nuevoProducto->fechaCaducidad = my_strdup("N/A");
    nuevoProducto->cantidad = 1;
    nuevoProducto->requiereReceta = producto->requiereReceta;
    return nuevoProducto;
}

struct Producto* buscarProductoEnSucursal(struct Sucursal *sucursal, char *codigoProducto) {
    struct NodoProducto *productoNodo;
    productoNodo = sucursal->productos;
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
    struct NodoProducto *productoNodo;
    productoNodo = productosVendidos;
    while (productoNodo != NULL) {
        if (strcmp(productoNodo->datosProducto->codigo, codigoProducto) == 0) {
            return productoNodo->datosProducto;
        }
        productoNodo = productoNodo->sig;
    }
    return NULL;
}

void mostrarClientes(struct FarmaSalud *farmacia) {
    struct NodoClientes *actual;
    actual = farmacia->clientes;

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

void printAnnoInvalido() {
    printf("Año invalido.\n");
}

void printMesInvalido() {
    printf("Mes invalido.\n");
}

void realizarCompra(struct FarmaSalud *farmacia) {
    int idSucursal, idCliente, cantidadCompras, cantidadProducto, i, j;
    struct Producto **productosComprados;
    int *cantidades;
    int *productosRequierenReceta;
    struct NodoClientes *cliente;
    struct NodoSucursales *sucursal;
    struct Producto *producto;
    struct Producto *productoVendido;
    int espacio;
    char codigoProducto[10];
    int esClienteRegistrado;
    int tieneReceta;
    int total;
    int mesVenta;
    char annoVentaStr[5];  // String para el año
    int annoVenta;

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
    espacio = 10000 - espacioRestante(sucursal->datosSucursal);
    if (scanf("%d", &cantidadCompras) != 1 || cantidadCompras <= 0 || cantidadCompras > espacio) {
        printCantidadProductosInvalida();
        pause();
        return;
    }
    cls();

    productosComprados = (struct Producto **)malloc(cantidadCompras * sizeof(struct Producto *));
    cantidades = (int *)malloc(cantidadCompras * sizeof(int));
    productosRequierenReceta = (int *)malloc(cantidadCompras * sizeof(int));

    // Mostrar los productos disponibles en la sucursal seleccionada
    mostrarSucursal(sucursal);
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

        producto = buscarProductoEnSucursal(sucursal->datosSucursal, codigoProducto);
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

    // Captura del mes y año de la venta
    cls();
    printf("Ingrese el mes de la venta (1-12): ");
    if (scanf("%d", &mesVenta) != 1 || mesVenta < 1 || mesVenta > 12) {
        printMesInvalido();
        pause();
        return;
    }

    // Validacion para el año de venta
    while (1) {
        printf("Ingrese el año de la venta (cuatro digitos): ");
        if (scanf("%4s", annoVentaStr) != 1 || strlen(annoVentaStr) != 4 || sscanf(annoVentaStr, "%d", &annoVenta) != 1 || annoVenta < 1000 || annoVenta > 9999) {
            printAnnoInvalido();
            pause();
        } else {
            break;
        }
    }

    // Asignar mes y año de venta a cada producto vendido que requiere receta
    for (i = 0; i < cantidadCompras; i++) {
        if (productosRequierenReceta[i]) {
            productosComprados[i]->cantidad -= cantidades[i];

            productoVendido = buscarProductoEnVendidos(sucursal->datosSucursal->productosVendidos, productosComprados[i]->codigo);
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

            // Asignar mes y año de venta al producto vendido
            productoVendido->mesVenta = mesVenta;
            productoVendido->anioVenta = annoVenta;
        }
    }

    sucursal->datosSucursal->cantidadDeVentas++;

    // Impresion de la boleta
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
    total = 0;
    for (i = 0; i < cantidadCompras; i++) {
        if (productosRequierenReceta[i]) {
            total += productosComprados[i]->precio * cantidades[i];
        }
    }
    printTotal(total);

    pause();
}

// Controlador
void eliminarProducto(struct NodoProducto **productoActual, struct NodoSucursales *sucursalActual) {
    struct NodoProducto *siguienteProducto;
    siguienteProducto = (*productoActual)->sig;
    if ((*productoActual)->ant != NULL) {
        (*productoActual)->ant->sig = (*productoActual)->sig;
    } else {
        // Si es el primer producto, actualizamos el puntero inicial de productos
        sucursalActual->datosSucursal->productos = (*productoActual)->sig;
    }

    if ((*productoActual)->sig != NULL) {
        (*productoActual)->sig->ant = (*productoActual)->ant;
    }

    *productoActual = siguienteProducto;
}

// Vista
void imprimirActualizacionInventario() {
    printf("Inventarios actualizados. Productos con stock 0 eliminados.\n");
}

// Funcion principal
void actualizarInventariosSucursales(struct FarmaSalud *farmacia) {
    struct NodoProducto *productoActual;
    struct NodoSucursales *sucursalActual;

    if (farmacia == NULL || farmacia->sucursales == NULL) {
        printf("No hay sucursales registradas.\n");
        return;
    }

    sucursalActual = farmacia->sucursales;

    // Iterar sobre cada sucursal
    do {
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

struct Sucursal *getSucursalConMasVentas(struct NodoSucursales *head) {
    struct NodoSucursales *rec;
    struct Sucursal *sucursalMasVentas = NULL;
    int maxVentas = -1;
    int contadorMax = 0;

    if (head != NULL) {
        rec = head;
        do {
            if (sucursalMasVentas == NULL || rec->datosSucursal->cantidadDeVentas > maxVentas) {
                maxVentas = rec->datosSucursal->cantidadDeVentas;
                sucursalMasVentas = rec->datosSucursal;
                contadorMax = 1;
            } else if (rec->datosSucursal->cantidadDeVentas == maxVentas) {
                contadorMax++;
            }
            rec = rec->sig;
        } while (rec != head);
    }

    if (contadorMax > 1) {
        return NULL;
    }

    return sucursalMasVentas;
}

void mostrarSucursalConMasVentas(struct FarmaSalud *farmaSalud) {
    struct Sucursal *sucursalMasVentas;

    if (!farmaSalud || !farmaSalud->sucursales) {
        cls();
        printf("\nNo existen sucursales en el sistema.\n");
        pause();
        return;
    }

    sucursalMasVentas = getSucursalConMasVentas(farmaSalud->sucursales);
    if (!sucursalMasVentas) {
        cls();
        printf("\nHay mas de una sucursal con la misma cantidad de ventas.\n");
        return;
    }

    cls();
    printf("\nLa sucursal con mas ventas es:\n");
    printf("ID: %d\n", sucursalMasVentas->id);
    printf("Nombre: %s\n", sucursalMasVentas->nombre);
    printf("Direccion: %s\n", sucursalMasVentas->direccion);
    printf("Cantidad de Ventas: %d\n", sucursalMasVentas->cantidadDeVentas);
}

float calcularEdadPromedioClientes(struct FarmaSalud *farmaSalud) {
    int totalClientes = 0;
    int sumaEdades = 0;
    struct NodoClientes *rec = farmaSalud->clientes;

    if (!farmaSalud || !farmaSalud->clientes) {
        printf("\nNo existen clientes en el sistema.\n");
        return 0.0;
    }

    // Sumar las edades de todos los clientes
    while (rec) {
        sumaEdades += rec->datosClientes->edadCliente;
        totalClientes++;
        rec = rec->sig;
    }

    if (totalClientes == 0) {
        return 0.0;
    }

    return (float)sumaEdades / totalClientes;
}

void mostrarEdadPromedioClientes(struct FarmaSalud *farmaSalud) {
    float edadPromedio = calcularEdadPromedioClientes(farmaSalud);
    printf("\nLa edad promedio de los clientes es: %.2f\n", edadPromedio);
}

float calcularPromedioVentas(struct FarmaSalud *farmaSalud) {
    int totalVentas = 0;
    int contadorSucursales = 0;
    struct NodoSucursales *rec = farmaSalud->sucursales;

    if (!farmaSalud || !farmaSalud->sucursales) {
        printf("\nNo existen sucursales en el sistema.\n");
        return 0.0;
    }

    // Sumar las ventas de todas las sucursales
    do {
        totalVentas += rec->datosSucursal->cantidadDeVentas;
        contadorSucursales++;
        rec = rec->sig;
    } while (rec != farmaSalud->sucursales);

    if (contadorSucursales == 0) {
        return 0.0;
    }

    return (float)totalVentas / contadorSucursales;
}

int contarSucursalesConVentasBajoPromedio(struct FarmaSalud *farmaSalud) {
    float promedioVentas = calcularPromedioVentas(farmaSalud);
    int contadorSucursales = 0;
    struct NodoSucursales *rec = farmaSalud->sucursales;

    if (promedioVentas == 0.0) {
        return 0;
    }

    // Contar las sucursales con ventas menores que el promedio
    do {
        if (rec->datosSucursal->cantidadDeVentas < promedioVentas) {
            contadorSucursales++;
        }
        rec = rec->sig;
    } while (rec != farmaSalud->sucursales);

    return contadorSucursales;
}

void mostrarPorcentajeSucursalesConVentasBajoPromedio(struct FarmaSalud *farmaSalud) {
    int totalSucursales = 0;
    float porcentaje;
    int sucursalesBajoPromedio;
    struct NodoSucursales *rec = farmaSalud->sucursales;

    if (!farmaSalud || !farmaSalud->sucursales) {
        printf("\nNo existen sucursales en el sistema.\n");
        pause();
        return;
    }

    do {
        totalSucursales++;
        rec = rec->sig;
    } while (rec != farmaSalud->sucursales);

    sucursalesBajoPromedio = contarSucursalesConVentasBajoPromedio(farmaSalud);
    porcentaje = ((float)sucursalesBajoPromedio / totalSucursales) * 100;

    printf("El porcentaje de sucursales con ventas por debajo del promedio es: %.2f%%\n", porcentaje);
    pause();
}

float calcularPorcentajeClientesAfiliados(struct FarmaSalud *farmaSalud) {
    int totalClientes = 0;
    int totalAfiliados = 0;
    struct NodoClientes *rec = farmaSalud->clientes;

    if (!farmaSalud || !farmaSalud->clientes) {
        printf("\nNo existen clientes en el sistema.\n");
        return 0.0;
    }

    // Contar el total de clientes y los afiliados
    while (rec) {
        if (rec->datosClientes->afiliado) {
            totalAfiliados++;
        }
        totalClientes++;
        rec = rec->sig;
    }

    if (totalClientes == 0) {
        return 0.0;
    }

    return ((float)totalAfiliados / totalClientes) * 100;
}

void mostrarPorcentajeClientesAfiliados(struct FarmaSalud *farmaSalud) {
    float porcentajeAfiliados = calcularPorcentajeClientesAfiliados(farmaSalud);
    printf("El porcentaje de clientes afiliados es: %.2f%%\n", porcentajeAfiliados);
}

// Funcion auxiliar para verificar si un mes esta en un arreglo de meses
int mesEnLista(int mes, int lista[]) {
    int i;
    for (i = 0; i < 3; i++) {
        if (mes == lista[i]) {
            return 1;
        }
    }
    return 0;
}

// Funcion para calcular el total de transacciones de una categoria
int totalTransaccionesDeCategoria(struct NodoProducto *ventas, char *categoria) {
    int total = 0;
    struct NodoProducto *rec = ventas;

    while (rec != NULL) {
        if (strcmp(rec->datosProducto->categoria, categoria) == 0) {
            total += rec->datosProducto->cantidad;
        }
        rec = rec->sig;
    }
    return total;
}

// Funcion para obtener la categoria mas vendida en una estacion del año
char* getCategoriaMasVendidaEstacion(struct NodoProducto *ventas, char estacion) {
    struct NodoProducto *rec;
    char *categoriaMasVendida = NULL;
    int maxVentas = 0, condicion, numeroMes;

    if (ventas == NULL) {
        return NULL;
    }

    rec = ventas;
    while (rec != NULL) {
        numeroMes = rec->datosProducto->mesVenta;
        switch (estacion) {
            case 'P': // Primavera
                condicion = numeroMes >= 9 && numeroMes <= 11;
                break;
            case 'V': // Verano
                condicion = numeroMes == 12 || numeroMes <= 2;
                break;
            case 'O': // Otoño
                condicion = numeroMes >= 3 && numeroMes <= 5;
                break;
            case 'I': // Invierno
                condicion = numeroMes >= 6 && numeroMes <= 8;
                break;
            default:
                condicion = 0;
                break;
        }

        if (condicion) {
            int ventasCategoria = totalTransaccionesDeCategoria(ventas, rec->datosProducto->categoria);
            if (categoriaMasVendida == NULL || ventasCategoria > maxVentas) {
                maxVentas = ventasCategoria;
                categoriaMasVendida = rec->datosProducto->categoria;
            }
        }
        rec = rec->sig;
    }
    return categoriaMasVendida;
}

// Funcion principal para calcular y mostrar la categoria y producto mas vendido por estacion del año
void calcularMasVendidoPorEstacion(struct FarmaSalud *farmacia) {
    char *categoriaMasVendidaPrimavera, *categoriaMasVendidaVerano, *categoriaMasVendidaOtonio, *categoriaMasVendidaInvierno;
    struct NodoSucursales *nodoSucursal;
    struct NodoSucursales *primerNodoSucursal;
    struct NodoProducto *ventas;
    char *categoriaPrimavera, *categoriaVerano, *categoriaOtonio, *categoriaInvierno;

    // Inicializacion de variables
    categoriaMasVendidaPrimavera = categoriaMasVendidaVerano = categoriaMasVendidaOtonio = categoriaMasVendidaInvierno = NULL;

    if (farmacia == NULL || farmacia->sucursales == NULL) {
        printf("No hay datos disponibles.\n");
        return;
    }

    nodoSucursal = farmacia->sucursales;
    primerNodoSucursal = nodoSucursal;

    do {
        if (nodoSucursal == NULL || nodoSucursal->datosSucursal == NULL) break;

        ventas = nodoSucursal->datosSucursal->productosVendidos;

        categoriaPrimavera = getCategoriaMasVendidaEstacion(ventas, 'P');
        categoriaVerano = getCategoriaMasVendidaEstacion(ventas, 'V');
        categoriaOtonio = getCategoriaMasVendidaEstacion(ventas, 'O');
        categoriaInvierno = getCategoriaMasVendidaEstacion(ventas, 'I');

        // Actualizacion de categorias mas vendidas
        if (categoriaPrimavera != NULL && (categoriaMasVendidaPrimavera == NULL ||
            totalTransaccionesDeCategoria(ventas, categoriaPrimavera) >
            totalTransaccionesDeCategoria(ventas, categoriaMasVendidaPrimavera))) {
            categoriaMasVendidaPrimavera = categoriaPrimavera;
        }
        if (categoriaVerano != NULL && (categoriaMasVendidaVerano == NULL ||
            totalTransaccionesDeCategoria(ventas, categoriaVerano) >
            totalTransaccionesDeCategoria(ventas, categoriaMasVendidaVerano))) {
            categoriaMasVendidaVerano = categoriaVerano;
        }
        if (categoriaOtonio != NULL && (categoriaMasVendidaOtonio == NULL ||
            totalTransaccionesDeCategoria(ventas, categoriaOtonio) >
            totalTransaccionesDeCategoria(ventas, categoriaMasVendidaOtonio))) {
            categoriaMasVendidaOtonio = categoriaOtonio;
        }
        if (categoriaInvierno != NULL && (categoriaMasVendidaInvierno == NULL ||
            totalTransaccionesDeCategoria(ventas, categoriaInvierno) >
            totalTransaccionesDeCategoria(ventas, categoriaMasVendidaInvierno))) {
            categoriaMasVendidaInvierno = categoriaInvierno;
        }

        nodoSucursal = nodoSucursal->sig;
    } while (nodoSucursal != primerNodoSucursal);

    // Impresion de resultados
    cls();
    printf("Verano:\n");
    if (categoriaMasVendidaVerano != NULL) {
        printf("  Categoria mas vendida: %s\n", categoriaMasVendidaVerano);
    }
    else {
        printf("  No hay ventas registradas.\n");
    }

    printf("\nOtonio:\n");
    if (categoriaMasVendidaOtonio != NULL) {
        printf("  Categoria mas vendida: %s\n", categoriaMasVendidaOtonio);
    }
    else {
        printf("  No hay ventas registradas.\n");
    }

    printf("\nInvierno:\n");
    if (categoriaMasVendidaInvierno != NULL) {
        printf("  Categoria mas vendida: %s\n", categoriaMasVendidaInvierno);
    }
    else {
        printf("  No hay ventas registradas.\n");
    }

    printf("\nPrimavera:\n");
    if (categoriaMasVendidaPrimavera != NULL) {
        printf("  Categoria mas vendida: %s\n", categoriaMasVendidaPrimavera);
    } 
    else {
        printf("  No hay ventas registradas.\n");
    }
    printf("\n");
    pause();
}

int productoPerteneceAEstacion(int mesVenta, char estacion) {
    switch (estacion) {
        case 'P':
            return (mesVenta == 9 || mesVenta == 10 || mesVenta == 11);
        case 'V':
            return (mesVenta == 12 || mesVenta == 1 || mesVenta == 2);
        case 'O':
            return (mesVenta == 3 || mesVenta == 4 || mesVenta == 5);
        case 'I':
            return (mesVenta == 6 || mesVenta == 7 || mesVenta == 8);
        default:
            return 0;
    }
}

// Funcion para ordenar productos por ventas
void ordenarProductosPorVentas(struct NodoProducto *ventas, char estacion, char *topProductos[], int topVentas[]) {
    struct Producto *producto;
    int j, k;

    while (ventas != NULL) {
        producto = ventas->datosProducto;
        if (producto != NULL && productoPerteneceAEstacion(producto->mesVenta, estacion)) {
            for (j = 0; j < 5; j++) {
                if (producto->cantidad > topVentas[j]) {
                    for (k = 4; k > j; k--) {
                        topProductos[k] = topProductos[k - 1];
                        topVentas[k] = topVentas[k - 1];
                    }
                    topProductos[j] = producto->nombreProducto;
                    topVentas[j] = producto->cantidad;
                    break;
                }
            }
        }
        ventas = ventas->sig;
    }
}

// Funcion para mostrar el total de sucursales y ventas totales
void mostrarTotalSucursalesVentas(struct FarmaSalud *farmaSalud) {
    struct NodoSucursales *rec;
    int totalSucursales = 0;
    int totalVentas = 0;

    if (!farmaSalud || !farmaSalud->sucursales) {
        printf("\nNo existen sucursales en el sistema.\n");
        return;
    }

    rec = farmaSalud->sucursales;

    // Calcular estadisticas
    do {
        totalVentas += rec->datosSucursal->cantidadDeVentas;
        totalSucursales++;
        rec = rec->sig;
    } while (rec != farmaSalud->sucursales);

    // Imprimir estadisticas
    cls();
    printf("\nTotal de Sucursales: %d\n", totalSucursales);
    printf("Total de Ventas: %d\n", totalVentas);
    pause();
}

// Funcion para mostrar el promedio de ventas por sucursal
void mostrarPromedioVentas(struct FarmaSalud *farmaSalud) {
    struct NodoSucursales *rec;
    int totalSucursales = 0;
    int totalVentas = 0;
    float promedioVentas = 0.0;

    if (!farmaSalud || !farmaSalud->sucursales) {
        printf("\nNo existen sucursales en el sistema.\n");
        return;
    }

    rec = farmaSalud->sucursales;

    // Calcular estadisticas
    do {
        totalVentas += rec->datosSucursal->cantidadDeVentas;
        totalSucursales++;
        rec = rec->sig;
    } while (rec != farmaSalud->sucursales);

    promedioVentas = (float)totalVentas / totalSucursales;

    // Imprimir estadistica
    cls();
    printf("\nPromedio de Ventas por Sucursal: %.2f\n", promedioVentas);
    pause();
}

// Funcion para encontrar la sucursal con mas ventas
void mostrarSucursalMasVentas(struct FarmaSalud *farmaSalud) {
    struct NodoSucursales *rec = NULL;
    int maxVentas = -1;
    int idSucursalMaxVentas = 0;

    if (!farmaSalud || !farmaSalud->sucursales) {
        printf("\nNo existen sucursales en el sistema.\n");
        return;
    }

    rec = farmaSalud->sucursales;
    idSucursalMaxVentas = rec->datosSucursal->id;

    // Encontrar la sucursal con mas ventas
    do {
        int ventas = rec->datosSucursal->cantidadDeVentas;
        if (ventas > maxVentas) {
            maxVentas = ventas;
            idSucursalMaxVentas = rec->datosSucursal->id;
        }
        rec = rec->sig;
    } while (rec != farmaSalud->sucursales);

    // Imprimir estadistica
    cls();
    printf("\nSucursal con mas ventas (ID): %d\n", idSucursalMaxVentas);
    pause();
}

// Funcion para encontrar la sucursal con menos ventas
void mostrarSucursalMenosVentas(struct FarmaSalud *farmaSalud) {
    struct NodoSucursales *rec = NULL;
    int minVentas = 0;
    int idSucursalMinVentas = 0;

    if (!farmaSalud || !farmaSalud->sucursales) {
        printf("\nNo existen sucursales en el sistema.\n");
        return;
    }

    rec = farmaSalud->sucursales;
    minVentas = rec->datosSucursal->cantidadDeVentas;
    idSucursalMinVentas = rec->datosSucursal->id;

    // Encontrar la sucursal con menos ventas
    do {
        int ventas = rec->datosSucursal->cantidadDeVentas;
        if (ventas < minVentas) {
            minVentas = ventas;
            idSucursalMinVentas = rec->datosSucursal->id;
        }
        rec = rec->sig;
    } while (rec != farmaSalud->sucursales);

    // Imprimir estadistica
    cls();
    printf("\nSucursal con menos ventas (ID): %d\n", idSucursalMinVentas);
    pause();
}

void mostrarEstadisticas(struct FarmaSalud *farmaSalud) {
    struct NodoSucursales *rec = farmaSalud->sucursales;
    int totalVentas = 0;
    int maxVentas = -1;
    int minVentas = rec->datosSucursal->cantidadDeVentas; // Inicializa con el primer valor real de ventas
    int idSucursalMenosVentas = rec->datosSucursal->id; // Inicializa con el primer ID de sucursal
    int contadorSucursales = 0;

    if (!farmaSalud || !farmaSalud->sucursales) {
        printf("\nNo existen sucursales en el sistema.\n");
        return;
    }

    // Calcular el total de ventas y encontrar la sucursal con mas y menos ventas
    do {
        int ventas = rec->datosSucursal->cantidadDeVentas;
        totalVentas += ventas;
        if (ventas > maxVentas) {
            maxVentas = ventas;
        }
        if (ventas < minVentas) {
            minVentas = ventas;
            idSucursalMenosVentas = rec->datosSucursal->id;
        }
        contadorSucursales++;
        rec = rec->sig;
    } while (rec != farmaSalud->sucursales);

    // Imprimir estadisticas
    printf("\n\nEstadisticas de Ventas de Sucursales:\n");
    printf("Total de Sucursales: %d\n", contadorSucursales);
    printf("Total de Ventas: %d\n", totalVentas);
    printf("Promedio de Ventas por Sucursal: %.2f\n", (float)totalVentas / contadorSucursales);
    printf("Sucursal con menos ventas (ID): %d\n", idSucursalMenosVentas);
    if (idSucursalMenosVentas == -1) {
        printf("La sucursal con menos ventas no tiene un ID valido asignado.\n");
    }
}

void generarDatosDePrueba(struct FarmaSalud *farmacia) {
    struct Producto *prod1, *prod2, *prod3, *prod4, *prod5, *prod6;
    struct Producto *compras1[2], *compras2[2], *compras3[2], *compras4[2];
    struct NodoClientes *cliente1, *cliente2, *cliente3, *cliente4;
    struct NodoSucursales *sucursal1, *sucursal2, *sucursal3;
    struct NodoProveedor *proveedor1, *proveedor2, *proveedor3;
    struct Producto *producto1, *producto2, *producto3, *producto4, *producto5;
    struct Producto *producto6;

    prod1 = crearProductoFalso("1", "Sertralina 100mg 30 Comp", "Antidepresivo en forma de Capsulas", "Antidepresivo", 6327, "Lab. de Chile", "DRU30", "02/2024", 1, 1);
    prod2 = crearProductoFalso("2", "Clonazepam 2 Mg 30 Comp", "Actua sobre el sistema nervioso central, con propiedades ansioliticas.", "Ansiolitico", 5007, "Lab. de Chile", "K3935", "05/2024", 1, 1);
    prod3 = crearProductoFalso("3", "Tapsin Dia-Noche Plus 18 Comp", "Comprimidos Recubiertos rapido alivio de los sintomas de la gripe", "Antigripal", 2575, "Lab. Maver", "39DMS", "06/2024", 1, 0);
    prod4 = crearProductoFalso("4", "Tapsin Dia Limon 6 Un.", "Para el alivio sintomatico de las molestias del resfrio y la gripe.", "Antigripal", 2500, "Lab. Maver", "KEMBWAL4", "02/2025", 2, 0);
    prod5 = crearProductoFalso("5", "100 Whey Protein Cookies Cream", "Frasco de Proteina 857.5G", "Proteina", 58990, "PharmaCorp", "GIO4HUM4D", "02/2004", 1, 0);
    prod6 = crearProductoFalso("6", "Sustenan Testosterona 250 mg", "Para aliviar la alergia", "Salud Hombre", 32500, "PharmaCorp", "RGFP74", "10/2022", 1, 1);

    compras1[0] = prod1; compras1[1] = prod2;
    compras2[0] = prod3; compras2[1] = prod4;
    compras3[0] = prod5; compras3[1] = prod6;
    compras4[0] = prod1; compras4[1] = prod3;

    // Creacion de clientes
    cliente1 = crearClienteConsole(1, "8032837-5", 25, 1, compras1, 2);
    agregarClienteConsole(farmacia, cliente1);
    cliente2 = crearClienteConsole(2, "22222222-2", 25, 0, compras2, 2);
    agregarClienteConsole(farmacia, cliente2);
    cliente3 = crearClienteConsole(3, "33333333-3", 30, 1, compras3, 2);
    agregarClienteConsole(farmacia, cliente3);
    cliente4 = crearClienteConsole(4, "44444444-4", 35, 0, compras4, 2);
    agregarClienteConsole(farmacia, cliente4);

    // Creacion de sucursales
    sucursal1 = crearSucursalConsole(1, "Kennedy", "Apoquindo 234, Santiago", 100);
    agregarSucursalConsole(farmacia, sucursal1);
    sucursal2 = crearSucursalConsole(2, "Los Heroes", "Metro Pedro de Valdivia, Santiago", 200);
    agregarSucursalConsole(farmacia, sucursal2);
    sucursal3 = crearSucursalConsole(3, "La Florida", "Vicuña Mackenna 4500, Santiago", 150);
    agregarSucursalConsole(farmacia, sucursal3);

    // Creacion de proveedores
    proveedor1 = crearProveedorConsole(1, "Lab. de Chile", "Av. Marathon 1315", "90322000-7");
    agregarProveedorConsole(farmacia, proveedor1);
    proveedor2 = crearProveedorConsole(2, "Lab. Maver", "Las Encinas 1777", "98765432-1");
    agregarProveedorConsole(farmacia, proveedor2);
    proveedor3 = crearProveedorConsole(3, "PharmaCorp", "Av. Providencia 123", "90321000-8");
    agregarProveedorConsole(farmacia, proveedor3);

    // Creacion de productos de prueba
    producto1 = crearProductoFalso("1", "Sertralina 100mg 30 Comp", "Antidepresivo en forma de Capsulas", "Antidepresivo", 6327, "Lab. de Chile", "N/A", "N/A", -1, 1);
    producto2 = crearProductoFalso("2", "Clonazepam 2 Mg 30 Comp", "Actua sobre el sistema nervioso central, con propiedades ansioliticas.", "Ansiolitico", 5007, "Lab. de Chile", "N/A", "N/A", -1, 1);
    producto3 = crearProductoFalso("3", "Tapsin Dia-Noche Plus 18 Comp", "Comprimidos Recubiertos rapido alivio de los sintomas de la gripe", "Antigripal", 2575, "Lab. Maver", "N/A", "N/A", -1, 0);
    producto4 = crearProductoFalso("4", "Tapsin Dia Limon 6 Un.", "Para el alivio sintomatico de las molestias del resfrio y la gripe.", "Antigripal", 2500, "Lab. Maver", "N/A", "N/A", -1, 0);
    producto5 = crearProductoFalso("5", "100 Whey Protein Cookies Cream", "Frasco de Proteina 857.5G", "Proteina", 58990, "PharmaCorp", "N/A", "N/A", -1, 0);
    producto6 = crearProductoFalso("6", "Sustenan Testosterona 250 mg", "Para aliviar la alergia", "Salud Hombre", 32500, "PharmaCorp", "N/A", "N/A", -1, 1);

    // Agregar productos a proveedores
    agregarProductoAProveedor(proveedor1->datosProveedor, producto1);
    agregarProductoAProveedor(proveedor1->datosProveedor, producto2);

    agregarProductoAProveedor(proveedor2->datosProveedor, producto3);
    agregarProductoAProveedor(proveedor2->datosProveedor, producto4);

    agregarProductoAProveedor(proveedor3->datosProveedor, producto5);
    agregarProductoAProveedor(proveedor3->datosProveedor, producto6);

    // Agregar productos de prueba a las sucursales
    agregarProductoASucursal(sucursal1->datosSucursal, prod1);
    agregarProductoASucursal(sucursal1->datosSucursal, prod2);
    agregarProductoASucursal(sucursal1->datosSucursal, prod3);
    agregarProductoASucursal(sucursal1->datosSucursal, prod4);

    agregarProductoASucursal(sucursal2->datosSucursal, prod4);
    agregarProductoASucursal(sucursal2->datosSucursal, prod5);
    agregarProductoASucursal(sucursal2->datosSucursal, prod6);

    agregarProductoASucursal(sucursal3->datosSucursal, prod1);
    agregarProductoASucursal(sucursal3->datosSucursal, prod2);
    agregarProductoASucursal(sucursal3->datosSucursal, prod3);
    agregarProductoASucursal(sucursal3->datosSucursal, prod4);
    agregarProductoASucursal(sucursal3->datosSucursal, prod5);
    agregarProductoASucursal(sucursal3->datosSucursal, prod6);

    cls();
    printf("\nDatos de prueba generados exitosamente.\n");
    pause();
    return;
}

void menuSucursal(struct FarmaSalud *farmacia) {
    int opcion;
    while (1) {
        cls();
        printf("\n------ Sucursal -----\n");
        printf("1. Leer Sucursales\n");
        printf("2. Crear Sucursal\n");
        printf("3. Eliminar Sucursal\n");
        printf("4. Actualizar Sucursales\n");
        printf("5. Agregar Producto a Sucursal\n");
        printf("6. Volver al Menu Principal\n");
        printf("---------------------\n");
        printf("\nSeleccione una opcion: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                leerSucursales(farmacia);
                break;
            case 2:
                crearSucursal(farmacia);
                break;
            case 3:
                eliminarSucursalView(farmacia);
                break;
            case 4:
                actualizarInventariosSucursales(farmacia);
                break;
            case 5:
                transferirProductoProveedorASucursal(farmacia);
                break;
            case 6:
                return; // Volver al menu principal
            default:
                printf("Opcion no valida. Intente nuevamente.\n");
                pause();
                break;
        }
    }
}

void menuCliente(struct FarmaSalud *farmacia) {
    int opcion;
    while (1) {
        cls();
        printf("\n------ Cliente -----\n");
        printf("1. Leer Cliente\n");
        printf("2. Crear Cliente\n");
        printf("3. Eliminar Cliente\n");
        printf("4. Volver al Menu Principal\n");
        printf("---------------------\n");
        printf("\nSeleccione una opcion: ");
        scanf("%d", &opcion);

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
                return; // Volver al menu principal
            default:
                printf("Opcion no valida. Intente nuevamente.\n");
                pause();
                break;
        }
    }
}

void menuProveedor(struct FarmaSalud *farmacia) {
    int opcion;
    while (1) {
        cls();
        printf("\n------ Proveedor -----\n");
        printf("1. Leer Proveedor\n");
        printf("2. Crear Proveedor\n");
        printf("3. Eliminar Proveedor\n");
        printf("4. Agregar Producto a Proveedor\n");
        printf("5. Transferir Productos de Proveedor a Sucursal\n");
        printf("6. Volver al Menu Principal\n");
        printf("----------------------\n");
        printf("\nSeleccione una opcion: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                leerProveedores(farmacia);
                break;
            case 2:
                crearProveedorView(farmacia);
                break;
            case 3:
                eliminarProveedor(farmacia);
                break;
            case 4:
                agregarProductoProveedor(farmacia);
                break;
            case 5:
                transferirProductosProveedorASucursal(farmacia);
                break;
            case 6:
                return; // Volver al menu principal
            default:
                printf("Opcion no valida. Intente nuevamente.\n");
                pause();
                break;
        }
    }
}

void menuInformes(struct FarmaSalud *farmacia) {
    int opcion;
    while (1) {
        cls();
        printf("\n------ Informes -----\n");
        printf("1. Ver Informes\n");
        printf("2. Calcular categoria mas vendida por Estacion del Año\n");
        printf("3. Mostrar Total de Sucursales y Ventas\n");
        printf("4. Mostrar Promedio de Ventas por Sucursal\n");
        printf("5. Mostrar Sucursal con Mas Ventas\n");
        printf("6. Mostrar Sucursal con Menos Ventas\n");
        printf("7. Volver al Menu Principal\n");
        printf("---------------------\n");
        printf("\nSeleccione una opcion: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                mostrarSucursalConMasVentas(farmacia);
                mostrarEstadisticas(farmacia);
                mostrarPorcentajeClientesAfiliados(farmacia);
                mostrarPorcentajeSucursalesConVentasBajoPromedio(farmacia);
                break;
            case 2:
                calcularMasVendidoPorEstacion(farmacia);
                break;
            case 3:
                mostrarTotalSucursalesVentas(farmacia);
                break;
            case 4:
                mostrarPromedioVentas(farmacia);
                break;
            case 5:
                mostrarSucursalConMasVentas(farmacia);
                break;
            case 6:
                mostrarSucursalMenosVentas(farmacia);
                break;
            case 7:
                return; // Volver al menu principal
            default:
                printf("Opcion no valida. Intente nuevamente.\n");
                pause();
                break;
        }
    }
}

void menuAdministrador(struct FarmaSalud *farmacia) {
    int opcion;
    while (1) {
        cls();
        printf("\n------ Administrador -----\n");
        printf("1. Realizar Compra\n");
        printf("2. Eliminar Productos Vencidos\n");
        printf("3. Generar Datos de Prueba\n");
        printf("4. Acceder Menu Informes\n");
        printf("5. Volver al Menu Principal\n");
        printf("--------------------------\n");
        printf("\nSeleccione una opcion: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                realizarCompra(farmacia);
                actualizarInventariosSucursales(farmacia);
                break;
            case 2:
                menuEliminarProductosVencidos(farmacia);
                break;
            case 3:
                generarDatosDePrueba(farmacia);
                break;
            case 4:
                menuInformes(farmacia);
                break;
            case 5:
                return; // Volver al menu principal
            default:
                printf("Opcion no valida. Intente nuevamente.\n");
                pause();
                break;
        }
    }
}

int menuPrincipal(struct FarmaSalud *farmacia) {
    int opcion;
    while (1) {
        cls();
        printf("\n  ----- FarmaSalud -----\n");
        printf("--------------------------\n");
        printf("1. Sucursal\n");
        printf("2. Cliente\n");
        printf("3. Proveedor\n");
        printf("4. Administrador\n");
        printf("5. Salida\n");
        printf("--------------------------\n");
        printf("\nSeleccione una opcion: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                menuSucursal(farmacia);
                break;
            case 2:
                menuCliente(farmacia);
                break;
            case 3:
                menuProveedor(farmacia);
                break;
            case 4:
                menuAdministrador(farmacia);
                break;
            case 5:
                printf("Saliendo...\n");
                return 0; // Salir del programa
            default:
                printf("Opcion no valida. Intente nuevamente.\n");
                pause();
                break;
        }
    }
}

int main() {
    struct FarmaSalud *farmacia;
    farmacia = (struct FarmaSalud *)malloc(sizeof(struct FarmaSalud));

    farmacia->clientes = NULL;
    farmacia->sucursales = NULL;
    farmacia->proveedores = NULL;

    // Ejecutar el menu principal una vez
    menuPrincipal(farmacia);

    return 0;
}
