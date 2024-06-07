
/* 
To-do list:

* Crear Funcion Estadisticas (sucursal con mas items, proveedor con mas items, etc etc)
* Crear Funcion para quitar productos con vencimiento DE UNA SUCURSAL o DE TODAS

(PREGUNTAR PROFE) 4. Abastecimiento y relaciones con proveedores: FarmaSalud trabaja con múltiples proveedores y fabricantes de medicamentos. La 
aplicación debe facilitar la generación de órdenes de compra, el seguimiento de envíos y la actualización del inventario cuando se reciben 
nuevos productos.

(CORRECCION YO ) 5. Alertas de desabastecimiento: La aplicación debe generar alertas cuando el stock de un producto esté por debajo de un umbral 
predefinido, considerando el promedio de ventas y el tiempo de reabastecimiento.

(CORRECCION YO) 5. Generación de alertas de caducidad próxima y desabastecimiento.

7. Generación de órdenes de compra a proveedores basadas en niveles de stock y demanda. MANUALMANUALMANUAL

(FALTA) 8. Análisis de datos y generación de informes de ventas, productos más vendidos, tendencias estacionales, etc.

(LISTA) Quizas hay que actualizar  Struct Sucursal para que tenga un contador de ventas efectuadas, ojop con eso, porque puede
que existan productos que se le vendan a un cliente no registrado por ejemplo, pero igual hay que llevar la cuenta de las ventas

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
    char *nombreCliente;
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
    char *fechaCaducidad; // DD/MM/YYYY
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

#include "Utilidades.h"
#include "FuncionesDeClientes.h"
#include "AnalisisDeDatos.h"
#include "FuncionesDeSucursales.h"
#include "FuncionesDeProveedores.h"
#include "FuncionesDeTransferencia.h"
#include "realizarCompra.h"
#include "menuPrincipal.h"


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
    //agregarProductoAProveedor(proveedor1->datosProveedor, producto5);
    //agregarProductoAProveedor(proveedor1->datosProveedor, producto10);

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