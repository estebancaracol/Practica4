#include "Red.h"

Enrutador* Red::encontrarEnrutador(const string& id) const {
    for (auto& enrutador : enrutadores) {
        if (enrutador.obtenerID() == id) {
            return const_cast<Enrutador*>(&enrutador);
        }
    }
    return nullptr;
}

void Red::agregarEnrutador(const string& id) {
    if (!encontrarEnrutador(id)) {
        enrutadores.emplace_back(id);
        enrutadoresId.push_back(id);
        cout << "Enrutador " << id << " agregado a la red.\n";
    } else {
        cout << "El enrutador " << id << " ya existe.\n";
    }
}

void Red::agregarEnlace(const string& origen, const string& destino, int costo) {
    Enrutador* eOrigen = encontrarEnrutador(origen);
    Enrutador* eDestino = encontrarEnrutador(destino);
    if (eOrigen && eDestino) {
        eOrigen->agregarEnlace(destino, costo);
        eDestino->agregarEnlace(origen, costo);
        cout << "Enlace agregado entre " << origen << " y " << destino << " con costo " << costo << ".\n";
    } else {
        cout << "Uno o ambos enrutadores no existen.\n";
    }
}

void Red::agregarEnrutadorConAutoConexiones() {
    char ultimoEnrutador = enrutadoresId.empty() ? 'A' : enrutadoresId.back()[0] + 1;
    string nuevoEnrutador(1, ultimoEnrutador);
    agregarEnrutador(nuevoEnrutador);

    if (enrutadoresId.size() > 1) {
        string enrutadorPrevio = enrutadoresId[enrutadoresId.size() - 2];
        int costo;
        cout << "Ingrese el costo del enlace entre " << enrutadorPrevio << " y " << nuevoEnrutador << ": ";
        cin >> costo;
        agregarEnlace(enrutadorPrevio, nuevoEnrutador, costo);
    }

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

    actualizarCostosRed();
}

bool Red::hayEnlace(int i, int j) const {
    const Enrutador* enrutadorOrigen = encontrarEnrutador(enrutadoresId[i]);
    const Enrutador* enrutadorDestino = encontrarEnrutador(enrutadoresId[j]);

    if (enrutadorOrigen && enrutadorDestino) {
        const auto& enlaces = enrutadorOrigen->obtenerTablaEnlaces();
        return enlaces.find(enrutadoresId[j]) != enlaces.end();
    }
    return false;
}

int Red::obtenerCosto(int i, int j) const {
    const Enrutador* enrutadorOrigen = encontrarEnrutador(enrutadoresId[i]);

    if (enrutadorOrigen) {
        const auto& enlaces = enrutadorOrigen->obtenerTablaEnlaces();
        auto it = enlaces.find(enrutadoresId[j]);
        if (it != enlaces.end()) {
            return it->second;
        }
    }
    return INT_MAX;
}

void Red::actualizarCostosRed() {
    int n = enrutadores.size();
    vector<vector<int>> dist(n, vector<int>(n, INT_MAX));
    predecesor.resize(n, vector<int>(n, -1));

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i == j) {
                dist[i][j] = 0;
                predecesor[i][j] = i;
            } else if (hayEnlace(i, j)) {
                dist[i][j] = obtenerCosto(i, j);
                predecesor[i][j] = i;
            }
        }
    }

    for (int k = 0; k < n; ++k) {
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                if (dist[i][k] != INT_MAX && dist[k][j] != INT_MAX) {
                    dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
                }
            }
        }
    }

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i != j && dist[i][j] != INT_MAX) {
                enrutadores[i].actualizarEnlace(enrutadoresId[j], dist[i][j]);
            }
        }
    }
}

int Red::encontrarIndiceEnrutador(const string& id) const {
    for (int i = 0; i < enrutadores.size(); ++i) {
        if (enrutadores[i].obtenerID() == id) {
            return i;
        }
    }
    return -1;
}

void Red::mostrarTablasEnrutamiento() const {
    for (const auto& enrutador : enrutadores) {
        enrutador.mostrarTabla();
    }
}

