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

using namespace std;

class Enrutador {
private:
    string id;
    map<string, int> tablaEnlaces;  // Map de destino y costo

public:
    Enrutador(const string& id) : id(id) {}

    map<string, int>& obtenerTablaEnlaces() {
        return tablaEnlaces;
    }

    string obtenerID() const { return id; }

    void agregarEnlace(const string& destino, int costo) {
        tablaEnlaces[destino] = costo;
    }

    void actualizarEnlace(const string& destino, int nuevoCosto) {
        tablaEnlaces[destino] = nuevoCosto;
    }

    void mostrarTabla() const {
        cout << "Tabla de enrutamiento para " << id << ":\n";
        for (const auto& [destino, costo] : tablaEnlaces) {
            cout << "  -> " << destino << " : " << costo << "\n";
        }
    }

    map<string, int> obtenerTablaEnlaces() const {
        return tablaEnlaces;
    }
};

class Red {
private:
    vector<Enrutador> enrutadores;
    vector<string> enrutadoresId;
    vector<vector<int>> predecesor;

    Enrutador* encontrarEnrutador(const string& id) const {
        for (auto& enrutador : enrutadores) {
            if (enrutador.obtenerID() == id) {
                return const_cast<Enrutador*>(&enrutador);
            }
        }
        return nullptr;
    }

public:
    void agregarEnrutador(const string& id) {
        if (!encontrarEnrutador(id)) {
            enrutadores.emplace_back(id);
            enrutadoresId.push_back(id);
            cout << "Enrutador " << id << " agregado a la red.\n";
        } else {
            cout << "El enrutador " << id << " ya existe.\n";
        }
    }

    void agregarEnlace(const string& origen, const string& destino, int costo) {
        Enrutador* eOrigen = encontrarEnrutador(origen);
        Enrutador* eDestino = encontrarEnrutador(destino);
        if (eOrigen && eDestino) {
            eOrigen->agregarEnlace(destino, costo);
            eDestino->agregarEnlace(origen, costo);  // Si es bidireccional
            cout << "Enlace agregado entre " << origen << " y " << destino << " con costo " << costo << ".\n";
        } else {
            cout << "Uno o ambos enrutadores no existen.\n";
        }
    }

    // Función combinada para agregar enrutador con auto conexiones
    void agregarEnrutadorConAutoConexiones() {
        // Generar nombre automáticamente en base al último enrutador agregado
        char ultimoEnrutador = enrutadoresId.empty() ? 'A' : enrutadoresId.back()[0] + 1;
        string nuevoEnrutador(1, ultimoEnrutador);

        // Agregar el nuevo enrutador a la red
        agregarEnrutador(nuevoEnrutador);

        // Conectar automáticamente al último enrutador agregado
        if (enrutadoresId.size() > 1) {
            string enrutadorPrevio = enrutadoresId[enrutadoresId.size() - 2];
            int costo;
            cout << "Ingrese el costo del enlace entre " << enrutadorPrevio << " y " << nuevoEnrutador << ": ";
            cin >> costo;
            agregarEnlace(enrutadorPrevio, nuevoEnrutador, costo);
        }

        // Mostrar otros enrutadores disponibles (excluyendo el nuevo)
        if (enrutadoresId.size() > 2) {
            cout << "Enrutadores disponibles para conexion:\n";
            for (size_t i = 0; i < enrutadoresId.size() - 1; ++i) {
                cout << i + 1 << ". " << enrutadoresId[i] << "\n";
            }

            cout << "Seleccione un enrutador para conectar al nuevo (0 para ninguno): ";
            int opcion;
            cin >> opcion;

            while (opcion != 0 && opcion <= enrutadoresId.size() - 1) {
                string enrutadorSeleccionado = enrutadoresId[opcion - 1];
                int costoExtra;
                cout << "Ingrese el costo del enlace entre " << nuevoEnrutador << " y " << enrutadorSeleccionado << ": ";
                cin >> costoExtra;
                agregarEnlace(nuevoEnrutador, enrutadorSeleccionado, costoExtra);

                cout << "Seleccione otro enrutador (0 para terminar): ";
                cin >> opcion;
            }
        }

        // Actualizar costos con caminos mínimos
        actualizarCostosRed();
    }

