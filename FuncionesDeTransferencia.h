/* - - - - MODIFICAR, USA COSAS RARAS - - - - -*/

void obtenerFecha(char *fecha) {
    int dia, mes;
    printf("Ingrese el mes (MM): ");
    scanf("%d", &mes);
    printf("Ingrese el dia (DD): ");
    scanf("%d", &dia);
    sprintf(fecha, "%02d/%02d", mes, dia);
}

void agregarRegistroEnvio(struct Sucursal *sucursal, const char *proveedorNombre) {
    if (sucursal->numRegistros >= MAX_ENVIOS) {
        printf("Error: No se puede agregar más registros de envíos, capacidad máxima alcanzada.\n");
        return;
    }

    char fecha[6];
    obtenerFecha(fecha);

    char registro[200];
    strcpy(registro, "Productos enviados de Proveedor ");
    strcat(registro, proveedorNombre);
    strcat(registro, " a Sucursal ");
    strcat(registro, sucursal->nombre);
    strcat(registro, " el día ");
    strcat(registro, fecha);

    sucursal->registrosEnvios[sucursal->numRegistros] = strdup(registro);
    sucursal->numRegistros++;
}

/*  - - - - - - -*/
void transferirProductosProveedorASucursal(struct FarmaSalud *farmacia) {
    int idProveedor, idSucursal;
    char fechaCaducidad[11];
    char lote[50];
    int cantidad;

    cls();

    mostrarProveedores(farmacia);
    printf("Ingrese el ID del proveedor: ");
    scanf("%d", &idProveedor);

    // Buscar el proveedor por ID
    struct NodoProveedor *proveedorActual = buscarProveedorPorID(farmacia, idProveedor);
    if (proveedorActual == NULL) {
        printf("Proveedor con ID %d no encontrado.\n", idProveedor);
        pause();
        return;
    }

    cls();
    mostrarSucursales(farmacia);
    printf("Ingrese el ID de la sucursal: ");
    scanf("%d", &idSucursal);

    // Buscar la sucursal por ID
    struct NodoSucursales *sucursalActual = buscarSucursalPorID(farmacia, idSucursal);
    if (sucursalActual == NULL) {
        cls();
        printf("Sucursal con ID %d no encontrada.\n", idSucursal);
        pause();
        return;
    }

    // Transferir productos del proveedor a la sucursal
    struct NodoArbolProducto *nodoProducto = proveedorActual->datosProveedor->productos;
    while (nodoProducto != NULL) {
        cls();
        printf("Ingrese la fecha de vencimiento para el producto (formato MM/AAAA) [%s]: ", nodoProducto->datosProducto->nombreProducto);
        scanf("%s", fechaCaducidad);
        cls();
        printf("Ingrese el lote para el producto [%s]: ", nodoProducto->datosProducto->nombreProducto);
        scanf("%s", lote);
        cls();
        printf("Ingrese la cantidad para el producto [%s]: ", nodoProducto->datosProducto->nombreProducto);
        scanf("%d", &cantidad);

        // Crear una copia completa del producto
        struct Producto *nuevoProducto = (struct Producto *)malloc(sizeof(struct Producto));
        if (nuevoProducto == NULL) {
            printf("Error al asignar memoria para el nuevo producto.\n");
            pause();
            return;
        }
        // Copiar datos del producto original al nuevo producto
        strcpy(nuevoProducto->codigo, nodoProducto->datosProducto->codigo);
        nuevoProducto->nombreProducto = strdup(nodoProducto->datosProducto->nombreProducto);
        nuevoProducto->descripcion = strdup(nodoProducto->datosProducto->descripcion);
        nuevoProducto->categoria = strdup(nodoProducto->datosProducto->categoria);
        nuevoProducto->precio = nodoProducto->datosProducto->precio;
        nuevoProducto->idProveedor = strdup(nodoProducto->datosProducto->idProveedor);
        nuevoProducto->lote = strdup(lote);  // Usar el valor ingresado por el usuario
        nuevoProducto->fechaCaducidad = strdup(fechaCaducidad);  // Usar el valor ingresado por el usuario
        nuevoProducto->cantidad = cantidad;  // Usar el valor ingresado por el usuario
        nuevoProducto->requiereReceta = nodoProducto->datosProducto->requiereReceta;

        // Crear un nuevo nodo producto
        struct NodoProducto *nuevoNodoProducto = (struct NodoProducto *)malloc(sizeof(struct NodoProducto));
        if (nuevoNodoProducto == NULL) {
            printf("Error al asignar memoria para el nuevo nodo de producto.\n");
            free(nuevoProducto);
            pause();
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

        // Mover al siguiente producto del proveedor
        nodoProducto = nodoProducto->der;
    }

     agregarRegistroEnvio(sucursalActual->datosSucursal, proveedorActual->datosProveedor->nombre);

    cls();
    printf("Productos transferidos del proveedor con ID %d a la sucursal con ID %d.\n", idProveedor, idSucursal);
    pause();
    cls();
}


void obtenerDetallesProducto(char* fechaCaducidad, char* lote, int* cantidad, char* nombreProducto) {
    printf("Ingrese la fecha de vencimiento para el producto (formato MM/AAAA) [%s]: ", nombreProducto);
    scanf("%s", fechaCaducidad);
    printf("Ingrese el lote para el producto [%s]: ", nombreProducto);
    scanf("%s", lote);
    printf("Ingrese la cantidad para el producto [%s]: ", nombreProducto);
    scanf("%d", cantidad);
}

void transferirProductoProveedorASucursal(struct FarmaSalud *farmacia) {
    int idProveedor, idSucursal;
    char fechaCaducidad[11];
    char lote[50];
    int cantidad;
    char codigoProducto[10];

    cls();

    mostrarProveedores(farmacia);
    printf("Ingrese el ID del proveedor: ");
    scanf("%d", &idProveedor);

    // Buscar el proveedor por ID
    struct NodoProveedor *proveedorActual = buscarProveedorPorID(farmacia, idProveedor);
    if (proveedorActual == NULL) {
        printf("Proveedor con ID %d no encontrado.\n", idProveedor);
        pause();
        return;
    }

    cls();
    mostrarSucursales(farmacia);
    printf("Ingrese el ID de la sucursal: ");
    scanf("%d", &idSucursal);

    // Buscar la sucursal por ID
    struct NodoSucursales *sucursalActual = buscarSucursalPorID(farmacia, idSucursal);
    if (sucursalActual == NULL) {
        cls();
        printf("Sucursal con ID %d no encontrada.\n", idSucursal);
        pause();
        return;
    }

    // Mostrar productos del proveedor y pedir el código del producto a transferir
    cls();
    printf("Productos del proveedor:\n");
    struct NodoArbolProducto *nodoProducto = proveedorActual->datosProveedor->productos;
    while (nodoProducto != NULL) {
        printf("Codigo: %s, Nombre: %s\n", nodoProducto->datosProducto->codigo, nodoProducto->datosProducto->nombreProducto);
        nodoProducto = nodoProducto->der;
    }
    printf("Ingrese el codigo del producto a transferir: ");
    scanf("%s", codigoProducto);

    // Buscar el producto por código
    nodoProducto = proveedorActual->datosProveedor->productos;
    struct Producto *productoSeleccionado = NULL;
    while (nodoProducto != NULL) {
        if (strcmp(nodoProducto->datosProducto->codigo, codigoProducto) == 0) {
            productoSeleccionado = nodoProducto->datosProducto;
            break;
        }
        nodoProducto = nodoProducto->der;
    }

    if (productoSeleccionado == NULL) {
        printf("Producto con codigo %s no encontrado.\n", codigoProducto);
        pause();
        return;
    }

    obtenerDetallesProducto(fechaCaducidad, lote, &cantidad, productoSeleccionado->nombreProducto);

    // Crear una copia completa del producto
    struct Producto *nuevoProducto = (struct Producto *)malloc(sizeof(struct Producto));
    if (nuevoProducto == NULL) {
        printf("Error al asignar memoria para el nuevo producto.\n");
        pause();
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
        pause();
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
    printf("Producto %s transferido del proveedor con ID %d a la sucursal con ID %d.\n", codigoProducto, idProveedor, idSucursal);
    pause();
    cls();
}