void Red::removerEnrutador() {
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

    transform(opcion.begin(), opcion.end(), opcion.begin(), ::toupper);

    if (opcion == "0") {
        cout << "Operacion cancelada.\n";
        return;
    }

    auto itId = find_if(enrutadoresId.begin(), enrutadoresId.end(), [&](const string& id) {
        string idUpper = id;
        transform(idUpper.begin(), idUpper.end(), idUpper.begin(), ::toupper);
        return idUpper == opcion;
    });

    if (itId != enrutadoresId.end()) {
        string id = *itId;
        auto it = find_if(enrutadores.begin(), enrutadores.end(), [&](const Enrutador& enrutador) {
            return enrutador.obtenerID() == id;
        });

        if (it != enrutadores.end()) {
            enrutadores.erase(it);
            enrutadoresId.erase(itId);

            for (auto& enrutador : enrutadores) {
                auto& tablaEnlaces = enrutador.obtenerTablaEnlaces();
                auto enlaceIt = tablaEnlaces.find(id);
                if (enlaceIt != tablaEnlaces.end()) {
                    tablaEnlaces.erase(enlaceIt);
                }
            }

            cout << "Enrutador " << id << " eliminado de la red.\n";
            actualizarCostosRed();
        }
    } else {
        cout << "El enrutador no fue encontrado en la red.\n";
    }
}

void Red::costoEnvioPaquete(const string& origen, const string& destino) {
    int indiceOrigen = encontrarIndiceEnrutador(origen);
    int indiceDestino = encontrarIndiceEnrutador(destino);

    if (indiceOrigen == -1 || indiceDestino == -1) {
        cout << "Uno o ambos enrutadores no existen.\n";
        return;
    }

    int costo = obtenerCosto(indiceOrigen, indiceDestino);
    if (costo == INT_MAX) {
        cout << "No hay un enlace directo entre " << origen << " y " << destino << ".\n";
    } else {
        cout << "El costo para enviar un paquete desde " << origen << " a " << destino << " es " << costo << ".\n";
    }
}

void Red::caminoOptimoPaquete(const string& origen, const string& destino) {
    int indiceOrigen = encontrarIndiceEnrutador(origen);
    int indiceDestino = encontrarIndiceEnrutador(destino);

    if (indiceOrigen == -1 || indiceDestino == -1) {
        cout << "Uno o ambos enrutadores no existen.\n";
        return;
    }

    if (predecesor[indiceOrigen][indiceDestino] == -1) {
        cout << "No hay ruta entre " << origen << " y " << destino << ".\n";
    } else {
        vector<string> camino;
        int current = indiceDestino;

        while (current != indiceOrigen) {
            camino.push_back(enrutadoresId[current]);
            current = predecesor[indiceOrigen][current];
        }

        camino.push_back(enrutadoresId[indiceOrigen]);
        reverse(camino.begin(), camino.end());

        cout << "El camino más corto desde " << origen << " hasta " << destino << " es: ";
        for (const auto& id : camino) {
            cout << id << " ";
        }
        cout << "\n";
    }
}

void Red::generarRedAleatoria(int cantidadEnrutadores, int maxCosto, const string& archivoRespaldo) {
    srand(time(nullptr));

    enrutadores.clear();
    enrutadoresId.clear();

    for (int i = 0; i < cantidadEnrutadores; ++i) {
        char id = 'A' + i;
        agregarEnrutador(string(1, id));
    }

    for (int i = 0; i < cantidadEnrutadores; ++i) {
        for (int j = i + 1; j < cantidadEnrutadores; ++j) {
            int costo = rand() % maxCosto + 1;
            agregarEnlace(enrutadoresId[i], enrutadoresId[j], costo);
        }
    }

    guardarEnArchivo(archivoRespaldo);
    actualizarCostosRed();
}

void Red::cargarDesdeArchivo(const string& nombreArchivo) {
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

void Red::guardarEnArchivo(const string& nombreArchivo) const {
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

void Red::mostrarMenu() {
    cout << "\n-------MENU-------\n";
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

