struct NodoProveedor* crearProveedorConsole(int id, char* nombre, char* direccion, char* telefono) {
    struct NodoProveedor* nuevoNodo = (struct NodoProveedor*)malloc(sizeof(struct NodoProveedor));
    struct Proveedor* nuevoProveedor = (struct Proveedor*)malloc(sizeof(struct Proveedor));

    if (nuevoNodo == NULL || nuevoProveedor == NULL) {
        exit(1);
    }

    nuevoProveedor->id = id;
    nuevoProveedor->nombre = my_strdup(nombre);
    nuevoProveedor->direccion = my_strdup(direccion);
    nuevoProveedor->telefono = my_strdup(telefono);
    nuevoProveedor->productos = NULL;

    nuevoNodo->datosProveedor = nuevoProveedor;
    nuevoNodo->ant = nuevoNodo->sig = NULL;

    return nuevoNodo;
}