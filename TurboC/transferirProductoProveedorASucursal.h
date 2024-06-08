void printPromptFechaVencimiento(char* nombreProducto) {
    printf("Ingrese la fecha de vencimiento para el producto (formato MM/AAAA) [%s]: ", nombreProducto);
}

void printPromptLote(char* nombreProducto) {
    printf("Ingrese el lote para el producto [%s]: ", nombreProducto);
}

void printPromptCantidad(char* nombreProducto) {
    printf("Ingrese la cantidad para el producto [%s]: ", nombreProducto);
}

void printSucursalNoEncontrada() {
    printf("Sucursal no encontrada.\n");
}

void printPromptProveedor(void) {
    printf("Ingrese el ID del proveedor: ");
}

void printProveedorNoEncontrado(int idProveedor) {
    printf("Proveedor con ID %d no encontrado.\n", idProveedor);
}

void printPromptSucursal(void) {
    printf("Ingrese el ID de la sucursal: ");
}

void printPromptCodigoProducto(void) {
    printf("Ingrese el codigo del producto a transferir: ");
}

void printProductoCodigoNoEncontrado(char* codigoProducto) {
    printf("Producto con codigo %s no encontrado.\n", codigoProducto);
}

void printTransferenciaExitosa(char* codigoProducto, int idProveedor, int idSucursal) {
    printf("Producto %s transferido del proveedor con ID %d a la sucursal con ID %d.\n", codigoProducto, idProveedor, idSucursal);
}

//void printCamion() {
//    printf("──────▄▌▐▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀​▀▀▀▀▀▀▌\n");
//    printf("───▄▄██▌█ beep beep\n");
//    printf("▄▄▄▌▐██▌█ FarmaSalud Truck \n");
//    printf("███████▌█▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄​▄▄▄▄▄▄▌\n");
//    printf("▀(@)▀▀▀▀▀▀▀(@)(@)▀▀▀▀▀▀▀▀▀▀▀▀▀​▀▀▀▀(@)▀\n");
//}

void obtenerDetallesProducto(char* fechaCaducidad, char* lote, int* cantidad, char* nombreProducto) {
    printPromptFechaVencimiento(nombreProducto);
    scanf("%s", fechaCaducidad);
    printPromptLote(nombreProducto);
    scanf("%s", lote);
    printPromptCantidad(nombreProducto);
    scanf("%d", cantidad);
}

int getch(void) {
    int ch;
    ch = getchar();
    return ch;
}

void mostrarYSeleccionarProveedor(struct FarmaSalud *farmacia, int *idProveedor, struct NodoProveedor **proveedorActual) {
    mostrarProveedores(farmacia);
    printPromptProveedor();
    scanf("%d", idProveedor);

    *proveedorActual = buscarProveedorPorID(farmacia, *idProveedor);
    if (*proveedorActual == NULL) {
        printProveedorNoEncontrado(*idProveedor);
        getch();
    }
}

void mostrarYSeleccionarSucursal(struct FarmaSalud *farmacia, int *idSucursal, struct NodoSucursales **sucursalActual) {
    mostrarSucursales(farmacia);
    printPromptSucursal();
    scanf("%d", idSucursal);

    *sucursalActual = buscarSucursalPorID(farmacia, *idSucursal);
    if (*sucursalActual == NULL) {
        printSucursalNoEncontrada(*idSucursal);
        getch();
    }
}

void mostrarYSeleccionarProducto(struct NodoProveedor *proveedorActual, char *codigoProducto, struct Producto **productoSeleccionado) {
    printf("Productos del proveedor:\n");
    struct NodoArbolProducto *nodoProducto = proveedorActual->datosProveedor->productos;
    while (nodoProducto != NULL) {
        printf("Codigo: %s, Nombre: %s\n", nodoProducto->datosProducto->codigo, nodoProducto->datosProducto->nombreProducto);
        nodoProducto = nodoProducto->der;
    }
    printPromptCodigoProducto();
    scanf("%s", codigoProducto);

    nodoProducto = proveedorActual->datosProveedor->productos;
    *productoSeleccionado = NULL;
    while (nodoProducto != NULL) {
        if (strcmp(nodoProducto->datosProducto->codigo, codigoProducto) == 0) {
            *productoSeleccionado = nodoProducto->datosProducto;
            break;
        }
        nodoProducto = nodoProducto->der;
    }

    if (*productoSeleccionado == NULL) {
        printProductoCodigoNoEncontrado(codigoProducto);
        getch();
    }
}

