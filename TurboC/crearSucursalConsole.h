char* my_strdup(const char* str) {
    char* dup = (char*)malloc(strlen(str) + 1);
    if (dup != NULL) {
        strcpy(dup, str);
    }
    return dup;
}

struct NodoSucursales* crearSucursalConsole(int id, char* nombre, char* direccion, int capacidadAlmacenamiento) {
    struct NodoSucursales* nuevoNodo = (struct NodoSucursales*)malloc(sizeof(struct NodoSucursales));
    struct Sucursal* nuevaSucursal = (struct Sucursal*)malloc(sizeof(struct Sucursal));
    
    if (nuevoNodo == NULL || nuevaSucursal == NULL) {
        exit(1);
    }

    nuevaSucursal->id = id;
    nuevaSucursal->nombre = my_strdup(nombre);
    nuevaSucursal->direccion = my_strdup(direccion);
    nuevaSucursal->cantidadDeVentas = 0; 
    nuevaSucursal->capacidadAlmacenamiento = capacidadAlmacenamiento;
    nuevaSucursal->productos = NULL;
    nuevaSucursal->productosVendidos = NULL; 
    nuevaSucursal->numRegistros = 0; 
    for (int i = 0; i < MAX_ENVIOS; i++) {
        nuevaSucursal->registrosEnvios[i] = NULL; 
    }

    nuevoNodo->datosSucursal = nuevaSucursal;
    nuevoNodo->ant = nuevoNodo->sig = nuevoNodo;
    
    return nuevoNodo;
}