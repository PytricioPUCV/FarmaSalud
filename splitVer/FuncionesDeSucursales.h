void actualizarInventariosSucursales(struct FarmaSalud *farmacia) {
    if (farmacia == NULL || farmacia->sucursales == NULL) {
        printf("No hay sucursales registradas.\n");
        return;
    }

    struct NodoSucursales *sucursalActual = farmacia->sucursales;

    // Iterar sobre cada sucursal
    do {
        if (sucursalActual->datosSucursal == NULL || sucursalActual->datosSucursal->productos == NULL) {
            sucursalActual = sucursalActual->sig;
            continue;
        }

        struct NodoProducto *productoActual = sucursalActual->datosSucursal->productos; // Iniciar en el primer nodo (sin nodo fantasma)
        while (productoActual != NULL) {
            struct NodoProducto *siguienteProducto = productoActual->sig;
            if (productoActual->datosProducto != NULL && productoActual->datosProducto->cantidad == 0) {
                // Eliminar producto con cantidad 0
                if (productoActual->ant != NULL) {
                    productoActual->ant->sig = productoActual->sig;
                } else {
                    // Si es el primer producto, actualizamos el puntero inicial de productos
                    sucursalActual->datosSucursal->productos = productoActual->sig;
                }

                if (productoActual->sig != NULL) {
                    productoActual->sig->ant = productoActual->ant;
                }

                // Liberar memoria del producto
                free(productoActual->datosProducto->nombreProducto);
                free(productoActual->datosProducto->descripcion);
                free(productoActual->datosProducto->categoria);
                free(productoActual->datosProducto->idProveedor);
                free(productoActual->datosProducto->lote);
                free(productoActual->datosProducto->fechaCaducidad);
                free(productoActual->datosProducto);
                free(productoActual);
            }
            productoActual = siguienteProducto;
        }
        sucursalActual = sucursalActual->sig;
    } while (sucursalActual != farmacia->sucursales);

    printf("Inventarios actualizados. Productos con stock 0 eliminados.\n");
}


int espacioRestante(struct Sucursal *sucursal) {
    if (sucursal == NULL) {
        return -1;
    }

    int totalProductos = 0;
    struct NodoProducto *nodoActual = sucursal->productos;

    while (nodoActual != NULL) {
        totalProductos += nodoActual->datosProducto->cantidad;
        nodoActual = nodoActual->sig;
    }
    int espacioRestante = 10000 - totalProductos;
    return espacioRestante;
}

void leerSucursales(struct FarmaSalud *farmacia) {
    int espacio;
    struct NodoSucursales *nodoActual = farmacia->sucursales;
    cls();
    if (nodoActual == NULL) {
        printf("No hay sucursales registradas.\n");
        pause();
        return;
    }
    do {
        printf("ID: %d\n", nodoActual->datosSucursal->id);
        printf("Nombre: %s\n", nodoActual->datosSucursal->nombre);
        printf("Dirección: %s\n", nodoActual->datosSucursal->direccion);
        printf("Cantidad de Ventas: %d\n", nodoActual->datosSucursal->cantidadDeVentas);
        printf("Número de Registros de Envíos: %d\n", nodoActual->datosSucursal->numRegistros);
        for (int i = 0; i < nodoActual->datosSucursal->numRegistros; i++) {
            printf("\tRegistro de Envío %d: %s\n", i + 1, nodoActual->datosSucursal->registrosEnvios[i]);
        }

        if (nodoActual->datosSucursal->productos != NULL) {
            printf("Productos:\n");
            struct NodoProducto *productoNodo = nodoActual->datosSucursal->productos;
            while (productoNodo != NULL) {
                printf("\t%s - %s", productoNodo->datosProducto->codigo, productoNodo->datosProducto->nombreProducto);
                printf("\tCantidad en stock: %d", productoNodo->datosProducto->cantidad);
                printf("\tFecha de caducidad: %s", productoNodo->datosProducto->fechaCaducidad);
                printf("\tLote: %s\n", productoNodo->datosProducto->lote);

                // Comprobar si el producto tiene bajo stock
                if (productoNodo->datosProducto->cantidad < 10) {
                    printf("\t*** ALERTA: Bajo stock! ***\n");
                }

                productoNodo = productoNodo->sig;
            }
            printf("\n");
        } else {
            printf("Sucursal sin productos\n");
        }
        
        espacio = espacioRestante(nodoActual->datosSucursal);
        printf("Espacio restante en la sucursal: %d\n\n", espacio);
        nodoActual = nodoActual->sig;
    } while (nodoActual != farmacia->sucursales);
    pause();
}


