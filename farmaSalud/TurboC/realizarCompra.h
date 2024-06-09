void printClientesRegistrados() {
    printf("Clientes registrados:\n");
    printf("ID\tNombre\t\tRUT\n");
    printf("-----------------------------------\n");
}

void printClienteInfo(int id, char* rutCliente) {
    printf("%d\t%s\t\n", id, rutCliente);
}

void printNoHayClientesRegistrados() {
    printf("No hay clientes registrados.\n");
}

void printEntradaInvalida() {
    printf("Entrada inválida.\n");
}

void printClienteRegistrado() {
    printf("El cliente está registrado (1-Sí, 0-No): ");
}

void printIngreseIDCliente() {
    printf("Ingrese el ID del cliente: ");
}

void printClienteNoEncontrado() {
    printf("Cliente no encontrado.\n");
}

void printIngreseCantidadProductos() {
    printf("Ingrese la cantidad de productos a comprar: ");
}

void printCantidadProductosInvalida() {
    printf("Cantidad de productos inválida.\n");
}

void printIngreseCodigoProducto(int index) {
    printf("Ingrese el código del producto %d: ", index + 1);
}

void printIngreseCantidadProducto(int index) {
    printf("Ingrese la cantidad del producto %d: ", index + 1);
}

void printProductoNoEncontrado() {
    printf("Producto no encontrado en la sucursal.\n");
}

void printStockInsuficiente(char* nombreProducto) {
    printf("No hay suficiente stock del producto %s.\n", nombreProducto);
}

void printProductoRequiereReceta(char* nombreProducto) {
    printf("El producto %s requiere receta. El cliente tiene la receta (1-Sí, 0-No): ", nombreProducto);
}

void printCompraSinReceta(char* nombreProducto) {
    printf("No se puede completar la compra del producto %s sin receta.\n", nombreProducto);
}

void printBoletaFisicaHeader() {
    printf("\n=======================================\n");
    printf("                 FarmaSalud\n");
    printf("           La farmacia del pueblo\n");
    printf("               Boleta Física\n");
    printf("=======================================\n");
}

void printSucursalNombre(char* nombre) {
    printf("Sucursal: %s\n", nombre);
}

void printClienteNombre(char* rutCliente) {
    printf("Cliente: %s\n", rutCliente);
}

void printCompraAnonima() {
    printf("Cliente: Compra anónima\n");
}

void printProductosHeader() {
    printf("---------------------------------------\n");
    printf("PRODUCTOS                CANTIDAD   PRECIO\n");
    printf("---------------------------------------\n");
}

void printProductoInfo(char* nombreProducto, int cantidad, int precio) {
    printf("%-20s %5d    $%d\n", nombreProducto, cantidad, precio);
}

void printTotal(int total) {
    printf("---------------------------------------\n");
    printf("Total:                           $%d\n", total);
    printf("=======================================\n");
}

struct NodoClientes* buscarClientePorID(struct FarmaSalud *farmacia, int idCliente) {
    struct NodoClientes *clienteActual = farmacia->clientes;
    while (clienteActual != NULL && clienteActual->datosClientes->id != idCliente) {
        clienteActual = clienteActual->sig;
    }
    return clienteActual;
}

struct Producto* crearProductoVendido(struct Producto *producto) {
    struct Producto *nuevoProducto = (struct Producto*)malloc(sizeof(struct Producto));
    strcpy(nuevoProducto->codigo, producto->codigo);
    nuevoProducto->nombreProducto = strdup(producto->nombreProducto);
    nuevoProducto->descripcion = strdup(producto->descripcion);
    nuevoProducto->categoria = strdup(producto->categoria);
    nuevoProducto->precio = producto->precio;
    nuevoProducto->idProveedor = strdup(producto->idProveedor);
    nuevoProducto->lote = strdup("N/A");
    nuevoProducto->fechaCaducidad = strdup("N/A");
    nuevoProducto->cantidad = 1;
    nuevoProducto->requiereReceta = producto->requiereReceta;
    return nuevoProducto;
}

struct Producto* buscarProductoEnSucursal(struct Sucursal *sucursal, char *codigoProducto) {
    struct NodoProducto *productoNodo = sucursal->productos;
    while (productoNodo != NULL) {
        if (strcmp(productoNodo->datosProducto->codigo, codigoProducto) == 0) {
            return productoNodo->datosProducto;
        }
        productoNodo = productoNodo->sig;
    }
    return NULL;
}

void agregarProductoAVendidos(struct Sucursal *sucursal, struct Producto *producto) {
    struct NodoProducto *nuevoNodo = (struct NodoProducto*)malloc(sizeof(struct NodoProducto));
    nuevoNodo->datosProducto = producto;
    nuevoNodo->sig = sucursal->productosVendidos;
    nuevoNodo->ant = NULL;
    if (sucursal->productosVendidos != NULL) {
        sucursal->productosVendidos->ant = nuevoNodo;
    }
    sucursal->productosVendidos = nuevoNodo;
}

struct Producto* buscarProductoEnVendidos(struct NodoProducto *productosVendidos, char *codigoProducto) {
    struct NodoProducto *productoNodo = productosVendidos;
    while (productoNodo != NULL) {
        if (strcmp(productoNodo->datosProducto->codigo, codigoProducto) == 0) {
            return productoNodo->datosProducto;
        }
        productoNodo = productoNodo->sig;
    }
    return NULL;
}

