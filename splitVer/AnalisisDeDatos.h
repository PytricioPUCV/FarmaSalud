void sucursalConMasVentas(struct FarmaSalud *farmacia) {
    if (farmacia->sucursales == NULL) {
        cls();
        printf("No hay sucursales registradas.\n");
        pause();
        return;
    }

    struct NodoSucursales *actual = farmacia->sucursales;
    struct Sucursal *mayorSucursal = actual->datosSucursal;

    int totalSucursales = 0;
    int todasCero = 1; // Bandera para verificar si todas las ventas son cero

    do {
        if (actual->datosSucursal->cantidadDeVentas > 0) {
            todasCero = 0;
        }
        if (actual->datosSucursal->cantidadDeVentas > mayorSucursal->cantidadDeVentas) {
            mayorSucursal = actual->datosSucursal;
        }
        actual = actual->sig;
        totalSucursales++;
    } while (actual != farmacia->sucursales);

    if (todasCero) {
        cls();
        printf("No hay ventas en las sucursales.\n");
        pause();
    } else {
        cls();
        printf("La sucursal con más ventas es: %s con %d ventas.\n",
               mayorSucursal->nombre, mayorSucursal->cantidadDeVentas);
        pause();
    }
}
// FUNCIONES DE ESTADISTICA NO PROBADAS

// Función auxiliar para sumar la cantidad de productos en un árbol binario
int sumarCantidadProductos(struct NodoArbolProducto *nodo) {
    if (nodo == NULL) {
        return 0;
    }
    return nodo->datosProducto->cantidad + sumarCantidadProductos(nodo->izq) + sumarCantidadProductos(nodo->der);
}

// Función buscar el mas vendido en una lista de productos
void productosMasVendidosPorSucursal(struct FarmaSalud *farmacia) {
    struct NodoSucursales *sucursalActual = farmacia->sucursales;
    if (sucursalActual == NULL) {
        printf("No hay sucursales registradas.\n");
        return;
    }

    do {
        struct NodoProducto *productoActual = sucursalActual->datosSucursal->productosVendidos;
        if (productoActual == NULL) {
            printf("Sucursal %s no tiene productos vendidos.\n", sucursalActual->datosSucursal->nombre);
            sucursalActual = sucursalActual->sig;
            continue;
        }

        // Contador de ventas por producto
        struct {
            struct Producto *producto;
            int cantidadVendida;
        } contador[100];

        int contadorIndex = 0;

        while (productoActual != NULL) {
            int encontrado = 0;
            for (int i = 0; i < contadorIndex; i++) {
                if (strcmp(contador[i].producto->codigo, productoActual->datosProducto->codigo) == 0) {
                    contador[i].cantidadVendida += productoActual->datosProducto->cantidad;
                    encontrado = 1;
                    break;
                }
            }
            if (!encontrado) {
                contador[contadorIndex].producto = productoActual->datosProducto;
                contador[contadorIndex].cantidadVendida = productoActual->datosProducto->cantidad;
                contadorIndex++;
            }
            productoActual = productoActual->sig;
        }

        // Encontrar el producto más vendido
        struct Producto *productoMasVendido = NULL;
        int maxCantidadVendida = 0;
        for (int i = 0; i < contadorIndex; i++) {
            if (contador[i].cantidadVendida > maxCantidadVendida) {
                maxCantidadVendida = contador[i].cantidadVendida;
                productoMasVendido = contador[i].producto;
            }
        }

        if (productoMasVendido != NULL) {
            printf("Producto más vendido en sucursal %s: %s (Cantidad: %d)\n",
                   sucursalActual->datosSucursal->nombre, productoMasVendido->nombreProducto, maxCantidadVendida);
        } else {
            printf("No hay productos vendidos en sucursal %s.\n", sucursalActual->datosSucursal->nombre);
        }

        sucursalActual = sucursalActual->sig;
    } while (sucursalActual != farmacia->sucursales);
}

