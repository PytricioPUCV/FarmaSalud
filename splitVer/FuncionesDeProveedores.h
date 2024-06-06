// 5. Funciones de proveedores
struct NodoProveedor* crearProveedorConsole(int id, char* nombre, char* direccion, char* telefono) {
    struct NodoProveedor* nuevoNodo = (struct NodoProveedor*)malloc(sizeof(struct NodoProveedor));
    struct Proveedor* nuevoProveedor = (struct Proveedor*)malloc(sizeof(struct Proveedor));

    nuevoProveedor->id = id;
    nuevoProveedor->nombre = strdup(nombre);
    nuevoProveedor->direccion = strdup(direccion);
    nuevoProveedor->telefono = strdup(telefono);
    nuevoProveedor->productos = NULL;

    nuevoNodo->datosProveedor = nuevoProveedor;
    nuevoNodo->ant = nuevoNodo->sig = NULL;

    return nuevoNodo;
}

void agregarProveedorConsole(struct FarmaSalud *farmacia, struct NodoProveedor *nuevoProveedor) {
    if (farmacia->proveedores == NULL) {
        farmacia->proveedores = nuevoProveedor;
    } else {
        struct NodoProveedor *temp = farmacia->proveedores;
        while (temp->sig != NULL) {
            temp = temp->sig;
        }
        temp->sig = nuevoProveedor;
        nuevoProveedor->ant = temp;
    }
}

void mostrarProductosProveedor(struct NodoArbolProducto *nodoProducto) {
    char *requiereReceta;

    if (nodoProducto == NULL) {
        return;
    }

    mostrarProductosProveedor(nodoProducto->izq);

    struct Producto *producto = nodoProducto->datosProducto;
    if (producto->requiereReceta) {
        requiereReceta = "Sí";
    } else {
        requiereReceta = "No";
    }

    printf("%-10s", producto->codigo);
    printf("%-40s", producto->nombreProducto);
    printf("$%-20d", producto->precio);
    printf("%-18s\n", requiereReceta);

    mostrarProductosProveedor(nodoProducto->der);
}

void leerProveedores(struct FarmaSalud *farmacia) {
    cls();
    struct NodoProveedor *nodoActual = farmacia->proveedores;
    if (nodoActual == NULL) {
        printf("No hay proveedores registrados.\n");
        pause();
        return;
    }


    while (nodoActual != NULL) {
        printf("ID: %d\n", nodoActual->datosProveedor->id);
        printf("Nombre: %s\n", nodoActual->datosProveedor->nombre);
        printf("Dirección: %s\n", nodoActual->datosProveedor->direccion);
        printf("Teléfono: %s\n", nodoActual->datosProveedor->telefono);

        // Mostrar productos del proveedor
        if (nodoActual->datosProveedor->productos == NULL) {
            printf("No hay productos asociados a este proveedor.\n\n");
        } else {
            printf("Productos:\n\n");
            printf("ID          Nombre                                    Precio                Requiere receta\n\n");
            mostrarProductosProveedor(nodoActual->datosProveedor->productos);
            printf("\n");
        }

        nodoActual = nodoActual->sig;
    }
    pause();
}

