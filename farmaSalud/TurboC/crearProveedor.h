void agregarProveedor(struct FarmaSalud *farmacia, struct NodoProveedor *nuevoProveedor) {
    if (farmacia->proveedores == NULL) {
        farmacia->proveedores = nuevoProveedor;
        nuevoProveedor->ant = nuevoProveedor->sig = NULL;
    } else {
        struct NodoProveedor *ultimo = farmacia->proveedores;
        while (ultimo->sig != NULL) {
            ultimo = ultimo->sig;
        }
        ultimo->sig = nuevoProveedor;
        nuevoProveedor->ant = ultimo;
        nuevoProveedor->sig = NULL;
    }
}

int existeProveedor(struct FarmaSalud *farmacia, int id) {
    struct NodoProveedor *actual = farmacia->proveedores;
    while (actual != NULL) {
        if (actual->datosProveedor->id == id) {
            return 1;  // Proveedor encontrado
        }
        actual = actual->sig;
    }
    return 0;  // Proveedor no encontrado
}


void leerDatosProveedor(int *id, char **nombre, char **direccion, char **telefono) {
    cls();
    printf("Ingrese ID del proveedor: ");
    scanf("%d", id);
    getchar();  // Consumir el salto de línea que queda en el buffer

    *nombre = (char *)malloc(50 * sizeof(char));
    if (*nombre == NULL) {
        printf("Error al asignar memoria para el nombre del proveedor.\n");
        return;
    }
    printf("Ingrese nombre del proveedor: ");
    fgets(*nombre, 50, stdin);
    (*nombre)[strcspn(*nombre, "\n")] = 0;  // Eliminar el salto de línea

    *direccion = (char *)malloc(100 * sizeof(char));
    if (*direccion == NULL) {
        printf("Error al asignar memoria para la dirección del proveedor.\n");
        free(*nombre);
        return;
    }
    printf("Ingrese dirección del proveedor: ");
    fgets(*direccion, 100, stdin);
    (*direccion)[strcspn(*direccion, "\n")] = 0;  // Eliminar el salto de línea

    *telefono = (char *)malloc(15 * sizeof(char));
    if (*telefono == NULL) {
        printf("Error al asignar memoria para el teléfono del proveedor.\n");
        free(*nombre);
        free(*direccion);
        return;
    }
    printf("Ingrese teléfono del proveedor: ");
    fgets(*telefono, 15, stdin);
    (*telefono)[strcspn(*telefono, "\n")] = 0;  // Eliminar el salto de línea
}

void mostrarMensajeProveedorAgregado() {
    cls();
    printf("Proveedor agregado con éxito.\n");
    pause();
}

void mostrarMensajeProveedorExiste() {
    cls();
    printf("Error: Ya existe un proveedor con el mismo ID.\n");
    pause();
}

void crearProveedorView(struct FarmaSalud *farmacia) {
    int id;
    char *nombre, *direccion, *telefono;

    leerDatosProveedor(&id, &nombre, &direccion, &telefono);

    if (nombre == NULL || direccion == NULL || telefono == NULL) {
        return;
    }

    if (existeProveedor(farmacia, id)) {
        mostrarMensajeProveedorExiste();
        free(nombre);
        free(direccion);
        free(telefono);
        return;
    }

    struct NodoProveedor *nuevoNodo = (struct NodoProveedor*)malloc(sizeof(struct NodoProveedor));
    if (!nuevoNodo) {
        printf("Error al asignar memoria para nuevo nodo proveedor.\n");
        free(nombre);
        free(direccion);
        free(telefono);
        return;
    }

    struct Proveedor *nuevoProveedor = (struct Proveedor*)malloc(sizeof(struct Proveedor));
    if (!nuevoProveedor) {
        free(nuevoNodo);
        printf("Error al asignar memoria para nuevo proveedor.\n");
        free(nombre);
        free(direccion);
        free(telefono);
        return;
    }

    nuevoProveedor->id = id;
    nuevoProveedor->nombre = nombre;
    nuevoProveedor->direccion = direccion;
    nuevoProveedor->telefono = telefono;
    nuevoProveedor->productos = NULL;

    nuevoNodo->datosProveedor = nuevoProveedor;
    nuevoNodo->ant = nuevoNodo->sig = NULL;

    agregarProveedor(farmacia, nuevoNodo);

    mostrarMensajeProveedorAgregado();
}
void mostrarProveedores(struct FarmaSalud *farmacia) {
    struct NodoProveedor *proveedorActual;
    proveedorActual = farmacia->proveedores;
    printf("Proveedores disponibles:\n");
    while (proveedorActual != NULL) {
        printf("ID: %d, Nombre: %s\n", proveedorActual->datosProveedor->id, proveedorActual->datosProveedor->nombre);
        proveedorActual = proveedorActual->sig;
    }
    printf("\n");
}