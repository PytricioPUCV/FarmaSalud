struct NodoClientes* buscarClientePorID(struct FarmaSalud *farmacia, int idCliente) {
    struct NodoClientes *clienteActual = farmacia->clientes;
    while (clienteActual != NULL && clienteActual->datosClientes->id != idCliente) {
        clienteActual = clienteActual->sig;
    }
    return clienteActual;
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

int calcularTotal(struct Producto *productos[], int cantidades[], int cantidadCompras) {
    int total = 0;
    for (int i = 0; i < cantidadCompras; i++) {
        total += productos[i]->precio * cantidades[i];
    }
    return total;
}

void realizarCompra(struct FarmaSalud *farmacia) {
    int idSucursal, idCliente, cantidadCompras, cantidadProducto, i, j;
    char codigoProducto[10];
    struct NodoSucursales *sucursal = NULL;
    struct NodoClientes *cliente = NULL;
    int esClienteRegistrado = 0;

    cls();
    mostrarSucursales(farmacia);

    printf("Ingrese ID de la sucursal: ");
    scanf("%d", &idSucursal);
    cls();
    sucursal = buscarSucursalPorID(farmacia, idSucursal);
    if (sucursal == NULL) {
        printf("Sucursal no encontrada.\n");
        pause();
        return;
    }

    printf("¿El cliente está registrado? (1-Sí, 0-No): ");
    scanf("%d", &esClienteRegistrado);

    if (esClienteRegistrado) {
        cls();
        printf("Ingrese el ID del cliente: ");
        scanf("%d", &idCliente);
        cls();
        cliente = buscarClientePorID(farmacia, idCliente);
        if (cliente == NULL) {
            printf("Cliente no encontrado.\n");
            pause();
            return;
        }
    }

    cls();
    printf("Ingrese la cantidad de productos a comprar: ");
    scanf("%d", &cantidadCompras);
    cls();

    struct Producto *productosComprados[cantidadCompras];
    int cantidades[cantidadCompras];
    for (i = 0; i < cantidadCompras; i++) {
        printf("Ingrese el código del producto %d: ", i + 1);
        scanf("%s", codigoProducto);
        printf("Ingrese la cantidad del producto %d: ", i + 1);
        scanf("%d", &cantidadProducto);
        cls();
        struct Producto *producto = buscarProductoEnSucursal(sucursal->datosSucursal, codigoProducto);
        if (producto == NULL) {
            printf("Producto no encontrado en la sucursal.\n");
            pause();
            return;
        }
        if (producto->cantidad < cantidadProducto) {
            printf("No hay suficiente stock del producto %s.\n", producto->nombreProducto);
            pause();
            return;
        }
        producto->cantidad -= cantidadProducto;
        productosComprados[i] = producto;
        cantidades[i] = cantidadProducto;

        // Actualizar productosVendidos en la sucursal
        struct Producto *productoVendido = buscarProductoEnVendidos(sucursal->datosSucursal->productosVendidos, codigoProducto);
        if (productoVendido == NULL) {
            productoVendido = crearProductoVendido(producto);
            agregarProductoAVendidos(sucursal->datosSucursal, productoVendido);
        } else {
            productoVendido->cantidad += cantidadProducto;
        }
    }

    if (esClienteRegistrado) {
        for (i = 0; i < cantidadCompras; i++) {
            for (j = 0; j < cantidades[i]; j++) {
                cliente->datosClientes->comprasCliente[cliente->datosClientes->numCompras] = productosComprados[i];
                cliente->datosClientes->numCompras++;
            }
        }
    }

    // Incrementar el contador de ventas de la sucursal
    sucursal->datosSucursal->cantidadDeVentas++;

    printf("\n=======================================\n");
    printf("                 FarmaSalud\n");
    printf("           La farmacia del pueblo\n");
    printf("               Boleta Física\n");
    printf("=======================================\n");
    printf("Sucursal: %s\n", sucursal->datosSucursal->nombre);
    if (esClienteRegistrado) {
        printf("Cliente: %s\n", cliente->datosClientes->rutCliente);
    } else {
        printf("Cliente: Compra anónima\n");
    }
    printf("---------------------------------------\n");
    printf("PRODUCTOS                CANTIDAD   PRECIO\n");
    printf("---------------------------------------\n");
    for (i = 0; i < cantidadCompras; i++) {
        printf("%-20s %5d    $%d\n", productosComprados[i]->nombreProducto, cantidades[i], productosComprados[i]->precio * cantidades[i]);
    }
    printf("---------------------------------------\n");
    printf("Total:                           $%d\n", calcularTotal(productosComprados, cantidades, cantidadCompras));
    printf("=======================================\n");

    pause();
    
}
