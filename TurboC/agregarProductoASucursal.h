struct NodoProducto* crearNodoProducto(struct Producto* producto) {
    struct NodoProducto* nuevoNodo = (struct NodoProducto*)malloc(sizeof(struct NodoProducto));
    nuevoNodo->datosProducto = producto;
    nuevoNodo->ant = nuevoNodo->sig = NULL;
    return nuevoNodo;
}

void agregarProductoASucursal(struct Sucursal* sucursal, struct Producto* producto) {
    if (sucursal == NULL || producto == NULL) {
        return;
    }

    // Crear nodo producto
    struct NodoProducto* nuevoNodo = crearNodoProducto(producto);

    // Insertar al principio de la lista de productos de la sucursal
    if (sucursal->productos == NULL) {
        sucursal->productos = nuevoNodo;
    } else {
        nuevoNodo->sig = sucursal->productos;
        sucursal->productos->ant = nuevoNodo;
        sucursal->productos = nuevoNodo;
    }
}
