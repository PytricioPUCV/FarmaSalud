void printIngresarMes() {
    printf("Ingrese el mes (MM): ");
}

void printIngresarDia() {
    printf("Ingrese el dia (DD): ");
}

void printSucursalNoEncontrada() {
    printf("Sucursal no encontrada.\n");
}

void printErrorCapacidadMaxima() {
    printf("Error: No se puede agregar más registros de envíos, capacidad máxima alcanzada.\n");
}

void printRegistroEnvio(const char *registro) {
    printf("%s\n", registro);
}

void printNoSucursalesRegistradas() {
    printf("No hay sucursales registradas.\n");
}

void printSucursalInfo(struct Sucursal *sucursal) {
    printf("ID: %d\n", sucursal->id);
    printf("Nombre: %s\n", sucursal->nombre);
    printf("Dirección: %s\n", sucursal->direccion);
    printf("Cantidad de Ventas: %d\n", sucursal->cantidadDeVentas);
    printf("Capacidad de Almacenamiento: %d / 10000\n", sucursal->capacidadAlmacenamiento);
    printf("Número de Registros de Envíos: %d\n", sucursal->numRegistros);
    for (int i = 0; i < sucursal->numRegistros; i++) {
        printf("\tRegistro de Envío %d: %s\n", i + 1, sucursal->registrosEnvios[i]);
    }
    printf("--------------------------------------------------\n");
}

void printIngreseIDProveedor() {
    printf("Ingrese el ID del proveedor: ");
}

void printProveedorNoEncontrado(int idProveedor) {
    printf("Proveedor con ID %d no encontrado.\n", idProveedor);
}

void printIngreseIDSucursal() {
    printf("Ingrese el ID de la sucursal: ");
}

void printIngreseFechaCaducidad(const char *nombreProducto) {
    printf("Ingrese la fecha de vencimiento para el producto (formato MM/AAAA) [%s]: ", nombreProducto);
}

void printIngreseLote(const char *nombreProducto) {
    printf("Ingrese el lote para el producto [%s]: ", nombreProducto);
}

void printIngreseCantidad(const char *nombreProducto) {
    printf("Ingrese la cantidad para el producto [%s]: ", nombreProducto);
}

void printErrorAsignarMemoriaProducto() {
    printf("Error al asignar memoria para el nuevo producto.\n");
}

void printErrorAsignarMemoriaNodoProducto() {
    printf("Error al asignar memoria para el nuevo nodo de producto.\n");
}

void printProductosTransferidos(int idProveedor, int idSucursal) {
    printf("Productos transferidos del proveedor con ID %d a la sucursal con ID %d.\n", idProveedor, idSucursal);
}

// Funciones principales
void obtenerFecha(char *fecha) {
    int dia, mes;
    printIngresarMes();
    scanf("%d", &mes);
    printIngresarDia();
    scanf("%d", &dia);
    sprintf(fecha, "%02d/%02d", mes, dia);
}

void my_strcat(char *dest, const char *src) {
    while (*dest) {
        dest++;
    }
    while (*src) {
        *dest = *src;
        dest++;
        src++;
    }
    *dest = '\0';
}

void agregarRegistroEnvio(struct Sucursal *sucursal, const char *proveedorNombre) {
    if (sucursal->numRegistros >= MAX_ENVIOS) {
        printErrorCapacidadMaxima();
        return;
    }

    char fecha[6];
    obtenerFecha(fecha);

    char registro[200];
    strcpy(registro, "Productos enviados de Proveedor ");
    my_strcat(registro, proveedorNombre);
    my_strcat(registro, " a Sucursal ");
    my_strcat(registro, sucursal->nombre);
    my_strcat(registro, " el día ");
    my_strcat(registro, fecha);

    sucursal->registrosEnvios[sucursal->numRegistros] = (char *)malloc(strlen(registro) + 1);
    if (sucursal->registrosEnvios[sucursal->numRegistros] != NULL) {
        strcpy(sucursal->registrosEnvios[sucursal->numRegistros], registro);
        sucursal->numRegistros++;
    }
}

struct NodoSucursales* buscarSucursalPorID(struct FarmaSalud *farmacia, int idSucursal) {
    struct NodoSucursales *sucursalActual = farmacia->sucursales;
    if (sucursalActual == NULL) {
        return NULL;
    }

    struct NodoSucursales *inicio = sucursalActual;
    do {
        if (sucursalActual->datosSucursal->id == idSucursal) {
            return sucursalActual;
        }
        sucursalActual = sucursalActual->sig;
    } while (sucursalActual != inicio);

    return NULL;
}

