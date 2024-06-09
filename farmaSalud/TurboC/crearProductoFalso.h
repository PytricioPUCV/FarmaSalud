struct Producto* crearProductoFalso(char* codigo, char* nombre, char* descripcion, char* categoria, int precio, char* idProveedor, char* lote, char* fechaCaducidad, int cantidad, int requiereReceta) {
    struct Producto* nuevoProducto = (struct Producto*)malloc(sizeof(struct Producto));
    if (nuevoProducto == NULL) {
        exit(1);
    }

    strcpy(nuevoProducto->codigo, codigo);
    nuevoProducto->nombreProducto = strdup(nombre);
    nuevoProducto->descripcion = strdup(descripcion);
    nuevoProducto->categoria = strdup(categoria);
    nuevoProducto->precio = precio;
    nuevoProducto->idProveedor = strdup(idProveedor);
    nuevoProducto->lote = strdup(lote);
    nuevoProducto->fechaCaducidad = strdup(fechaCaducidad);
    nuevoProducto->cantidad = cantidad;
    nuevoProducto->requiereReceta = requiereReceta;

    return nuevoProducto;
}