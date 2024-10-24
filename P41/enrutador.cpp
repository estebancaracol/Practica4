#include "Enrutador.h"

// Constructor
Enrutador::Enrutador(const string& id) : id(id) {}

// Obtener la tabla de enlaces
map<string, int>& Enrutador::obtenerTablaEnlaces() {
    return tablaEnlaces;
}

// Obtener ID del enrutador
string Enrutador::obtenerID() const {
    return id;
}

// Agregar enlace
void Enrutador::agregarEnlace(const string& destino, int costo) {
    tablaEnlaces[destino] = costo;
}

// Actualizar enlace
void Enrutador::actualizarEnlace(const string& destino, int nuevoCosto) {
    tablaEnlaces[destino] = nuevoCosto;
}

// Mostrar la tabla de enrutamiento
void Enrutador::mostrarTabla() const {
    cout << "Tabla de enrutamiento para " << id << ":\n";
    for (const auto& [destino, costo] : tablaEnlaces) {
        cout << "  -> " << destino << " : " << costo << "\n";
    }
}

// Obtener la tabla de enlaces (versión constante)
map<string, int> Enrutador::obtenerTablaEnlaces() const {
    return tablaEnlaces;
}
