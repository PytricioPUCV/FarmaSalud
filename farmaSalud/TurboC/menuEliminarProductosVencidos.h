void extraerFecha(const char *fecha, int *mes, int *anio) {
    sscanf(fecha, "%d/%d", mes, anio);
}

void imprimirSucursalOProductosInvalidos() {
    printf("Sucursal o productos no válidos.\n");
}

void imprimirFechaCaducidadInvalida() {
    printf("Fecha de caducidad inválida.\n");
}

void imprimirNoHaySucursalesRegistradas() {
    printf("No hay sucursales registradas.\n");
}

void imprimirProductosVencidosEliminadosDeTodasLasSucursales() {
    printf("Productos vencidos eliminados de todas las sucursales correctamente.\n");
}

void imprimirSucursalNoEncontrada() {
    printf("Sucursal no encontrada.\n");
}

void imprimirProductosVencidosEliminadosCorrectamente() {
    printf("Productos vencidos eliminados correctamente.\n");
}

void imprimirIngreseIDSucursal() {
    printf("Ingrese el ID de la sucursal (o -1 para todas las sucursales): ");
}

void imprimirIngreseFechaVencimiento() {
    printf("Ingrese la fecha de vencimiento (MM/AAAA): ");
}

void imprimirFechaCaducidadInvalidaMenu() {
    printf("Fecha de caducidad invalida. Por favor, ingrese una fecha válida.\n");
}

int compararFechasVencidas(char *fecha1, char *fecha2) {
    int mes1, anio1, mes2, anio2;

    extraerFecha(fecha1, &mes1, &anio1);
    extraerFecha(fecha2, &mes2, &anio2);

    if (anio1 < anio2) return -1;
    if (anio1 > anio2) return 1;
    if (mes1 < mes2) return -1;
    if (mes1 > mes2) return 1;
    return 0;
}

int validarFecha(const char *fecha) {
    int mes, anio;
    extraerFecha(fecha, &mes, &anio);
    if (mes < 1 || mes > 12) {
        return 0;
    }
    return 1;
}

void eliminarProductosVencidos(struct Sucursal *sucursal, char *fechaLimite) {
    if (sucursal == NULL || sucursal->productos == NULL) {
        imprimirSucursalOProductosInvalidos();
        return;
    }
    if (!validarFecha(fechaLimite)) {
        imprimirFechaCaducidadInvalida();
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

            if (productoActual->datosProducto->nombreProducto != NULL) {
                free(productoActual->datosProducto->nombreProducto);
            }
            if (productoActual->datosProducto->descripcion != NULL) {
                free(productoActual->datosProducto->descripcion);
            }
            if (productoActual->datosProducto->categoria != NULL) {
                free(productoActual->datosProducto->categoria);
            }
            if (productoActual->datosProducto->idProveedor != NULL) {
                free(productoActual->datosProducto->idProveedor);
            }
            if (productoActual->datosProducto->lote != NULL) {
                free(productoActual->datosProducto->lote);
            }
            if (productoActual->datosProducto->fechaCaducidad != NULL) {
                free(productoActual->datosProducto->fechaCaducidad);
            }
            free(productoActual->datosProducto);
            free(productoActual);
        }
        productoActual = siguienteProducto;
    }
}

void eliminarProductosVencidosDeTodasLasSucursales(struct FarmaSalud *farmacia, char *fechaLimite) {
    if (farmacia == NULL || farmacia->sucursales == NULL) {
        imprimirNoHaySucursalesRegistradas();
        return;
    }

    if (!validarFecha(fechaLimite)) {
        imprimirFechaCaducidadInvalida();
        return;
    }

    struct NodoSucursales *sucursalActual = farmacia->sucursales;
    do {
        eliminarProductosVencidos(sucursalActual->datosSucursal, fechaLimite);
        sucursalActual = sucursalActual->sig;
    } while (sucursalActual != farmacia->sucursales);

    imprimirProductosVencidosEliminadosDeTodasLasSucursales();
}

void menuEliminarProductosVencidos(struct FarmaSalud *farmacia) {
    cls();
    int idSucursal;
    char fechaLimite[8];

    mostrarSucursales(farmacia);
    imprimirIngreseIDSucursal();
    scanf("%d", &idSucursal);

    do {
        imprimirIngreseFechaVencimiento();
        scanf("%s", fechaLimite);

        if (!validarFecha(fechaLimite)) {
            imprimirFechaCaducidadInvalidaMenu();
        }
    } while (!validarFecha(fechaLimite));

    if (idSucursal == -1) {
        eliminarProductosVencidosDeTodasLasSucursales(farmacia, fechaLimite);
    } else {
        struct NodoSucursales *sucursalNodo = buscarSucursalPorID(farmacia, idSucursal);
        if (sucursalNodo == NULL) {
            imprimirSucursalNoEncontrada();
            return;
        }
        eliminarProductosVencidos(sucursalNodo->datosSucursal, fechaLimite);
        cls();
        imprimirProductosVencidosEliminadosCorrectamente();
        pause();
    }
}