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