struct NodoProveedor* buscarProveedorPorID(struct FarmaSalud *farmacia, int idProveedor) {
    struct NodoProveedor *proveedorActual = farmacia->proveedores;
    while (proveedorActual != NULL && proveedorActual->datosProveedor->id != idProveedor) {
        proveedorActual = proveedorActual->sig;
    }
    return proveedorActual;
}

void mostrarSucursales(struct FarmaSalud *farmacia) {
    struct NodoSucursales *sucursalActual = farmacia->sucursales;
    if (sucursalActual == NULL) {
        printNoSucursalesRegistradas();
        return;
    }

    do {
        printSucursalInfo(sucursalActual->datosSucursal);
        sucursalActual = sucursalActual->sig;
    } while (sucursalActual != farmacia->sucursales);
}

void transferirProductosProveedorASucursal(struct FarmaSalud *farmacia) {
    int idProveedor, idSucursal;
    char fechaCaducidad[11];
    char lote[50];
    int cantidad;

    cls();

    mostrarProveedores(farmacia);
    printIngreseIDProveedor();
    scanf("%d", &idProveedor);

    struct NodoProveedor *proveedorActual = buscarProveedorPorID(farmacia, idProveedor);
    if (proveedorActual == NULL) {
        printProveedorNoEncontrado(idProveedor);
        pause();
        return;
    }

    cls();
    mostrarSucursales(farmacia);
    printIngreseIDSucursal();
    scanf("%d", &idSucursal);

    struct NodoSucursales *sucursalActual = buscarSucursalPorID(farmacia, idSucursal);
    if (sucursalActual == NULL) {
        cls();
        printSucursalNoEncontrada();
        pause();
        return;
    }

    struct NodoArbolProducto *nodoProducto = proveedorActual->datosProveedor->productos;
    while (nodoProducto != NULL) {
        cls();
        printIngreseFechaCaducidad(nodoProducto->datosProducto->nombreProducto);
        scanf("%s", fechaCaducidad);
        cls();
        printIngreseLote(nodoProducto->datosProducto->nombreProducto);
        scanf("%s", lote);
        cls();
        printIngreseCantidad(nodoProducto->datosProducto->nombreProducto);
        scanf("%d", &cantidad);

        struct Producto *nuevoProducto = (struct Producto *)malloc(sizeof(struct Producto));
        if (nuevoProducto == NULL) {
            printErrorAsignarMemoriaProducto();
            pause();
            return;
        }

        strcpy(nuevoProducto->codigo, nodoProducto->datosProducto->codigo);

        // Asignar memoria para las cadenas y copiar
        nuevoProducto->nombreProducto = (char *)malloc(strlen(nodoProducto->datosProducto->nombreProducto) + 1);
        strcpy(nuevoProducto->nombreProducto, nodoProducto->datosProducto->nombreProducto);

        nuevoProducto->descripcion = (char *)malloc(strlen(nodoProducto->datosProducto->descripcion) + 1);
        strcpy(nuevoProducto->descripcion, nodoProducto->datosProducto->descripcion);

        nuevoProducto->categoria = (char *)malloc(strlen(nodoProducto->datosProducto->categoria) + 1);
        strcpy(nuevoProducto->categoria, nodoProducto->datosProducto->categoria);

        nuevoProducto->idProveedor = (char *)malloc(strlen(nodoProducto->datosProducto->idProveedor) + 1);
        strcpy(nuevoProducto->idProveedor, nodoProducto->datosProducto->idProveedor);

        nuevoProducto->lote = (char *)malloc(strlen(lote) + 1);
        strcpy(nuevoProducto->lote, lote);

        nuevoProducto->fechaCaducidad = (char *)malloc(strlen(fechaCaducidad) + 1);
        strcpy(nuevoProducto->fechaCaducidad, fechaCaducidad);

        nuevoProducto->precio = nodoProducto->datosProducto->precio;
        nuevoProducto->cantidad = cantidad;
        nuevoProducto->requiereReceta = nodoProducto->datosProducto->requiereReceta;

        struct NodoProducto *nuevoNodoProducto = (struct NodoProducto *)malloc(sizeof(struct NodoProducto));
        if (nuevoNodoProducto == NULL) {
            printErrorAsignarMemoriaNodoProducto();
            free(nuevoProducto);
            pause();
            return;
        }
        nuevoNodoProducto->datosProducto = nuevoProducto;
        nuevoNodoProducto->ant = nuevoNodoProducto->sig = NULL;

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

        nodoProducto = nodoProducto->der;
    }

    agregarRegistroEnvio(sucursalActual->datosSucursal, proveedorActual->datosProveedor->nombre);

    cls();
    printProductosTransferidos(idProveedor, idSucursal);
    pause();
    cls();
}