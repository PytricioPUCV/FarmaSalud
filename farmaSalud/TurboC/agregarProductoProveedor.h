void quitarNuevaLinea(char *cadena) {
    char *pos;
    if ((pos = strchr(cadena, '\n')) != NULL) {
        *pos = '\0';
    }
}


void solicitarDatosProducto(struct Producto *producto) {
    cls();
    printf("Ingrese código del producto: ");
    scanf("%s", producto->codigo);
    getchar();
    cls();
    printf("Ingrese nombre del producto: ");
    fgets(producto->nombreProducto, 50, stdin);
    quitarNuevaLinea(producto->nombreProducto);
    cls();
    printf("Ingrese descripción del producto: ");
    fgets(producto->descripcion, 100, stdin);
    quitarNuevaLinea(producto->descripcion);
    cls();
    printf("Ingrese categoría del producto: ");
    fgets(producto->categoria, 50, stdin);
    quitarNuevaLinea(producto->categoria);
    cls();
    printf("Ingrese precio del producto: ");
    scanf("%d", &producto->precio);
    getchar();
    cls();
    printf("\nIngrese ID del proveedor: ");
    scanf("%s", producto->idProveedor);
    cls();
    printf("¿Requiere receta? (1-Sí, 0-No): ");
    scanf("%d", &producto->requiereReceta);
}

struct Producto* crearProducto() {
    struct Producto *nuevoProducto = (struct Producto *)malloc(sizeof(struct Producto));
    nuevoProducto->nombreProducto = (char *)malloc(50 * sizeof(char));
    nuevoProducto->descripcion = (char *)malloc(100 * sizeof(char));
    nuevoProducto->categoria = (char *)malloc(50 * sizeof(char));
    nuevoProducto->idProveedor = (char *)malloc(20 * sizeof(char));
    nuevoProducto->lote = strdup("N/A");
    nuevoProducto->fechaCaducidad = strdup("N/A");
    nuevoProducto->cantidad = -1;
    return nuevoProducto;
}

int agregarProductoProveedorModelo(struct FarmaSalud *farmacia, struct Producto *nuevoProducto) {
    int idProveedorInt = atoi(nuevoProducto->idProveedor);
    struct NodoProveedor *proveedorActual = farmacia->proveedores;
    while (proveedorActual != NULL && proveedorActual->datosProveedor->id != idProveedorInt) {
        proveedorActual = proveedorActual->sig;
    }

    if (proveedorActual == NULL) {
        return 0;
    }

    struct NodoArbolProducto *nuevoNodoProducto = (struct NodoArbolProducto *)malloc(sizeof(struct NodoArbolProducto));
    nuevoNodoProducto->datosProducto = nuevoProducto;
    nuevoNodoProducto->izq = nuevoNodoProducto->der = NULL;

    if (proveedorActual->datosProveedor->productos == NULL) {
        proveedorActual->datosProveedor->productos = nuevoNodoProducto;
    } else {
        struct NodoArbolProducto *actualProducto = proveedorActual->datosProveedor->productos;
        struct NodoArbolProducto *padreProducto = NULL;

        while (actualProducto != NULL) {
            padreProducto = actualProducto;
            if (strcmp(nuevoProducto->codigo, actualProducto->datosProducto->codigo) < 0) {
                actualProducto = actualProducto->izq;
            } else {
                actualProducto = actualProducto->der;
            }
        }

        if (strcmp(nuevoProducto->codigo, padreProducto->datosProducto->codigo) < 0) {
            padreProducto->izq = nuevoNodoProducto;
        } else {
            padreProducto->der = nuevoNodoProducto;
        }
    }
    return 1;
}

void agregarProductoProveedor(struct FarmaSalud *farmacia) {
    struct Producto *nuevoProducto = crearProducto();
    solicitarDatosProducto(nuevoProducto);
    mostrarProveedores(farmacia);
    int resultado = agregarProductoProveedorModelo(farmacia, nuevoProducto);
    if (resultado) {
        cls();
        printf("Producto agregado con éxito al proveedor.\n");
    } else {
        cls();
        printf("Proveedor no encontrado.\n");
        free(nuevoProducto->nombreProducto);
        free(nuevoProducto->descripcion);
        free(nuevoProducto->categoria);
        free(nuevoProducto->idProveedor);
        free(nuevoProducto->lote);
        free(nuevoProducto->fechaCaducidad);
        free(nuevoProducto);
    }
    pause();
}