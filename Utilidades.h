void pause() {
    printf("\nPresione Enter para continuar...\n");
    while (getchar() != '\n'); // Limpiar el búfer de entrada
    getchar(); // Esperar a que se presione Enter
}

void cls() {
    int i;
    for(i=0;i<20;i++) printf("\n");
}

void quitarNuevaLinea(char *cadena) {
    int longitud = strlen(cadena);
    if (cadena[longitud - 1] == '\n') {
        cadena[longitud - 1] = '\0';
    }
}

 int compararFechas(char* fechaProducto, char* fechaEliminacion) {
    int diaProducto, mesProducto, anioProducto;
    int diaEliminacion, mesEliminacion, anioEliminacion;

    sscanf(fechaProducto, "%d/%d/%d", &diaProducto, &mesProducto, &anioProducto);
    sscanf(fechaEliminacion, "%d/%d/%d", &diaEliminacion, &mesEliminacion, &anioEliminacion);

    if (anioProducto < anioEliminacion) {
        return -1; // Fecha del producto anterior a la fecha de eliminación
    } else if (anioProducto > anioEliminacion) {
        return 1; // Fecha del producto posterior a la fecha de eliminación
    } else { // Mismo año
        if (mesProducto < mesEliminacion) {
            return -1;
        } else if (mesProducto > mesEliminacion) {
            return 1;
        } else { // Mismo mes
            if (diaProducto < diaEliminacion) {
                return -1;
            } else if (diaProducto > diaEliminacion) {
                return 1;
            } else {
                return 0; // Misma fecha
            }
        }
    }
}