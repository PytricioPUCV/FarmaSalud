// 3. Funciones de clientes
struct NodoClientes* crearClienteConsole(int id, char* rutCliente, int edadCliente, int afiliado, struct Producto **comprasCliente, int numCompras) {
    struct NodoClientes* nuevoNodo = (struct NodoClientes*)malloc(sizeof(struct NodoClientes));
    struct Clientes* nuevoCliente = (struct Clientes*)malloc(sizeof(struct Clientes));

    nuevoCliente->id = id;
    nuevoCliente->rutCliente = strdup(rutCliente);
    nuevoCliente->edadCliente = edadCliente;
    nuevoCliente->afiliado = afiliado;
    nuevoCliente->numCompras = numCompras;

    // Inicializar el array de compras con NULL
    for (int i = 0; i < MAX_PRODUCTOS_POR_CLIENTE; i++) {
        nuevoCliente->comprasCliente[i] = NULL;
    }
    // Copiar los productos existentes
    for (int i = 0; i < numCompras; i++) {
        nuevoCliente->comprasCliente[i] = comprasCliente[i];
    }

    nuevoNodo->datosClientes = nuevoCliente;
    nuevoNodo->ant = nuevoNodo->sig = NULL;

    return nuevoNodo;
}

void agregarClienteConsole(struct FarmaSalud *farmacia, struct NodoClientes *nuevoCliente) {
    if (farmacia->clientes == NULL) {
        farmacia->clientes = nuevoCliente;
    } else {
        struct NodoClientes *temp = farmacia->clientes;
        while (temp->sig != NULL) {
            temp = temp->sig;
        }
        temp->sig = nuevoCliente;
        nuevoCliente->ant = temp;
    }
}

void leerClientes(struct FarmaSalud *farmacia) {
    cls();
    struct NodoClientes *nodoActual = farmacia->clientes;
    while(nodoActual != NULL) {
        printf("ID: %d\n", nodoActual->datosClientes->id);
        printf("RUT: %s\n", nodoActual->datosClientes->rutCliente);
        printf("Edad: %d\n", nodoActual->datosClientes->edadCliente);
        if((nodoActual->datosClientes->afiliado)==1)
        {
            printf("Afiliado: Si\n");
        }
        else
        {
            printf("Afiliado: No\n");
        }
        
        // Verificar si hay compras del cliente
        if (nodoActual->datosClientes->numCompras > 0) {
            printf("Compras:\n");
            for (int i = 0; i < nodoActual->datosClientes->numCompras; i++) {
                struct Producto *producto = nodoActual->datosClientes->comprasCliente[i];
                if (producto != NULL) {
                    printf("\t%s - %s\n", producto->codigo, producto->nombreProducto);
                }
            }
            printf("\n");
        } else {
            printf("Cliente sin compras\n\n");
        }
        
        nodoActual = nodoActual->sig;
    }
    pause();
}

void agregarCliente(struct FarmaSalud *farmacia) 
{
    struct NodoClientes *nuevoNodo = (struct NodoClientes *)malloc(sizeof(struct NodoClientes));
    nuevoNodo->datosClientes = (struct Clientes *)malloc(sizeof(struct Clientes));
    
    cls();
    
    // Obtener y asignar automáticamente el ID basado en el último ID disponible
    int maxId = 0;
    struct NodoClientes *temp = farmacia->clientes;
    while (temp != NULL) {
        if (temp->datosClientes->id > maxId) {
            maxId = temp->datosClientes->id;
        }
        temp = temp->sig;
    }
    nuevoNodo->datosClientes->id = maxId + 1;
    
    printf("Ingrese RUT del cliente: ");
    nuevoNodo->datosClientes->rutCliente = (char *)malloc(20 * sizeof(char));
    scanf("%s", nuevoNodo->datosClientes->rutCliente);
    printf("Ingrese edad del cliente: ");
    scanf("%d", &nuevoNodo->datosClientes->edadCliente);
    printf("¿Es afiliado? (1-Si, 0-No): ");
    scanf("%d", &nuevoNodo->datosClientes->afiliado);
    
    // Inicializar comprasCliente como una lista vacía
    for (int i = 0; i < MAX_PRODUCTOS_POR_CLIENTE; i++) {
        nuevoNodo->datosClientes->comprasCliente[i] = NULL;
    }
    nuevoNodo->datosClientes->numCompras = 0;

    // Agregar cliente al final de la lista
    nuevoNodo->ant = NULL;
    nuevoNodo->sig = NULL;
    if (farmacia->clientes == NULL) {
        farmacia->clientes = nuevoNodo;
    } else {
        temp = farmacia->clientes;
        while (temp->sig != NULL) {
            temp = temp->sig;
        }
        temp->sig = nuevoNodo;
        nuevoNodo->ant = temp;
    }
    
    cls();
    printf("Cliente agregado con éxito.\n");
    pause();
}