void crearProveedor(struct FarmaSalud *farmacia) {
    cls();
    struct NodoProveedor *nuevoNodo = (struct NodoProveedor*)malloc(sizeof(struct NodoProveedor));
    struct Proveedor *nuevoProveedor = (struct Proveedor*)malloc(sizeof(struct Proveedor));

    nuevoNodo->datosProveedor = nuevoProveedor;
    nuevoNodo->ant = nuevoNodo->sig = NULL;

    printf("Ingrese ID del proveedor: ");
    scanf("%d", &nuevoProveedor->id);
    getchar();  // Consumir el salto de línea que queda en el buffer

    printf("Ingrese nombre del proveedor: ");
    nuevoProveedor->nombre = (char *)malloc(50 * sizeof(char));
    fgets(nuevoProveedor->nombre, 50, stdin);
    nuevoProveedor->nombre[strcspn(nuevoProveedor->nombre, "\n")] = 0;  // Eliminar el salto de línea

    printf("Ingrese dirección del proveedor: ");
    nuevoProveedor->direccion = (char *)malloc(100 * sizeof(char));
    fgets(nuevoProveedor->direccion, 100, stdin);
    nuevoProveedor->direccion[strcspn(nuevoProveedor->direccion, "\n")] = 0;  // Eliminar el salto de línea

    printf("Ingrese teléfono del proveedor: ");
    nuevoProveedor->telefono = (char *)malloc(15 * sizeof(char));
    fgets(nuevoProveedor->telefono, 15, stdin);
    nuevoProveedor->telefono[strcspn(nuevoProveedor->telefono, "\n")] = 0;  // Eliminar el salto de línea

    nuevoProveedor->productos = NULL;

    agregarProveedorConsole(farmacia, nuevoNodo);

    cls();
    printf("Proveedor agregado con éxito.\n");
    pause();
}

void eliminarProveedor(struct FarmaSalud *farmacia) {
    cls();
    int idEliminar;
    struct NodoProveedor *temp, *prev;

    // Pedir al usuario que ingrese el ID del proveedor a eliminar
    printf("Ingrese el ID del proveedor que desea eliminar: ");
    scanf("%d", &idEliminar);

    temp = farmacia->proveedores;
    prev = NULL;

    // Buscar el proveedor con el ID proporcionado
    while (temp != NULL && temp->datosProveedor->id != idEliminar) {
        prev = temp;
        temp = temp->sig;
    }

    // Si no se encontró el proveedor con el ID proporcionado
    if (temp == NULL) {
        cls();
        printf("Proveedor no encontrado.\n");
        pause();
        return;
    }

    // Manejar los casos especiales para el primer y último nodo
    if (prev == NULL) {
        farmacia->proveedores = temp->sig;
    } else {
        prev->sig = temp->sig;
    }
    if (temp->sig != NULL) {
        temp->sig->ant = prev;
    }

    // Liberar la memoria asociada al proveedor
    free(temp->datosProveedor->nombre);
    free(temp->datosProveedor->direccion);
    free(temp->datosProveedor->telefono);
    free(temp->datosProveedor);
    free(temp);

    cls();
    printf("Proveedor eliminado con éxito.\n");
    pause();
}

