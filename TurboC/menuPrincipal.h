void menuPrincipal(struct FarmaSalud *farmacia) {
    int opcion;

    do {
        cls();
        printf("\n--- FarmaSalud ---\n");
        printf("1. leerClientes\n");
        printf("2. crearCliente\n");
        printf("3. eliminarCliente\n");
        printf("4. actualizarSucursales\n");
        printf("5. leerSucursales\n");
        printf("6. crearSucursal\n");
        printf("7. eliminarSucursal\n");
        printf("8. crearProveedor\n");
        printf("9. leerProveedor\n");
        printf("10. eliminarProveedor\n");
        printf("11. agregarProductoProveedor\n");
        printf("12. transferirProductosProveedorASucursal\n");
        printf("13. agregarProductoASucursal\n");
        printf("14. eliminarProductosVencidos\n");
        printf("15. realizarCompra\n");
        printf("16. Ver Informes\n"); 

        printf("\n17. Salir\n"); 
        printf("\nSeleccione una opcion: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                leerClientes(farmacia);
                break;
            case 2:
                agregarCliente(farmacia);
                break;
            case 3:
                eliminarCliente(farmacia);
                break;
            case 4:
                actualizarInventariosSucursales(farmacia);
                break;
            case 5:
                leerSucursales(farmacia);
                break;
            case 6:
                crearSucursal(farmacia);
                break;
            case 7:
                eliminarSucursalView(farmacia);
                break;
            case 8:
                crearProveedor(farmacia);
                break;
            case 9:
                leerProveedores(farmacia);
                break;
            case 10:
                eliminarProveedor(farmacia);
                break;
            case 11:
                agregarProductoProveedor(farmacia);
                break;
            case 12:
                transferirProductosProveedorASucursal(farmacia);
                break;
            case 13:
                transferirProductoProveedorASucursal(farmacia);
                break;
            case 14:
                menuEliminarProductosVencidos(farmacia);
                break;
            case 15:
                realizarCompra(farmacia);
                actualizarInventariosSucursales(farmacia);
                break;
            case 16:
                mostrarInforme(farmacia);
                break;
            case 17: // Actualizar opción de salida
                printf("Saliendo...\n");
                exit(0);
                break;
            default:
                printf("Opcion no valida. Intente nuevamente.\n");
                break;
        }
    } while (opcion != 17);
}