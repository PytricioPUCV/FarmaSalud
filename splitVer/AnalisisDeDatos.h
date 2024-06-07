int contarVentasProducto(struct NodoProducto *productosVendidos, char *codigoProducto) {
    int totalVentas = 0;
    struct NodoProducto *productoActual = productosVendidos->sig;

    while (productoActual != productosVendidos) {
        if (strcmp(productoActual->datosProducto->codigo, codigoProducto) == 0) {
            totalVentas += productoActual->datosProducto->cantidad;
        }
        productoActual = productoActual->sig;
    }
    return totalVentas;
}

struct Producto *productoMasVendido(struct FarmaSalud *farma) {
    if (!farma || !farma->sucursales) return NULL;
    struct Producto *productoMasVendido = NULL;
    int maxVentas = 0;
    struct NodoSucursales *sucursalActual = farma->sucursales->sig;
    while (sucursalActual != farma->sucursales) {
        if (sucursalActual && sucursalActual->datosSucursal && sucursalActual->datosSucursal->productosVendidos) {
            struct NodoProducto *productoActual = sucursalActual->datosSucursal->productosVendidos->sig;
            while (productoActual != sucursalActual->datosSucursal->productosVendidos) {
                if (productoActual && productoActual->datosProducto) {
                    struct Producto *producto = productoActual->datosProducto;
                    int ventasProducto = contarVentasProducto(sucursalActual->datosSucursal->productosVendidos,producto->codigo);
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
    if (!farmaSalud || !farmaSalud->sucursales) return NULL;
    struct NodoSucursales *nodoActual = farmaSalud->sucursales->sig; 
    struct Sucursal *sucursalConMasVentas = NULL;
    int maxVentas = -1;
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
    if (!farmaSalud || !farmaSalud->clientes) return NULL;
    struct NodoClientes *nodoActual = farmaSalud->clientes->sig;
    struct Clientes *clienteConMasCompras = NULL;
    int maxCompras = -1;
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
    if (!raiz) return 0;
    int contador = 0;
    struct NodoArbolProducto *pila[1000]; 
    int top = 0; 
    pila[top++] = raiz;
    while (top > 0) {
        struct NodoArbolProducto *nodoActual = pila[--top];
        contador++;
        if (nodoActual->izq) pila[top++] = nodoActual->izq;
        if (nodoActual->der) pila[top++] = nodoActual->der;
    }
    return contador;
}

struct Proveedor* proveedorConMasProductos(struct FarmaSalud *farmaSalud) {
    if (!farmaSalud || !farmaSalud->proveedores) return NULL;

    struct NodoProveedor *nodoActual = farmaSalud->proveedores->sig;
    struct Proveedor *proveedorConMasProductos = NULL;
    int maxProductos = -1;
    while (nodoActual && nodoActual != farmaSalud->proveedores) {
        if (nodoActual->datosProveedor && nodoActual->datosProveedor->productos) {
            int numProductos = contarProductos(nodoActual->datosProveedor->productos);
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
    if (!farmacia) {
        printf("La farmacia no está inicializada.\n");
        pause();
        return;
    }
    cls();

    struct Sucursal *sucursalMasVentas = sucursalConMasVentas(farmacia);
    struct Producto *productoMasVendidoResult = productoMasVendido(farmacia);
    struct Proveedor *proveedorMasProductos = proveedorConMasProductos(farmacia);
    struct Clientes *clienteMasCompras = clienteConMasCompras(farmacia);

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
