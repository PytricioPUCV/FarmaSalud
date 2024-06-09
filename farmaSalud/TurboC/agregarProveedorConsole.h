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