struct NodoSucursales* crearSucursalConsole(int id, char* nombre, char* direccion, int capacidadAlmacenamiento) {
    struct NodoSucursales* nuevoNodo = (struct NodoSucursales*)malloc(sizeof(struct NodoSucursales));
    struct Sucursal* nuevaSucursal = (struct Sucursal*)malloc(sizeof(struct Sucursal));
    
    nuevaSucursal->id = id;
    nuevaSucursal->nombre = strdup(nombre);
    nuevaSucursal->direccion = strdup(direccion);
    nuevaSucursal->cantidadDeVentas = 0; // Inicializar a 0
    nuevaSucursal->capacidadAlmacenamiento = capacidadAlmacenamiento;
    nuevaSucursal->productos = NULL;
    nuevaSucursal->productosVendidos = NULL; // Inicializar a NULL
    nuevaSucursal->numRegistros = 0; // Inicializar a 0
    for (int i = 0; i < MAX_ENVIOS; i++) {
        nuevaSucursal->registrosEnvios[i] = NULL; // Inicializar a NULL
    }

    nuevoNodo->datosSucursal = nuevaSucursal;
    nuevoNodo->ant = nuevoNodo->sig = nuevoNodo;
    
    return nuevoNodo;
}


void agregarSucursalConsole(struct FarmaSalud *farmacia, struct NodoSucursales *nuevaSucursal) {
    if (farmacia->sucursales == NULL) {
        farmacia->sucursales = nuevaSucursal;
    } else {
        struct NodoSucursales *temp = farmacia->sucursales;
        while (temp->sig != farmacia->sucursales) {
            temp = temp->sig;
        }
        temp->sig = nuevaSucursal;
        nuevaSucursal->ant = temp;
        nuevaSucursal->sig = farmacia->sucursales;
        farmacia->sucursales->ant = nuevaSucursal;
    }
}

struct NodoProducto* crearNodoProducto(struct Producto* producto) {
    struct NodoProducto* nuevoNodo = (struct NodoProducto*)malloc(sizeof(struct NodoProducto));
    nuevoNodo->datosProducto = producto;
    nuevoNodo->ant = nuevoNodo->sig = NULL;
    return nuevoNodo;
}

void agregarProductoASucursal(struct Sucursal* sucursal, struct Producto* producto) {
    if (sucursal == NULL || producto == NULL) {
        printf("Sucursal o producto no válido.\n");
        return;
    }

    // Crear nodo producto
    struct NodoProducto* nuevoNodo = crearNodoProducto(producto);

    // Insertar al principio de la lista de productos de la sucursal
    if (sucursal->productos == NULL) {
        sucursal->productos = nuevoNodo;
    } else {
        nuevoNodo->sig = sucursal->productos;
        sucursal->productos->ant = nuevoNodo;
        sucursal->productos = nuevoNodo;
    }
}

