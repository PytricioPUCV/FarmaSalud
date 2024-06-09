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