    // Definir hayEnlace y obtenerCosto en la clase Red
    bool hayEnlace(int i, int j) const {
        const Enrutador* enrutadorOrigen = encontrarEnrutador(enrutadoresId[i]);
        const Enrutador* enrutadorDestino = encontrarEnrutador(enrutadoresId[j]);

        if (enrutadorOrigen && enrutadorDestino) {
            const auto& enlaces = enrutadorOrigen->obtenerTablaEnlaces();
            return enlaces.find(enrutadoresId[j]) != enlaces.end();
        }
        return false;
    }

    int obtenerCosto(int i, int j) const {
        const Enrutador* enrutadorOrigen = encontrarEnrutador(enrutadoresId[i]);

        if (enrutadorOrigen) {
            const auto& enlaces = enrutadorOrigen->obtenerTablaEnlaces();
            auto it = enlaces.find(enrutadoresId[j]);
            if (it != enlaces.end()) {
                return it->second;
            }
        }
        return INT_MAX;  // Si no hay enlace, retornamos un valor alto (infinito)
    }

    void actualizarCostosRed() {
        int n = enrutadores.size();
        vector<vector<int>> dist(n, vector<int>(n, INT_MAX));
        predecesor.resize(n, vector<int>(n, -1));

        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                if (i == j) {
                    dist[i][j] = 0;
                    predecesor[i][j] = i;
                } else if (hayEnlace(i, j)) {  // Suponiendo que tienes un método para verificar el enlace
                    dist[i][j] = obtenerCosto(i, j); // Método que te da el costo entre dos enrutadores
                    predecesor[i][j] = i;
                }
            }
        }

        // Floyd-Warshall para calcular rutas mínimas
        for (int k = 0; k < n; ++k) {
            for (int i = 0; i < n; ++i) {
                for (int j = 0; j < n; ++j) {
                    if (dist[i][k] != INT_MAX && dist[k][j] != INT_MAX) {
                        dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
                    }
                }
            }
        }

        // Actualizar las tablas de enrutamiento con los costos mínimos
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                if (i != j && dist[i][j] != INT_MAX) {
                    enrutadores[i].actualizarEnlace(enrutadoresId[j], dist[i][j]);
                }
            }
        }
    }

    int encontrarIndiceEnrutador(const string& id) const {
        for (int i = 0; i < enrutadores.size(); ++i) {
            if (enrutadores[i].obtenerID() == id) {
                return i;
            }
        }
        return -1;
    }

    void mostrarTablasEnrutamiento() const {
        for (const auto& enrutador : enrutadores) {
            enrutador.mostrarTabla();
        }
    }

    void removerEnrutador() {
        if (enrutadores.empty()) {
            cout << "No hay enrutadores en la red para eliminar.\n";
            return;
        }

        cout << "Enrutadores disponibles para eliminar:\n";
        for (const auto& id : enrutadoresId) {
            cout << "- " << id << "\n";
        }

        string opcion;
        cout << "Ingrese el identificador del enrutador que desea eliminar (0 para cancelar): ";
        cin >> opcion;

        // Convertir la opción ingresada a mayúsculas para hacer la búsqueda insensible a mayúsculas/minúsculas
        transform(opcion.begin(), opcion.end(), opcion.begin(), ::toupper);

        if (opcion == "0") {
            cout << "Operacion cancelada.\n";
            return;
        }

        // Verificar si el enrutador ingresado existe
        auto itId = find_if(enrutadoresId.begin(), enrutadoresId.end(), [&](const string& id) {
            string idUpper = id;
            transform(idUpper.begin(), idUpper.end(), idUpper.begin(), ::toupper);
            return idUpper == opcion;
        });

        if (itId != enrutadoresId.end()) {
            string id = *itId;

            // Buscar el enrutador por su ID en la lista de enrutadores
            auto it = find_if(enrutadores.begin(), enrutadores.end(), [&](const Enrutador& enrutador) {
                return enrutador.obtenerID() == id;
            });

            if (it != enrutadores.end()) {
                // Eliminar el enrutador de la lista de enrutadores
                enrutadores.erase(it);

                // Eliminar el ID de la lista de enrutadoresId
                enrutadoresId.erase(itId);

                // Eliminar enlaces a este enrutador en el resto de la red
                for (auto& enrutador : enrutadores) {
                    auto& tablaEnlaces = enrutador.obtenerTablaEnlaces();
                    auto enlaceIt = tablaEnlaces.find(id);
                    if (enlaceIt != tablaEnlaces.end()) {
                        tablaEnlaces.erase(enlaceIt);
                    }
                }

                // Actualizar la red tras remover el enrutador
                actualizarCostosRed();
                cout << "Enrutador " << id << " eliminado de la red.\n";
            } else {
                cout << "El enrutador " << id << " no existe en la red.\n";
            }
        } else {
            cout << "El enrutador ingresado no es valido. Intente de nuevo.\n";
        }
    }

    void costoEnvioPaquete(const string& origen, const string& destino) {
        Enrutador* eOrigen = encontrarEnrutador(origen);
        Enrutador* eDestino = encontrarEnrutador(destino);

        if (!eOrigen || !eDestino) {
            cout << "Uno o ambos enrutadores no existen.\n";
            return;
        }

        int indiceOrigen = encontrarIndiceEnrutador(origen);
        int indiceDestino = encontrarIndiceEnrutador(destino);

        // Llama a la función de actualizar costos para asegurarse que los costos mínimos estén actualizados
        actualizarCostosRed();

        int costo = eOrigen->obtenerTablaEnlaces().find(destino) != eOrigen->obtenerTablaEnlaces().end() ? eOrigen->obtenerTablaEnlaces()[destino] :
                        INT_MAX;

        if (costo != INT_MAX) {
            cout << "El costo de enviar el paquete desde " << origen << " hasta " << destino << " es " << costo << ".\n";
        } else {
            cout << "No hay un camino disponible entre " << origen << " y " << destino << ".\n";
        }
    }

    void caminoOptimoPaquete(const string& origen, const string& destino) {
        Enrutador* eOrigen = encontrarEnrutador(origen);
        Enrutador* eDestino = encontrarEnrutador(destino);

        if (!eOrigen || !eDestino) {
            cout << "Uno o ambos enrutadores no existen.\n";
            return;
        }

        int indiceOrigen = encontrarIndiceEnrutador(origen);
        int indiceDestino = encontrarIndiceEnrutador(destino);

        if (indiceOrigen == -1 || indiceDestino == -1) {
            cout << "Uno o ambos enrutadores no existen.\n";
            return;
        }

        // Si no hay un camino
        if (predecesor[indiceOrigen][indiceDestino] == -1) {
            cout << "No hay camino disponible entre " << origen << " y " << destino << ".\n";
            return;
        }

        vector<string> camino;
        while (indiceOrigen != indiceDestino) {
            camino.push_back(enrutadoresId[indiceDestino]);
            indiceDestino = predecesor[indiceOrigen][indiceDestino];
        }
        camino.push_back(enrutadoresId[indiceOrigen]);
        reverse(camino.begin(), camino.end());

        cout << "El camino optimo desde " << origen << " hasta " << destino << " es: ";
        for (const auto& paso : camino) {
            cout << paso << " ";
        }
        cout << "\n";
    }


    void generarRedAleatoria(int cantidadEnrutadores, int maxCosto, const string& archivoRespaldo) {
        // Inicializa el generador de números aleatorios
        srand(time(nullptr));

        // Crear enrutadores
        for (int i = 0; i < cantidadEnrutadores; ++i) {
            char id = 'A' + i;  // Generar un id basado en letras
            agregarEnrutador(string(1, id));
        }

        // Crear enlaces aleatorios
        for (int i = 0; i < cantidadEnrutadores; ++i) {
            for (int j = i + 1; j < cantidadEnrutadores; ++j) {
                int costo = rand() % maxCosto + 1;  // Costo aleatorio entre 1 y maxCosto
                agregarEnlace(enrutadoresId[i], enrutadoresId[j], costo);
            }
        }

        // Actualizar costos con rutas mínimas
        actualizarCostosRed();
        guardarEnArchivo(archivoRespaldo);
    }

    void cargarDesdeArchivo(const string& nombreArchivo) {
        ifstream archivo(nombreArchivo);
        if (!archivo.is_open()) {
            cerr << "Error al abrir el archivo: " << nombreArchivo << "\n";
            return;
        }

        string linea;
        enrutadoresId.clear();
        enrutadores.clear();

        // Leer la primera línea con los identificadores de los enrutadores
        if (getline(archivo, linea)) {
            istringstream iss(linea);
            string id;
            iss >> id; // Salta el primer espacio
            while (iss >> id) {
                agregarEnrutador(id);
            }
        }

        // Leer el resto del archivo para obtener la matriz de costos
        int fila = 0;
        while (getline(archivo, linea)) {
            istringstream iss(linea);
            string origen;
            int costo;
            iss >> origen;  // Leer la letra del enrutador

            if (origen != enrutadoresId[fila]){
                cerr <<"Error en la matriz de costos: el origen no coincide con la fila.\n";
                return;
            }

            int columna = 0;
            while (iss >> costo && columna < enrutadores.size()) {
                if (origen != enrutadoresId[columna] && costo != 0) {  // Evitar agregar enlace consigo mismo
                    agregarEnlace(origen, enrutadoresId[columna], costo);
                }
                columna++;
            }
            fila++;
        }

        archivo.close();
    }

    void guardarEnArchivo(const string& nombreArchivo) const {
        ofstream archivo(nombreArchivo);
        if (!archivo.is_open()) {
            cerr << "Error al abrir el archivo para guardar: " << nombreArchivo << "\n";
            return;
        }

        archivo << "0";
        for (const auto& id : enrutadoresId) {
            archivo << " " << id;
        }
        archivo << "\n";

        for (const auto& origen : enrutadoresId) {
            archivo << origen;
            for (const auto& destino : enrutadoresId) {
                Enrutador* eOrigen = encontrarEnrutador(origen);
                if (origen == destino) {
                    archivo << " 0";
                } else if (eOrigen) {
                    auto enlaces = eOrigen->obtenerTablaEnlaces();
                    if (enlaces.find(destino) != enlaces.end()) {
                        archivo << " " << enlaces[destino];
                    } else {
                        archivo << " 0 ";
                    }
                }
            }
            archivo << "\n";
        }

        archivo.close();
    }
};

void mostrarMenu() {
    cout << "\n--- MENU INTERACTIVO ---\n";
    cout << "1. Agregar enrutador\n";
    cout << "2. Remover enrutador\n";
    cout << "3. Mostrar tablas de enrutamiento\n";
    cout << "4. Consultar costo de envio de paquete\n";
    cout << "5. Consultar camino optimo de envio de paquete\n";
    cout << "6. Generar red aleatoria\n";
    cout << "7. Guardar\n";
    cout << "8. Guardar y salir\n";
    cout << "Seleccione una opcion: ";
}

int main() {
    Red red;
    string archivo = "matriz.txt";
    red.cargarDesdeArchivo(archivo);

    int opcion;
    do {
        mostrarMenu();
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