void menuPrincipal(struct FarmaSalud *farmacia) {
    int opcion;
    do {
        printf("\n--- FarmaSalud ---\n");
        printf("1. leerClientes\n");
        printf("2. crearCliente\n");
        printf("3. eliminarCliente\n");
        printf("4. agregarProductoACliente\n");
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
        printf("16. Ver Informes\n"); // Nueva opción

        printf("\n17. Salir\n"); // Cambiamos a 17 para que se ajuste
        printf("\nSeleccione una opcion: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                leerClientes(farmacia);
                cls();
                break;
            case 2:
                agregarCliente(farmacia);
                cls();
                break;
            case 3:
                eliminarCliente(farmacia);
                cls();
                break;
            case 4:
                agregarProductoACliente(farmacia);
                cls();
                break;
            case 5:
                leerSucursales(farmacia);
                cls();
                break;
            case 6:
                crearSucursal(farmacia);
                cls();
                break;
            case 7:
                eliminarSucursal(farmacia);
                cls();
                break;
            case 8:
                crearProveedor(farmacia);
                cls();
                break;
            case 9:
                leerProveedores(farmacia);
                cls();
                break;
            case 10:
                eliminarProveedor(farmacia);
                cls();
                break;
            case 11:
                agregarProductoProveedor(farmacia);
                cls();
                break;
            case 12:
                transferirProductosProveedorASucursal(farmacia);
                cls();
                break;
            case 13:
                transferirProductoProveedorASucursal(farmacia);
                cls();
                break;
            case 14:
                menuEliminarProductosVencidos(farmacia);
                cls();
                break;
            case 15:
                realizarCompra(farmacia);
                actualizarInventariosSucursales(farmacia);
                cls();
                break;
            case 16:
                mostrarInforme(farmacia);
                cls();
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
