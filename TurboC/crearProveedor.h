void crearProveedor(struct FarmaSalud *farmacia) {
    cls();
    struct NodoProveedor *nuevoNodo = (struct NodoProveedor*)malloc(sizeof(struct NodoProveedor));
    struct Proveedor *nuevoProveedor = (struct Proveedor*)malloc(sizeof(struct Proveedor));

    nuevoNodo->datosProveedor = nuevoProveedor;
    nuevoNodo->ant = nuevoNodo->sig = NULL;

    printf("Ingrese ID del proveedor: ");
    scanf("%d", &nuevoProveedor->id);
    getchar();  // Consumir el salto de línea que queda en el buffer

    printf("Ingrese nombre del proveedor: ");
    nuevoProveedor->nombre = (char *)malloc(50 * sizeof(char));
    fgets(nuevoProveedor->nombre, 50, stdin);
    nuevoProveedor->nombre[strcspn(nuevoProveedor->nombre, "\n")] = 0;  // Eliminar el salto de línea

    printf("Ingrese dirección del proveedor: ");
    nuevoProveedor->direccion = (char *)malloc(100 * sizeof(char));
    fgets(nuevoProveedor->direccion, 100, stdin);
    nuevoProveedor->direccion[strcspn(nuevoProveedor->direccion, "\n")] = 0;  // Eliminar el salto de línea

    printf("Ingrese teléfono del proveedor: ");
    nuevoProveedor->telefono = (char *)malloc(15 * sizeof(char));
    fgets(nuevoProveedor->telefono, 15, stdin);
    nuevoProveedor->telefono[strcspn(nuevoProveedor->telefono, "\n")] = 0;  // Eliminar el salto de línea

    nuevoProveedor->productos = NULL;

    agregarProveedorConsole(farmacia, nuevoNodo);

    cls();
    printf("Proveedor agregado con éxito.\n");
    pause();
}

void mostrarProveedores(struct FarmaSalud *farmacia) {
    struct NodoProveedor *proveedorActual = farmacia->proveedores;
    printf("Proveedores disponibles:\n");
    while (proveedorActual != NULL) {
        printf("ID: %d, Nombre: %s\n", proveedorActual->datosProveedor->id, proveedorActual->datosProveedor->nombre);
        proveedorActual = proveedorActual->sig;
    }
    printf("\n");
}