void mostrarClientes(struct FarmaSalud *farmacia) {
    struct NodoClientes *actual = farmacia->clientes;
    
    if (actual == NULL) {
        printNoHayClientesRegistrados();
        return;
    }

    printClientesRegistrados();
    
    while (actual != NULL) {
        printClienteInfo(actual->datosClientes->id, actual->datosClientes->rutCliente);
        actual = actual->sig;
    }
}

void realizarCompra(struct FarmaSalud *farmacia) {
    int idSucursal, idCliente, cantidadCompras, cantidadProducto, i, j;
    char codigoProducto[10];
    struct NodoSucursales *sucursal = NULL;
    struct NodoClientes *cliente = NULL;
    int esClienteRegistrado = 0;
    int tieneReceta = 0;

    cls();
    mostrarSucursales(farmacia);

    printIngreseIDSucursal();
    if (scanf("%d", &idSucursal) != 1) {
        printEntradaInvalida();
        pause();
        return;
    }
    cls();
    sucursal = buscarSucursalPorID(farmacia, idSucursal);
    if (sucursal == NULL) {
        printSucursalNoEncontrada();
        pause();
        return;
    }

    printClienteRegistrado();
    if (scanf("%d", &esClienteRegistrado) != 1) {
        printEntradaInvalida();
        pause();
        return;
    }

    if (esClienteRegistrado) {
        cls();
        mostrarClientes(farmacia);
        printIngreseIDCliente();
        if (scanf("%d", &idCliente) != 1) {
            printEntradaInvalida();
            pause();
            return;
        }
        cls();
        cliente = buscarClientePorID(farmacia, idCliente);
        if (cliente == NULL) {
            printClienteNoEncontrado();
            pause();
            return;
        }
    }

    cls();
    printIngreseCantidadProductos();
    if (scanf("%d", &cantidadCompras) != 1 || cantidadCompras <= 0) {
        printCantidadProductosInvalida();
        pause();
        return;
    }
    cls();

    struct Producto *productosComprados[cantidadCompras];
    int cantidades[cantidadCompras];
    int productosRequierenReceta[cantidadCompras];

    for (i = 0; i < cantidadCompras; i++) {
        printIngreseCodigoProducto(i);
        if (scanf("%s", codigoProducto) != 1) {
            printEntradaInvalida();
            pause();
            return;
        }

        printIngreseCantidadProducto(i);
        if (scanf("%d", &cantidadProducto) != 1 || cantidadProducto <= 0) {
            printCantidadProductosInvalida();
            pause();
            return;
        }
        cls();

        struct Producto *producto = buscarProductoEnSucursal(sucursal->datosSucursal, codigoProducto);
        if (producto == NULL) {
            printProductoNoEncontrado();
            pause();
            return;
        }

        if (producto->cantidad < cantidadProducto) {
            printStockInsuficiente(producto->nombreProducto);
            pause();
            return;
        }

        if (producto->requiereReceta) {
            printProductoRequiereReceta(producto->nombreProducto);
            if (scanf("%d", &tieneReceta) != 1 || (tieneReceta != 0 && tieneReceta != 1)) {
                printEntradaInvalida();
                pause();
                return;
            }

            if (!tieneReceta) {
                printCompraSinReceta(producto->nombreProducto);
                productosRequierenReceta[i] = 0;
            } else {
                productosRequierenReceta[i] = 1;
            }
        } else {
            productosRequierenReceta[i] = 1;
        }

        productosComprados[i] = producto;
        cantidades[i] = cantidadProducto;
    }

    for (i = 0; i < cantidadCompras; i++) {
        if (productosRequierenReceta[i]) {
            productosComprados[i]->cantidad -= cantidades[i];

            struct Producto *productoVendido = buscarProductoEnVendidos(sucursal->datosSucursal->productosVendidos, productosComprados[i]->codigo);
            if (productoVendido == NULL) {
                productoVendido = crearProductoVendido(productosComprados[i]);
                agregarProductoAVendidos(sucursal->datosSucursal, productoVendido);
            } else {
                productoVendido->cantidad += cantidades[i];
            }

            if (esClienteRegistrado) {
                for (j = 0; j < cantidades[i]; j++) {
                    cliente->datosClientes->comprasCliente[cliente->datosClientes->numCompras] = productosComprados[i];
                    cliente->datosClientes->numCompras++;
                }
            }
        }
    }

    sucursal->datosSucursal->cantidadDeVentas++;

    printBoletaFisicaHeader();
    printSucursalNombre(sucursal->datosSucursal->nombre);
    if (esClienteRegistrado) {
        printClienteNombre(cliente->datosClientes->rutCliente);
    } else {
        printCompraAnonima();
    }
    printProductosHeader();
    for (i = 0; i < cantidadCompras; i++) {
        if (productosRequierenReceta[i]) {
            printProductoInfo(productosComprados[i]->nombreProducto, cantidades[i], productosComprados[i]->precio * cantidades[i]);
        }
    }
    int total = 0;
    for (i = 0; i < cantidadCompras; i++) {
        if (productosRequierenReceta[i]) {
            total += productosComprados[i]->precio * cantidades[i];
        }
    }
    printTotal(total);

    pause();
}
