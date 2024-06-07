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

void mostrarClientes(struct FarmaSalud *farmacia) {
    struct NodoClientes *actual = farmacia->clientes;
    
    if (actual == NULL) {
        printf("No hay clientes registrados.\n");
        return;
    }

    printf("Clientes registrados:\n");
    printf("ID\tNombre\t\tRUT\n");
    printf("-----------------------------------\n");
    
    while (actual != NULL) {
        printf("%d\t%s\t\n", actual->datosClientes->id, actual->datosClientes->rutCliente);
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

    printf("Ingrese ID de la sucursal: ");
    if (scanf("%d", &idSucursal) != 1) {
        printf("Entrada inválida.\n");
        pause();
        return;
    }
    cls();
    sucursal = buscarSucursalPorID(farmacia, idSucursal);
    if (sucursal == NULL) {
        printf("Sucursal no encontrada.\n");
        pause();
        return;
    }

    printf("El cliente está registrado (1-Sí, 0-No): ");
    if (scanf("%d", &esClienteRegistrado) != 1) {
        printf("Entrada inválida.\n");
        pause();
        return;
    }

    if (esClienteRegistrado) {
        cls();
        mostrarClientes(farmacia);
        printf("Ingrese el ID del cliente: ");
        if (scanf("%d", &idCliente) != 1) {
            printf("Entrada inválida.\n");
            pause();
            return;
        }
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
    if (scanf("%d", &cantidadCompras) != 1 || cantidadCompras <= 0) {
        printf("Cantidad de productos inválida.\n");
        pause();
        return;
    }
    cls();

    struct Producto *productosComprados[cantidadCompras];
    int cantidades[cantidadCompras];
    int productosRequierenReceta[cantidadCompras];

    for (i = 0; i < cantidadCompras; i++) {
        printf("Ingrese el código del producto %d: ", i + 1);
        if (scanf("%s", codigoProducto) != 1) {
            printf("Entrada inválida.\n");
            pause();
            return;
        }

        printf("Ingrese la cantidad del producto %d: ", i + 1);
        if (scanf("%d", &cantidadProducto) != 1 || cantidadProducto <= 0) {
            printf("Cantidad de producto inválida.\n");
            pause();
            return;
        }
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

        if (producto->requiereReceta) {
            printf("El producto %s requiere receta. El cliente tiene la receta (1-Sí, 0-No): ", producto->nombreProducto);
            if (scanf("%d", &tieneReceta) != 1 || (tieneReceta != 0 && tieneReceta != 1)) {
                printf("Entrada inválida.\n");
                pause();
                return;
            }

            if (!tieneReceta) {
                printf("No se puede completar la compra del producto %s sin receta.\n", producto->nombreProducto);
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
        if (productosRequierenReceta[i]) {
            printf("%-20s %5d    $%d\n", productosComprados[i]->nombreProducto, cantidades[i], productosComprados[i]->precio * cantidades[i]);
        }
    }
    printf("---------------------------------------\n");
    int total = 0;
    for (i = 0; i < cantidadCompras; i++) {
        if (productosRequierenReceta[i]) {
            total += productosComprados[i]->precio * cantidades[i];
        }
    }
    printf("Total:                           $%d\n", total);
    printf("=======================================\n");

    pause();
}