void transferirProductoProveedorASucursal(struct FarmaSalud *farmacia) 
{
    int idProveedor, idSucursal;
    char fechaCaducidad[11];
    char lote[50];
    int cantidad;
    char codigoProducto[10];

    cls(); // Limpia la pantalla

    struct NodoProveedor *proveedorActual = NULL;
    mostrarYSeleccionarProveedor(farmacia, &idProveedor, &proveedorActual);
    if (proveedorActual == NULL) {
        return;
    }

    cls();
    struct NodoSucursales *sucursalActual = NULL;
    mostrarYSeleccionarSucursal(farmacia, &idSucursal, &sucursalActual);
    if (sucursalActual == NULL) {
        return;
    }

    cls();
    struct Producto *productoSeleccionado = NULL;
    mostrarYSeleccionarProducto(proveedorActual, codigoProducto, &productoSeleccionado);
    if (productoSeleccionado == NULL) {
        return;
    }

    obtenerDetallesProducto(fechaCaducidad, lote, &cantidad, productoSeleccionado->nombreProducto);

    // Crear una copia completa del producto
    struct Producto *nuevoProducto = (struct Producto *)malloc(sizeof(struct Producto));
    if (nuevoProducto == NULL) {
        printf("Error al asignar memoria para el nuevo producto.\n");
        getch();
        return;
    }
    // Copiar datos del producto original al nuevo producto
    strcpy(nuevoProducto->codigo, productoSeleccionado->codigo);
    nuevoProducto->nombreProducto = strdup(productoSeleccionado->nombreProducto);
    nuevoProducto->descripcion = strdup(productoSeleccionado->descripcion);
    nuevoProducto->categoria = strdup(productoSeleccionado->categoria);
    nuevoProducto->precio = productoSeleccionado->precio;
    nuevoProducto->idProveedor = strdup(productoSeleccionado->idProveedor);
    nuevoProducto->lote = strdup(lote);  // Usar el valor ingresado por el usuario
    nuevoProducto->fechaCaducidad = strdup(fechaCaducidad);  // Usar el valor ingresado por el usuario
    nuevoProducto->cantidad = cantidad;  // Usar el valor ingresado por el usuario
    nuevoProducto->requiereReceta = productoSeleccionado->requiereReceta;

    // Crear un nuevo nodo producto
    struct NodoProducto *nuevoNodoProducto = (struct NodoProducto *)malloc(sizeof(struct NodoProducto));
    if (nuevoNodoProducto == NULL) {
        printf("Error al asignar memoria para el nuevo nodo de producto.\n");
        free(nuevoProducto);
        getch();
        return;
    }
    nuevoNodoProducto->datosProducto = nuevoProducto;
    nuevoNodoProducto->ant = nuevoNodoProducto->sig = NULL;

    // Insertar el nuevo nodo producto en la lista de productos de la sucursal
    if (sucursalActual->datosSucursal->productos == NULL) {
        sucursalActual->datosSucursal->productos = nuevoNodoProducto;
    } else {
        struct NodoProducto *temp = sucursalActual->datosSucursal->productos;
        while (temp->sig != NULL) {
            temp = temp->sig;
        }
        temp->sig = nuevoNodoProducto;
        nuevoNodoProducto->ant = temp;
    }

    cls();
    printTransferenciaExitosa(codigoProducto, idProveedor, idSucursal);
    //printCamion();
    getch();
    cls();
}