// Función buscar el mas vendido por mes en una lista de productos en todas las sucursales
void productosMasVendidosPorMes(struct FarmaSalud *farmacia, int mes) {
    struct NodoSucursales *sucursalActual = farmacia->sucursales;
    if (sucursalActual == NULL) {
        printf("No hay sucursales registradas.\n");
        return;
    }

    do {
        struct NodoProducto *productoActual = sucursalActual->datosSucursal->productosVendidos;
        if (productoActual == NULL) {
            printf("Sucursal %s no tiene productos vendidos.\n", sucursalActual->datosSucursal->nombre);
            sucursalActual = sucursalActual->sig;
            continue;
        }

        // Contador de ventas por producto
        struct {
            struct Producto *producto;
            int cantidadVendida;
        } contador[100];

        int contadorIndex = 0;

        while (productoActual != NULL) {
            int productoMes, productoAnio;
            sscanf(productoActual->datosProducto->fechaCaducidad, "%d/%d", &productoMes, &productoAnio);
            if (productoMes == mes) {
                int encontrado = 0;
                for (int i = 0; i < contadorIndex; i++) {
                    if (strcmp(contador[i].producto->codigo, productoActual->datosProducto->codigo) == 0) {
                        contador[i].cantidadVendida += productoActual->datosProducto->cantidad;
                        encontrado = 1;
                        break;
                    }
                }
                if (!encontrado) {
                    contador[contadorIndex].producto = productoActual->datosProducto;
                    contador[contadorIndex].cantidadVendida = productoActual->datosProducto->cantidad;
                    contadorIndex++;
                }
            }
            productoActual = productoActual->sig;
        }

        // Encontrar el producto más vendido en el mes
        struct Producto *productoMasVendido = NULL;
        int maxCantidadVendida = 0;
        for (int i = 0; i < contadorIndex; i++) {
            if (contador[i].cantidadVendida > maxCantidadVendida) {
                maxCantidadVendida = contador[i].cantidadVendida;
                productoMasVendido = contador[i].producto;
            }
        }

        if (productoMasVendido != NULL) {
            printf("Producto más vendido en sucursal %s en el mes %d: %s (Cantidad: %d)\n",
                   sucursalActual->datosSucursal->nombre, mes, productoMasVendido->nombreProducto, maxCantidadVendida);
        } else {
            printf("No hay productos vendidos en sucursal %s en el mes %d.\n", sucursalActual->datosSucursal->nombre, mes);
        }

        sucursalActual = sucursalActual->sig;
    } while (sucursalActual != farmacia->sucursales);
}

