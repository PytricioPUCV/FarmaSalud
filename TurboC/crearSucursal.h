void agregarSucursal(struct FarmaSalud *farmacia, struct NodoSucursales *nuevaSucursal) {
    if (farmacia->sucursales == NULL) {
        farmacia->sucursales = nuevaSucursal;
        nuevaSucursal->ant = nuevaSucursal->sig = nuevaSucursal;
    } else {
        struct NodoSucursales *ultimo = farmacia->sucursales->ant;
        ultimo->sig = nuevaSucursal;
        nuevaSucursal->ant = ultimo;
        nuevaSucursal->sig = farmacia->sucursales;
        farmacia->sucursales->ant = nuevaSucursal;
    }
}

void leerDatosSucursal(struct Sucursal *nuevaSucursal) {
    cls();
    printf("Ingrese ID de la sucursal: ");
    scanf("%d", &nuevaSucursal->id);
    getchar(); // Limpiar el buffer de entrada después de leer un entero

    nuevaSucursal->nombre = (char *)malloc(50 * sizeof(char));
    printf("Ingrese nombre de la sucursal: ");
    scanf(" %[^\n]", nuevaSucursal->nombre);
    getchar(); // Limpiar el buffer de entrada después de leer un string

    nuevaSucursal->direccion = (char *)malloc(100 * sizeof(char));
    printf("Ingrese dirección de la sucursal: ");
    scanf(" %[^\n]", nuevaSucursal->direccion);
    getchar(); // Limpiar el buffer de entrada después de leer un string

    nuevaSucursal->cantidadDeVentas = 0; // Inicializar a 0
    nuevaSucursal->capacidadAlmacenamiento = 0; // Cambiar si se necesita un valor específico
    nuevaSucursal->productos = NULL;
    nuevaSucursal->productosVendidos = NULL; // Inicializar a NULL
    nuevaSucursal->numRegistros = 0; // Inicializar a 0
    for (int i = 0; i < MAX_ENVIOS; i++) {
        nuevaSucursal->registrosEnvios[i] = NULL; // Inicializar a NULL
    }
}

void crearSucursal(struct FarmaSalud *farmacia) {
    struct NodoSucursales *nuevoNodo = (struct NodoSucursales*)malloc(sizeof(struct NodoSucursales));
    struct Sucursal *nuevaSucursal = (struct Sucursal*)malloc(sizeof(struct Sucursal));

    nuevoNodo->datosSucursal = nuevaSucursal;
    nuevoNodo->ant = nuevoNodo->sig = NULL;

    leerDatosSucursal(nuevaSucursal);
    agregarSucursal(farmacia, nuevoNodo);

    cls();
    printf("Sucursal agregada con éxito.\n");
    pause();
}