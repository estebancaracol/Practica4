#ifndef ENRUTADOR_H
#define ENRUTADOR_H

#include <iostream>
#include <string>
#include <map>
using namespace std;

class Enrutador {
private:
    string id;
    map<string, int> tablaEnlaces;  // Mapa de destino y costo

public:
    // Constructor
    Enrutador(const string& id);

    // Obtener la tabla de enlaces
    map<string, int>& obtenerTablaEnlaces();

    // Obtener ID del enrutador
    string obtenerID() const;

    // Agregar enlace
    void agregarEnlace(const string& destino, int costo);

    // Actualizar enlace
    void actualizarEnlace(const string& destino, int nuevoCosto);

    // Mostrar la tabla de enrutamiento
    void mostrarTabla() const;

    // Obtener la tabla de enlaces (versión constante)
    map<string, int> obtenerTablaEnlaces() const;
};

#endif // ENRUTADOR_H

