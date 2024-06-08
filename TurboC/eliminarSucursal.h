int eliminarSucursal(struct FarmaSalud *farmacia, int idEliminar){
    struct NodoSucursales *temp, *prev;

    temp = farmacia->sucursales;
    if (temp == NULL) {
        return 0; // No hay sucursales
    }

    struct NodoSucursales *inicio = temp;

    do {
        if (temp->datosSucursal->id == idEliminar) {
            // Manejar los casos especiales para el primer y último nodo
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

            // Liberar la memoria asociada a la sucursal
            free(temp->datosSucursal->nombre);
            free(temp->datosSucursal->direccion);
            for (int i = 0; i < temp->datosSucursal->numRegistros; i++) {
                free(temp->datosSucursal->registrosEnvios[i]);
            }
            free(temp->datosSucursal);
            free(temp);

            return 1; // Eliminación exitosa
        }
        temp = temp->sig;
    } while (temp != inicio);

    return 0; // No se encontró la sucursal
}

void leerIdSucursal(int *idEliminar) {
    cls();
    printf("Ingrese el ID de la sucursal que desea eliminar: ");
    scanf("%d", idEliminar);
    getchar(); // Limpiar el buffer de entrada después de leer un entero
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
    printf("Sucursal eliminada con éxito.\n");
    pause();
}

void eliminarSucursalView(struct FarmaSalud *farmacia) {
    int idEliminar;

    leerIdSucursal(&idEliminar);

    struct NodoSucursales *temp = farmacia->sucursales;
    if (temp == NULL) {
        mostrarMensajeNoSucursales();
        return;
    }

    int resultado = eliminarSucursal(farmacia, idEliminar);

    if (resultado == 0) {
        mostrarMensajeSucursalNoEncontrada();
    } else {
        mostrarMensajeSucursalEliminada();
    }
}