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
