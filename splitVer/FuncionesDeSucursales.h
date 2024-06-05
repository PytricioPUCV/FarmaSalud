// 4. Funciones de sucursales
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
        printf("Direccion: %s\n", nodoActual->datosSucursal->direccion);
        printf("Cantidad de Ventas: %d\n", nodoActual->datosSucursal->cantidadDeVentas);

        if (nodoActual->datosSucursal->productos != NULL) {
            printf("Productos:\n");
            struct NodoProducto *productoNodo = nodoActual->datosSucursal->productos;
            while (productoNodo != NULL) {
                printf("\t%s - %s", productoNodo->datosProducto->codigo, productoNodo->datosProducto->nombreProducto);
                printf("\tCantidad en stock: %d", productoNodo->datosProducto->cantidad);
                printf("\tFecha de caducidad: %s", productoNodo->datosProducto->fechaCaducidad);
                printf("\tLote: %s\n", productoNodo->datosProducto->lote);
                productoNodo = productoNodo->sig;
            }
            printf("\n");
        } 
        else 
        {
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
        printf("Sucursal o producto no valido.\n");
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
    printf("Ingrese direccion de la sucursal: ");
    nuevaSucursal->direccion = (char *)malloc(100 * sizeof(char));
    scanf("%s", nuevaSucursal->direccion);

    nuevaSucursal->cantidadDeVentas = 0; // Inicializar a 0
    nuevaSucursal->capacidadAlmacenamiento = 0; // Cambiar si se necesita un valor específico
    nuevaSucursal->productos = NULL;
    nuevaSucursal->productosVendidos = NULL; // Inicializar a NULL

    agregarSucursalConsole(farmacia, nuevoNodo);

    cls();
    printf("Sucursal agregada con exito.\n");
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
    // Aquí podrías añadir código para liberar productos asociados si es necesario
    free(temp->datosSucursal);
    free(temp);

    cls();
    printf("Sucursal eliminada con exito.\n");
    pause();
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

void mostrarSucursales(struct FarmaSalud *farmacia) {
    struct NodoSucursales *sucursalActual = farmacia->sucursales;
    if (sucursalActual == NULL) {
        printf("No hay sucursales registradas.\n");
        return;
    }

    struct NodoSucursales *inicio = sucursalActual;
    printf("Sucursales disponibles:\n");
    do {
        printf("ID: %d, Nombre: %s\n", sucursalActual->datosSucursal->id, sucursalActual->datosSucursal->nombre);
        sucursalActual = sucursalActual->sig;
    } while (sucursalActual != inicio);
    printf("\n");
}

void eliminarProductosVencidos(struct FarmaSalud* farmacia, int idSucursal, char* fechaEliminacion) {
    struct NodoSucursales* nodoSucursal = farmacia->sucursales;
    struct NodoProducto* nodoProducto;
    struct NodoProducto* nodoTemporal;

    if (idSucursal == -1) { // Eliminar productos vencidos de todas las sucursales
        while (nodoSucursal != NULL) {
            nodoProducto = nodoSucursal->datosSucursal->productos;
            while (nodoProducto != NULL) {
                if (compararFechas(nodoProducto->datosProducto->fechaCaducidad, fechaEliminacion) < 0) {
                    // Eliminar el producto vencido
                    nodoTemporal = nodoProducto;
                    if (nodoProducto->ant == NULL) {
                        nodoSucursal->datosSucursal->productos = nodoProducto->sig;
                    } else {
                        nodoProducto->ant->sig = nodoProducto->sig;
                    }
                    if (nodoProducto->sig != NULL) {
                        nodoProducto->sig->ant = nodoProducto->ant;
                    }
                    free(nodoTemporal->datosProducto->nombreProducto);
                    free(nodoTemporal->datosProducto->descripcion);
                    free(nodoTemporal->datosProducto->categoria);
                    free(nodoTemporal->datosProducto->idProveedor);
                    free(nodoTemporal->datosProducto->lote);
                    free(nodoTemporal->datosProducto->fechaCaducidad);
                    free(nodoTemporal->datosProducto);
                    free(nodoTemporal);
                    nodoProducto = nodoProducto->sig;
                } else {
                    nodoProducto = nodoProducto->sig;
                }
            }
            nodoSucursal = nodoSucursal->sig;
        }
    } else { // Eliminar productos vencidos de una sucursal específica
        while (nodoSucursal != NULL) {
            if (nodoSucursal->datosSucursal->id == idSucursal) {
                nodoProducto = nodoSucursal->datosSucursal->productos;
                while (nodoProducto != NULL) {
                    if (compararFechas(nodoProducto->datosProducto->fechaCaducidad, fechaEliminacion) < 0) {
                        // Eliminar el producto vencido
                        nodoTemporal = nodoProducto;
                        if (nodoProducto->ant == NULL) {
                            nodoSucursal->datosSucursal->productos = nodoProducto->sig;
                        } else {
                            nodoProducto->ant->sig = nodoProducto->sig;
                        }
                        if (nodoProducto->sig != NULL) {
                            nodoProducto->sig->ant = nodoProducto->ant;
                        }
                        free(nodoTemporal->datosProducto->nombreProducto);
                        free(nodoTemporal->datosProducto->descripcion);
                        free(nodoTemporal->datosProducto->categoria);
                        free(nodoTemporal->datosProducto->idProveedor);
                        free(nodoTemporal->datosProducto->lote);
                        free(nodoTemporal->datosProducto->fechaCaducidad);
                        free(nodoTemporal->datosProducto);
                        free(nodoTemporal);
                        nodoProducto = nodoProducto->sig;
                    } else {
                        nodoProducto = nodoProducto->sig;
                    }
                }
                break;
            }
            nodoSucursal = nodoSucursal->sig;
        }
    }
}

void menuEliminarProductosVencidos(struct FarmaSalud* farmacia) {
    int idSucursal;
    char fechaEliminacion[11];

    printf("\nEliminar productos vencidos\n");
    printf("Ingrese el ID de la sucursal (-1 para todas las sucursales): ");
    scanf("%d", &idSucursal);

    printf("Ingrese la fecha de vencimiento (DD/MM/YYYY): ");
    scanf("%10s", fechaEliminacion);

    eliminarProductosVencidos(farmacia, idSucursal, fechaEliminacion);
    printf("Productos vencidos eliminados correctamente.\n");
}