void eliminarCliente(struct FarmaSalud *farmacia) {
    cls();
    int idEliminar;
    struct NodoClientes *temp, *prev;

    // Pedir al usuario que ingrese el ID del cliente a eliminar
    printf("Ingrese el ID del cliente que desea eliminar: ");
    scanf("%d", &idEliminar);

    temp = farmacia->clientes;
    prev = NULL;

    // Buscar el cliente con el ID proporcionado
    while (temp != NULL && temp->datosClientes->id != idEliminar) {
        prev = temp;
        temp = temp->sig;
    }

    // Si no se encontró el cliente con el ID proporcionado
    if (temp == NULL) {
        cls();
        printf("Cliente no encontrado.\n");
        pause();
        return;
    }

    // Manejar los casos especiales para el primer y último nodo
    if (prev == NULL) {
        farmacia->clientes = temp->sig;
    } else {
        prev->sig = temp->sig;
    }
    if (temp->sig != NULL) {
        temp->sig->ant = prev;
    }

    // Liberar la memoria asociada al cliente
    free(temp->datosClientes->rutCliente);
    for (int i = 0; i < temp->datosClientes->numCompras; i++) {
        free(temp->datosClientes->comprasCliente[i]->nombreProducto);
        free(temp->datosClientes->comprasCliente[i]->descripcion);
        free(temp->datosClientes->comprasCliente[i]->categoria);
        free(temp->datosClientes->comprasCliente[i]->idProveedor);
        free(temp->datosClientes->comprasCliente[i]->lote);
        free(temp->datosClientes->comprasCliente[i]->fechaCaducidad);
        free(temp->datosClientes->comprasCliente[i]);
    }
    free(temp->datosClientes);
    free(temp);

    cls();
    printf("Cliente eliminado con éxito.\n");
    pause();
}

void agregarProductoACliente(struct FarmaSalud *farmacia) {
    cls();
    int idCliente;
    printf("Ingrese el ID del cliente al que desea agregar un producto: ");
    scanf("%d", &idCliente);

    struct NodoClientes *clienteActual = farmacia->clientes;
    while (clienteActual != NULL && clienteActual->datosClientes->id != idCliente) {
        clienteActual = clienteActual->sig;
    }

    if (clienteActual == NULL) {
        printf("Cliente no encontrado.\n");
        pause();
        return;
    }

    if (clienteActual->datosClientes->numCompras >= MAX_PRODUCTOS_POR_CLIENTE) {
        printf("El cliente ha alcanzado el máximo número de productos permitidos.\n");
        pause();
        return;
    }

    struct Producto *nuevoProducto = (struct Producto *)malloc(sizeof(struct Producto));
    printf("Ingrese código del producto: ");
    scanf("%s", nuevoProducto->codigo);
    nuevoProducto->nombreProducto = (char *)malloc(50 * sizeof(char));
    printf("Ingrese nombre del producto: ");
    scanf("%s", nuevoProducto->nombreProducto);
    nuevoProducto->descripcion = (char *)malloc(100 * sizeof(char));
    printf("Ingrese descripción del producto: ");
    scanf("%s", nuevoProducto->descripcion);
    nuevoProducto->categoria = (char *)malloc(50 * sizeof(char));
    printf("Ingrese categoría del producto: ");
    scanf("%s", nuevoProducto->categoria);
    printf("Ingrese precio del producto: ");
    scanf("%d", &nuevoProducto->precio);
    nuevoProducto->idProveedor = (char *)malloc(50 * sizeof(char));
    printf("Ingrese nombre del proveedor: ");
    scanf("%s", nuevoProducto->idProveedor);
    nuevoProducto->lote = (char *)malloc(20 * sizeof(char));
    printf("Ingrese lote del producto: ");
    scanf("%s", nuevoProducto->lote);
    nuevoProducto->fechaCaducidad = (char *)malloc(15 * sizeof(char));
    printf("Ingrese fecha de caducidad del producto (YYYY-MM-DD): ");
    scanf("%s", nuevoProducto->fechaCaducidad);
    printf("Ingrese cantidad del producto: ");
    scanf("%d", &nuevoProducto->cantidad);
    printf("¿Requiere receta? (1-Sí, 0-No): ");
    scanf("%d", &nuevoProducto->requiereReceta);

    // Agregar el nuevo producto al array de compras del cliente
    clienteActual->datosClientes->comprasCliente[clienteActual->datosClientes->numCompras] = nuevoProducto;
    clienteActual->datosClientes->numCompras++;

    cls();
    printf("Producto agregado con éxito al cliente.\n");
    pause();
}