struct NodoClientes* crearClienteConsole(int id, char* rutCliente, int edadCliente, int afiliado, struct Producto **comprasCliente, int numCompras) {
    struct NodoClientes* nuevoNodo;
    struct Clientes* nuevoCliente;
    char* rutClienteCopy;
    int i;

    nuevoNodo = (struct NodoClientes*)malloc(sizeof(struct NodoClientes));
    nuevoCliente = (struct Clientes*)malloc(sizeof(struct Clientes));

    nuevoCliente->id = id;

    // Implementación manual de strdup
    rutClienteCopy = (char*)malloc(strlen(rutCliente) + 1);
    strcpy(rutClienteCopy, rutCliente);
    nuevoCliente->rutCliente = rutClienteCopy;

    nuevoCliente->edadCliente = edadCliente;
    nuevoCliente->afiliado = afiliado;
    nuevoCliente->numCompras = numCompras;

    // Inicializar el array de compras con NULL
    for (i = 0; i < MAX_PRODUCTOS_POR_CLIENTE; i++) {
        nuevoCliente->comprasCliente[i] = NULL;
    }
    // Copiar los productos existentes
    for (i = 0; i < numCompras; i++) {
        nuevoCliente->comprasCliente[i] = comprasCliente[i];
    }

    nuevoNodo->datosClientes = nuevoCliente;
    nuevoNodo->ant = nuevoNodo->sig = NULL;

    return nuevoNodo;
}