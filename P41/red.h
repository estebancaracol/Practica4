#ifndef RED_H
#define RED_H

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <climits>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <sstream>
#include "Enrutador.h"

using namespace std;

class Red {
private:
    vector<Enrutador> enrutadores;
    vector<string> enrutadoresId;
    vector<vector<int>> predecesor;

    Enrutador* encontrarEnrutador(const string& id) const;

public:
    void agregarEnrutador(const string& id);
    void agregarEnlace(const string& origen, const string& destino, int costo);
    void agregarEnrutadorConAutoConexiones();
    bool hayEnlace(int i, int j) const;
    int obtenerCosto(int i, int j) const;
    void actualizarCostosRed();
    int encontrarIndiceEnrutador(const string& id) const;
    void mostrarTablasEnrutamiento() const;
    void removerEnrutador();
    void costoEnvioPaquete(const string& origen, const string& destino);
    void caminoOptimoPaquete(const string& origen, const string& destino);
    void generarRedAleatoria(int cantidadEnrutadores, int maxCosto, const string& archivoRespaldo);
    void cargarDesdeArchivo(const string& nombreArchivo);
    void guardarEnArchivo(const string& nombreArchivo) const;
    void mostrarMenu();
};


#endif // RED_H