void crearSucursal(struct FarmaSalud *farmacia) {
    cls();
    struct NodoSucursales *nuevoNodo = (struct NodoSucursales*)malloc(sizeof(struct NodoSucursales));
    struct Sucursal *nuevaSucursal = (struct Sucursal*)malloc(sizeof(struct Sucursal));

    nuevoNodo->datosSucursal = nuevaSucursal;
    nuevoNodo->ant = nuevoNodo->sig = NULL;

    printf("Ingrese ID de la sucursal: ");
    scanf("%d", &nuevaSucursal->id);
    printf("Ingrese nombre de la sucursal: ");
    nuevaSucursal->nombre = (char *)malloc(50 * sizeof(char));
    scanf("%s", nuevaSucursal->nombre);
    printf("Ingrese dirección de la sucursal: ");
    nuevaSucursal->direccion = (char *)malloc(100 * sizeof(char));
    scanf("%s", nuevaSucursal->direccion);

    nuevaSucursal->cantidadDeVentas = 0; // Inicializar a 0
    nuevaSucursal->capacidadAlmacenamiento = 0; // Cambiar si se necesita un valor específico
    nuevaSucursal->productos = NULL;
    nuevaSucursal->productosVendidos = NULL; // Inicializar a NULL
    nuevaSucursal->numRegistros = 0; // Inicializar a 0
    for (int i = 0; i < MAX_ENVIOS; i++) {
        nuevaSucursal->registrosEnvios[i] = NULL; // Inicializar a NULL
    }

    agregarSucursalConsole(farmacia, nuevoNodo);

    cls();
    printf("Sucursal agregada con éxito.\n");
    pause();
}


void eliminarSucursal(struct FarmaSalud *farmacia) {
    cls();
    int idEliminar;
    struct NodoSucursales *temp, *prev;

    // Pedir al usuario que ingrese el ID de la sucursal a eliminar
    printf("Ingrese el ID de la sucursal que desea eliminar: ");
    scanf("%d", &idEliminar);

    temp = farmacia->sucursales;
    if (temp == NULL) {
        printf("No hay sucursales registradas.\n");
        pause();
        return;
    }

    // Buscar la sucursal con el ID proporcionado
    do {
        if (temp->datosSucursal->id == idEliminar) {
            break;
        }
        temp = temp->sig;
    } while (temp != farmacia->sucursales);

    // Si no se encontró la sucursal con el ID proporcionado
    if (temp->datosSucursal->id != idEliminar) {
        cls();
        printf("Sucursal no encontrada.\n");
        pause();
        return;
    }

    // Manejar los casos especiales para el primer y último nodo
    if (temp->ant == temp && temp->sig == temp) {
        farmacia->sucursales = NULL;
    } else {
        prev = temp->ant;
        prev->sig = temp->sig;
        temp->sig->ant = prev;
        if (temp == farmacia->sucursales) {
            farmacia->sucursales = temp->sig;
        }
    }

    // Liberar la memoria asociada a la sucursal
    free(temp->datosSucursal->nombre);
    free(temp->datosSucursal->direccion);
    for (int i = 0; i < temp->datosSucursal->numRegistros; i++) {
        free(temp->datosSucursal->registrosEnvios[i]);
    }
    free(temp->datosSucursal);
    free(temp);

    cls();
    printf("Sucursal eliminada con éxito.\n");
    pause();
}

void mostrarSucursales(struct FarmaSalud *farmacia) {
    struct NodoSucursales *sucursalActual = farmacia->sucursales;
    if (sucursalActual == NULL) {
        printf("No hay sucursales registradas.\n");
        return;
    }

    do {
        printf("ID: %d\n", sucursalActual->datosSucursal->id);
        printf("Nombre: %s\n", sucursalActual->datosSucursal->nombre);
        printf("Dirección: %s\n", sucursalActual->datosSucursal->direccion);
        printf("Cantidad de Ventas: %d\n", sucursalActual->datosSucursal->cantidadDeVentas);
        printf("Capacidad de Almacenamiento: %d / 10000\n", sucursalActual->datosSucursal->capacidadAlmacenamiento);
        printf("Número de Registros de Envíos: %d\n", sucursalActual->datosSucursal->numRegistros);
        for (int i = 0; i < sucursalActual->datosSucursal->numRegistros; i++) {
            printf("\tRegistro de Envío %d: %s\n", i + 1, sucursalActual->datosSucursal->registrosEnvios[i]);
        }
        printf("--------------------------------------------------\n");
        sucursalActual = sucursalActual->sig;
    } while (sucursalActual != farmacia->sucursales);
}