// Función buscar el proveedor que mas reabastece
// No seguro si en base a la estructura se puede
void proveedoresQueReabastecenMas(struct FarmaSalud *farmacia) {
    struct NodoProveedor *proveedorActual = farmacia->proveedores;
    if (proveedorActual == NULL) {
        printf("No hay proveedores registrados.\n");
        return;
    }

    // Contador de productos reabastecidos por proveedor
    struct {
        struct Proveedor *proveedor;
        int cantidadReabastecida;
    } contador[100];

    int contadorIndex = 0;

    do {
        struct NodoArbolProducto *productoActual = proveedorActual->datosProveedor->productos;
        if (productoActual != NULL) {
            // Aquí necesitaríamos recorrer el árbol binario de productos
            // Para simplificar, asumiremos que hay una función que recorre el árbol y suma las cantidades
            int cantidadReabastecida = sumarCantidadProductos(productoActual);
            contador[contadorIndex].proveedor = proveedorActual->datosProveedor;
            contador[contadorIndex].cantidadReabastecida = cantidadReabastecida;
            contadorIndex++;
        }
        proveedorActual = proveedorActual->sig;
    } while (proveedorActual != NULL);

    // Encontrar el proveedor que más reabastece
    struct Proveedor *proveedorMasReabastecedor = NULL;
    int maxCantidadReabastecida = 0;
    for (int i = 0; i < contadorIndex; i++) {
        if (contador[i].cantidadReabastecida > maxCantidadReabastecida) {
            maxCantidadReabastecida = contador[i].cantidadReabastecida;
            proveedorMasReabastecedor = contador[i].proveedor;
        }
    }

    if (proveedorMasReabastecedor != NULL) {
        printf("Proveedor que más reabastece: %s (Cantidad: %d)\n",
               proveedorMasReabastecedor->nombre, maxCantidadReabastecida);
    } else {
        printf("No hay reabastecimientos registrados.\n");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 1. Sucursal con más ventas
// Esta función es factible porque struct Sucursal tiene el campo cantidadDeVentas.
void sucursalConMasVentas2(struct FarmaSalud *farmacia) {
    struct NodoSucursales *sucursalActual = farmacia->sucursales;
    if (sucursalActual == NULL) {
        printf("No hay sucursales registradas.\n");
        return;
    }

    struct Sucursal *sucursalConMasVentas = NULL;
    int maxVentas = 0;

    do {
        if (sucursalActual->datosSucursal->cantidadDeVentas > maxVentas) {
            maxVentas = sucursalActual->datosSucursal->cantidadDeVentas;
            sucursalConMasVentas = sucursalActual->datosSucursal;
        }
        sucursalActual = sucursalActual->sig;
    } while (sucursalActual != farmacia->sucursales);

    if (sucursalConMasVentas != NULL) {
        printf("Sucursal con más ventas: %s (Ventas: %d)\n",
               sucursalConMasVentas->nombre, maxVentas);
    } else {
        printf("No hay ventas registradas.\n");
    }
}

// 2. Productos con mayor y menor stock
// Esta función es factible porque struct Sucursal tiene un puntero a una lista de struct NodoProducto, 
// y cada struct Producto tiene un campo cantidad.
void productosMayorYMenorStock(struct FarmaSalud *farmacia) {
    struct NodoSucursales *sucursalActual = farmacia->sucursales;
    if (sucursalActual == NULL) {
        printf("No hay sucursales registradas.\n");
        return;
    }

    do {
        struct NodoProducto *productoActual = sucursalActual->datosSucursal->productos;
        if (productoActual == NULL) {
            printf("Sucursal %s no tiene productos.\n", sucursalActual->datosSucursal->nombre);
            sucursalActual = sucursalActual->sig;
            continue;
        }

        struct Producto *productoMayorStock = NULL;
        struct Producto *productoMenorStock = NULL;
        int maxStock = 0;
        int minStock = INT_MAX;

        while (productoActual != NULL) {
            if (productoActual->datosProducto->cantidad > maxStock) {
                maxStock = productoActual->datosProducto->cantidad;
                productoMayorStock = productoActual->datosProducto;
            }
            if (productoActual->datosProducto->cantidad < minStock) {
                minStock = productoActual->datosProducto->cantidad;
                productoMenorStock = productoActual->datosProducto;
            }
            productoActual = productoActual->sig;
        }

        if (productoMayorStock != NULL && productoMenorStock != NULL) {
            printf("Sucursal %s:\n", sucursalActual->datosSucursal->nombre);
            printf("Producto con mayor stock: %s (Cantidad: %d)\n",
                   productoMayorStock->nombreProducto, maxStock);
            printf("Producto con menor stock: %s (Cantidad: %d)\n",
                   productoMenorStock->nombreProducto, minStock);
        } else {
            printf("No hay productos en sucursal %s.\n", sucursalActual->datosSucursal->nombre);
        }

        sucursalActual = sucursalActual->sig;
    } while (sucursalActual != farmacia->sucursales);
}

// 3. Clientes que más compran
// Esta función es factible porque struct Clientes tiene el campo numCompras.
void clientesQueMasCompran(struct FarmaSalud *farmacia) {
    struct NodoClientes *clienteActual = farmacia->clientes;
    if (clienteActual == NULL) {
        printf("No hay clientes registrados.\n");
        return;
    }

    struct Clientes *clienteMasCompras = NULL;
    int maxCompras = 0;

    do {
        if (clienteActual->datosClientes->numCompras > maxCompras) {
            maxCompras = clienteActual->datosClientes->numCompras;
            clienteMasCompras = clienteActual->datosClientes;
        }
        clienteActual = clienteActual->sig;
    } while (clienteActual != NULL);

    if (clienteMasCompras != NULL) {
        printf("Cliente que más compra: %s (Compras: %d)\n",
            clienteMasCompras->rutCliente, maxCompras);
    } else {
        printf("No hay compras registradas.\n");
    }
}

// 4. Ventas totales por categoría de producto
// Esta función es factible porque struct Producto tiene el campo categoria y cantidad.
void ventasTotalesPorCategoria(struct FarmaSalud *farmacia) {
    struct NodoSucursales *sucursalActual = farmacia->sucursales;
    if (sucursalActual == NULL) {
        printf("No hay sucursales registradas.\n");
        return;
    }

    struct {
        char *categoria;
        int totalVentas;
    } ventasPorCategoria[100];

    int categoriaIndex = 0;

    do {
        struct NodoProducto *productoActual = sucursalActual->datosSucursal->productosVendidos;
        while (productoActual != NULL) {
            int encontrado = 0;
            for (int i = 0; i < categoriaIndex; i++) {
                if (strcmp(ventasPorCategoria[i].categoria, productoActual->datosProducto->categoria) == 0) {
                    ventasPorCategoria[i].totalVentas += productoActual->datosProducto->cantidad;
                    encontrado = 1;
                    break;
                }
            }
            if (!encontrado) {
                ventasPorCategoria[categoriaIndex].categoria = productoActual->datosProducto->categoria;
                ventasPorCategoria[categoriaIndex].totalVentas = productoActual->datosProducto->cantidad;
                categoriaIndex++;
            }
            productoActual = productoActual->sig;
        }
        sucursalActual = sucursalActual->sig;
    } while (sucursalActual != farmacia->sucursales);

    printf("Ventas totales por categoría:\n");
    for (int i = 0; i < categoriaIndex; i++) {
        printf("Categoría: %s, Ventas: %d\n", ventasPorCategoria[i].categoria, ventasPorCategoria[i].totalVentas);
    }
}

