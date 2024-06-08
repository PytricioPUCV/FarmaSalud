void agregarProductoAProveedor(struct Proveedor* proveedor, struct Producto* producto) {
    struct NodoArbolProducto* nuevoNodo = (struct NodoArbolProducto*)malloc(sizeof(struct NodoArbolProducto));
    if (nuevoNodo == NULL) {
        exit(1);
    }
    nuevoNodo->datosProducto = producto;
    nuevoNodo->izq = nuevoNodo->der = NULL;

    if (proveedor->productos == NULL) {
        proveedor->productos = nuevoNodo;
    } else {
        struct NodoArbolProducto* actual = proveedor->productos;
        struct NodoArbolProducto* padre = NULL;

        while (actual != NULL) {
            padre = actual;
            if (strcmp(producto->codigo, actual->datosProducto->codigo) < 0) {
                actual = actual->izq;
            } else {
                actual = actual->der;
            }
        }

        if (strcmp(producto->codigo, padre->datosProducto->codigo) < 0) {
            padre->izq = nuevoNodo;
        } else {
            padre->der = nuevoNodo;
        }
    }
}