// CODIGO NO AGREGADO AL GITHUB
int compararFechasVencidas(char *fecha1, char *fecha2) {
    int mes1, anio1, mes2, anio2;

    sscanf(fecha1, "%d/%d", &mes1, &anio1);
    sscanf(fecha2, "%d/%d", &mes2, &anio2);

    if (anio1 < anio2) return -1;
    if (anio1 > anio2) return 1;
    if (mes1 < mes2) return -1;
    if (mes1 > mes2) return 1;
    return 0;
}

int validarFecha(const char *fecha) {
    int mes, anio;
    if (sscanf(fecha, "%d/%d", &mes, &anio) != 2) {
        return 0;
    }
    if (mes < 1 || mes > 12) {
        return 0;
    }
    return 1;
}

void eliminarProductosVencidos(struct Sucursal *sucursal, char *fechaLimite) {
    if (sucursal == NULL || sucursal->productos == NULL) {
        printf("Sucursal o productos no válidos.\n");
        return;
    }
    if (!validarFecha(fechaLimite)) {
        printf("Fecha de caducidad inválida.\n");
        return;
    }

    struct NodoProducto *productoActual = sucursal->productos;
    while (productoActual != NULL) {
        struct NodoProducto *siguienteProducto = productoActual->sig;
        if (compararFechasVencidas(productoActual->datosProducto->fechaCaducidad, fechaLimite) < 0) {
            if (productoActual->ant != NULL) {
                productoActual->ant->sig = productoActual->sig;
            } else {
                sucursal->productos = productoActual->sig;
            }

            if (productoActual->sig != NULL) {
                productoActual->sig->ant = productoActual->ant;
            }

            free(productoActual->datosProducto->nombreProducto);
            free(productoActual->datosProducto->descripcion);
            free(productoActual->datosProducto->categoria);
            free(productoActual->datosProducto->idProveedor);
            free(productoActual->datosProducto->lote);
            free(productoActual->datosProducto->fechaCaducidad);
            free(productoActual->datosProducto);
            free(productoActual);
        }
        productoActual = siguienteProducto;
    }
}

void eliminarProductosVencidosDeTodasLasSucursales(struct FarmaSalud *farmacia, char *fechaLimite) {
    if (farmacia == NULL || farmacia->sucursales == NULL) {
        printf("No hay sucursales registradas.\n");
        return;
    }

    if (!validarFecha(fechaLimite)) {
        printf("Fecha de caducidad inválida.\n");
        return;
    }

    struct NodoSucursales *sucursalActual = farmacia->sucursales;
    do {
        eliminarProductosVencidos(sucursalActual->datosSucursal, fechaLimite);
        sucursalActual = sucursalActual->sig;
    } while (sucursalActual != farmacia->sucursales);

    printf("Productos vencidos eliminados de todas las sucursales correctamente.\n");
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

void menuEliminarProductosVencidos(struct FarmaSalud *farmacia) {
    cls();
    int idSucursal;
    char fechaLimite[8];

    mostrarSucursales(farmacia);
    printf("Ingrese el ID de la sucursal (o -1 para todas las sucursales): ");
    scanf("%d", &idSucursal);

    do {
        printf("Ingrese la fecha de vencimiento (MM/AAAA): ");
        scanf("%s", fechaLimite);

        if (!validarFecha(fechaLimite)) {
            printf("Fecha de caducidad invalida. Por favor, ingrese una fecha válida.\n");
        }
    } while (!validarFecha(fechaLimite));

    if (idSucursal == -1) {
        eliminarProductosVencidosDeTodasLasSucursales(farmacia, fechaLimite);
    } else {
        struct NodoSucursales *sucursalNodo = buscarSucursalPorID(farmacia, idSucursal);
        if (sucursalNodo == NULL) {
            printf("Sucursal no encontrada.\n");
            return;
        }
        eliminarProductosVencidos(sucursalNodo->datosSucursal, fechaLimite);
        cls();
        printf("Productos vencidos eliminados correctamente.\n");
    }
}