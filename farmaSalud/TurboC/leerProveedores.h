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