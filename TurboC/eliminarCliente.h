void eliminarCliente(struct FarmaSalud *farmacia) {
    cls();
    int idEliminar;
    struct NodoClientes *temp, *prev;

    // Pedir al usuario que ingrese el ID del cliente a eliminar
    printf("Ingrese el ID del cliente que desea eliminar: ");
    scanf("%d", &idEliminar);

    temp = farmacia->clientes;
    prev = NULL;

    // Buscar el cliente con el ID proporcionado
    while (temp != NULL && temp->datosClientes->id != idEliminar) {
        prev = temp;
        temp = temp->sig;
    }

    // Si no se encontró el cliente con el ID proporcionado
    if (temp == NULL) {
        cls();
        printf("Cliente no encontrado.\n");
        pause();
        return;
    }

    // Manejar los casos especiales para el primer y último nodo
    if (prev == NULL) {
        farmacia->clientes = temp->sig;
    } else {
        prev->sig = temp->sig;
    }
    if (temp->sig != NULL) {
        temp->sig->ant = prev;
    }

    // Liberar la memoria asociada al cliente
    free(temp->datosClientes->rutCliente);
    for (int i = 0; i < temp->datosClientes->numCompras; i++) {
        free(temp->datosClientes->comprasCliente[i]->nombreProducto);
        free(temp->datosClientes->comprasCliente[i]->descripcion);
        free(temp->datosClientes->comprasCliente[i]->categoria);
        free(temp->datosClientes->comprasCliente[i]->idProveedor);
        free(temp->datosClientes->comprasCliente[i]->lote);
        free(temp->datosClientes->comprasCliente[i]->fechaCaducidad);
        free(temp->datosClientes->comprasCliente[i]);
    }
    free(temp->datosClientes);
    free(temp);

    cls();
    printf("Cliente eliminado con éxito.\n");
    pause();
}