#ifndef CHM_CPP
#define CHM_CPP

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <pthread.h>

#include "CargarArchivos.hpp"

using namespace std;
int cargarArchivo(
    HashMapConcurrente &hashMap,
    std::string filePath
) {
    std::fstream file;
    int cant = 0;
    std::string palabraActual;

    // Abro el archivo.
    file.open(filePath, file.in);
    if (!file.is_open()) {
        std::cerr << "Error al abrir el archivo '" << filePath << "'" << std::endl;
        return -1;
    }
    while (file >> palabraActual) {
        // Completar (Ejercicio 4)
        hashMap.incrementar(palabraActual);
        cant++;
    }
    // Cierro el archivo.
    if (!file.eof()) {
        std::cerr << "Error al leer el archivo" << std::endl;
        file.close();
        return -1;
    }
    file.close();
    return cant;
}

void cargarArchivoThread(int principio, int fin, HashMapConcurrente& hashMap, vector<std::string>& filePaths)
{
    for (unsigned int index = principio; index < fin; index++) {
        cargarArchivo(hashMap, filePaths[index]);
    }
}

void cargarMultiplesArchivos(
    HashMapConcurrente &hashMap,
    unsigned int cantThreads,
    std::vector<std::string> filePaths
) {
    // Completar (Ejercicio 4)
    unsigned int cantFilePathsPorThread = filePaths.size() / cantThreads;
    unsigned int resto = filePaths.size() % cantThreads;

    unsigned int principio = 0;
    unsigned int fin = cantFilePathsPorThread;

    std::vector<thread> threads;
    for (unsigned int i = 0; i < cantThreads; i++) {
        if (resto > 0)
        {
            fin++;
            resto--;
        }

        threads.emplace_back(thread(cargarArchivoThread, principio, fin, std::ref(hashMap), std::ref(filePaths)));

        principio = fin;    // Iteramos hasta fin (no inclusive) en cargarArchivoThread
        fin = (fin + cantFilePathsPorThread > filePaths.size()) ? filePaths.size() : fin + cantFilePathsPorThread;
    }

    for (auto& t : threads) {
        t.join();
    }
}



#endif