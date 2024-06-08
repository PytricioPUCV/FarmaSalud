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

    imprimirActualizacionInventario();
}