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
    printf("ID: %d\n", nodoActual->datosSucursal->id);
    printf("Nombre: %s\n", nodoActual->datosSucursal->nombre);
    printf("Dirección: %s\n", nodoActual->datosSucursal->direccion);
    printf("Cantidad de Ventas: %d\n", nodoActual->datosSucursal->cantidadDeVentas);
    printf("Número de Registros de Envíos: %d\n", nodoActual->datosSucursal->numRegistros);
    for (int i = 0; i < nodoActual->datosSucursal->numRegistros; i++) {
        printf("\tRegistro de Envío %d: %s\n", i + 1, nodoActual->datosSucursal->registrosEnvios[i]);
    }

    if (nodoActual->datosSucursal->productos != NULL) {
        printf("Productos:\n");
        struct NodoProducto *productoNodo = nodoActual->datosSucursal->productos;
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