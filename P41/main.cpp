#include <iostream>
#include <vector>
#include <map>
#include <list>
#include <limits>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <string>
#include <fstream>
#include <sstream>
#include "red.h"
#include "enrutador.h"

using namespace std;

int main() {
    Red red;
    string archivo = "matriz.txt";
    red.cargarDesdeArchivo(archivo);

    int opcion;
    do {
        red.mostrarMenu();
        cin >> opcion;

        switch (opcion) {
        case 1:
            red.agregarEnrutadorConAutoConexiones();
            break;
        case 2:
            red.removerEnrutador();
            break;
        case 3:
            red.mostrarTablasEnrutamiento();
            break;
        case 4: {
            string origen, destino;
            cout << "Ingrese el enrutador origen: ";
            cin >> origen;
            cout << "Ingrese el enrutador destino: ";
            cin >> destino;
            red.costoEnvioPaquete(origen, destino);
            break;
        }
        case 5: {
            string origen, destino;
            cout << "Ingrese el enrutador origen: ";
            cin >> origen;
            cout << "Ingrese el enrutador destino: ";
            cin >> destino;
            red.caminoOptimoPaquete(origen, destino);
            break;
        }
        case 6: {
            int cantidadEnrutadores, maxCosto;
            cout << "Ingrese la cantidad de enrutadores: ";
            cin >> cantidadEnrutadores;
            cout << "Ingrese el costo maximo de los enlaces: ";
            cin >> maxCosto;
            red.generarRedAleatoria(cantidadEnrutadores, maxCosto, "matrizRespaldo.txt");
            break;
        }
        case 7:
            cout << "Saliendo sin guardar...\n";
            return 0; // Salir sin guardar
        case 8:
            red.guardarEnArchivo(archivo);
            cout << "Datos guardados en " << archivo << ". Saliendo...\n";
            return 0; // Guardar y salir
        default:
            cout << "Opcion no valida. Intente de nuevo.\n";
        }
    } while (opcion != 7 && opcion != 8);  // Se repite el menú hasta seleccionar salir (7 o 8)

    return 0;
}