void agregarProductoProveedor(struct FarmaSalud *farmacia) {
    cls();

    // Crear y llenar los datos del nuevo producto
    struct Producto *nuevoProducto = (struct Producto *)malloc(sizeof(struct Producto));
    printf("Ingrese código del producto: ");
    scanf("%s", nuevoProducto->codigo);
    getchar(); // Para consumir el '\n' que queda en el buffer después de scanf
    nuevoProducto->nombreProducto = (char *)malloc(50 * sizeof(char));
    cls();
    printf("Ingrese nombre del producto: ");
    fgets(nuevoProducto->nombreProducto, 50, stdin);
    quitarNuevaLinea(nuevoProducto->nombreProducto);
    nuevoProducto->descripcion = (char *)malloc(100 * sizeof(char));
    cls();
    printf("Ingrese descripción del producto: ");
    fgets(nuevoProducto->descripcion, 100, stdin);
    quitarNuevaLinea(nuevoProducto->descripcion);
    nuevoProducto->categoria = (char *)malloc(50 * sizeof(char));
    cls();
    printf("Ingrese categoría del producto: ");
    fgets(nuevoProducto->categoria, 50, stdin);
    quitarNuevaLinea(nuevoProducto->categoria);
    cls();
    printf("Ingrese precio del producto: ");
    scanf("%d", &nuevoProducto->precio);
    getchar(); // Para consumir el '\n' que queda en el buffer después de scanf

    // Mostrar todos los proveedores disponibles
    struct NodoProveedor *proveedorActual = farmacia->proveedores;
    cls();
    printf("Proveedores disponibles:\n");
    while (proveedorActual != NULL) {
        printf("ID: %d, Nombre: %s\n", proveedorActual->datosProveedor->id, proveedorActual->datosProveedor->nombre);
        proveedorActual = proveedorActual->sig;
    }

    // Solicitar el ID del proveedor durante la creación del producto
    nuevoProducto->idProveedor = (char *)malloc(20 * sizeof(char));
    printf("\nIngrese ID del proveedor: ");
    scanf("%s", nuevoProducto->idProveedor);

    cls();
    // Asignar "N/A" a lote y fechaCaducidad, no preguntar al usuario
    nuevoProducto->lote = strdup("N/A");
    nuevoProducto->fechaCaducidad = strdup("N/A");

    // No preguntar por cantidad, asignar -1 para indicar que es un producto del proveedor
    nuevoProducto->cantidad = -1;

    cls();
    printf("¿Requiere receta? (1-Sí, 0-No): ");
    scanf("%d", &nuevoProducto->requiereReceta);

    // Convertir idProveedor de char* a int
    int idProveedorInt = atoi(nuevoProducto->idProveedor);

    // Buscar el proveedor por ID
    proveedorActual = farmacia->proveedores;
    while (proveedorActual != NULL && proveedorActual->datosProveedor->id != idProveedorInt) {
        proveedorActual = proveedorActual->sig;
    }

    if (proveedorActual == NULL) {
        printf("Proveedor no encontrado.\n");
        // Liberar memoria asignada al nuevo producto si el proveedor no se encuentra
        free(nuevoProducto->nombreProducto);
        free(nuevoProducto->descripcion);
        free(nuevoProducto->categoria);
        free(nuevoProducto->idProveedor);
        free(nuevoProducto->lote);
        free(nuevoProducto->fechaCaducidad);
        free(nuevoProducto);
        pause();
        return;
    }

    // Crear un nuevo nodo para el producto
    struct NodoArbolProducto *nuevoNodoProducto = (struct NodoArbolProducto *)malloc(sizeof(struct NodoArbolProducto));
    nuevoNodoProducto->datosProducto = nuevoProducto;
    nuevoNodoProducto->izq = nuevoNodoProducto->der = NULL;

    // Insertar el nuevo nodo en el árbol de productos del proveedor
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

    cls();
    printf("Producto agregado con éxito al proveedor.\n");
    pause();
}

struct Producto* crearProductoFalso(char* codigo, char* nombre, char* descripcion, char* categoria, int precio, char* idProveedor, char* lote, char* fechaCaducidad, int cantidad, int requiereReceta) {
    struct Producto* nuevoProducto = (struct Producto*)malloc(sizeof(struct Producto));

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

// Nueva función para agregar producto a un proveedor específico
void agregarProductoAProveedor(struct Proveedor* proveedor, struct Producto* producto) {
    struct NodoArbolProducto* nuevoNodo = (struct NodoArbolProducto*)malloc(sizeof(struct NodoArbolProducto));
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

struct NodoProveedor* buscarProveedorPorID(struct FarmaSalud *farmacia, int idProveedor) {
    struct NodoProveedor *proveedorActual = farmacia->proveedores;
    while (proveedorActual != NULL && proveedorActual->datosProveedor->id != idProveedor) {
        proveedorActual = proveedorActual->sig;
    }
    return proveedorActual;
}

void mostrarProveedores(struct FarmaSalud *farmacia) {
    struct NodoProveedor *proveedorActual = farmacia->proveedores;
    printf("Proveedores disponibles:\n");
    while (proveedorActual != NULL) {
        printf("ID: %d, Nombre: %s\n", proveedorActual->datosProveedor->id, proveedorActual->datosProveedor->nombre);
        proveedorActual = proveedorActual->sig;
    }
    printf("\n");
}