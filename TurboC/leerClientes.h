// Controlador
struct NodoClientes* obtenerClienteActual(struct FarmaSalud *farmacia) {
    return farmacia->clientes;
}

struct NodoClientes* obtenerClienteSiguiente(struct NodoClientes *nodoActual) {
    return nodoActual->sig;
}

// Vista
void imprimirCliente(struct NodoClientes *nodoActual) {
    int i;
    struct Producto *producto;

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
        for (i = 0; i < nodoActual->datosClientes->numCompras; i++) {
            producto = nodoActual->datosClientes->comprasCliente[i];
            if (producto != NULL) {
                printf("\t%s - %s\n", producto->codigo, producto->nombreProducto);
            }
        }
        printf("\n");
    } else {
        printf("Cliente sin compras\n\n");
    }
}

// Función principal
void leerClientes(struct FarmaSalud *farmacia) {
    struct NodoClientes *nodoActual;

    cls();
    nodoActual = obtenerClienteActual(farmacia);
    while(nodoActual != NULL) {
        imprimirCliente(nodoActual);
        nodoActual = obtenerClienteSiguiente(nodoActual);
    }
    pause();
}