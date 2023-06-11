#ifndef CHM_CPP
#define CHM_CPP

#include <iostream>
#include <fstream>
#include <pthread.h>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#include "HashMapConcurrente.hpp"

using namespace std;

std::mutex mutexes[HashMapConcurrente::cantLetras];

HashMapConcurrente::HashMapConcurrente() {
    for (unsigned int i = 0; i < HashMapConcurrente::cantLetras; i++) {
        tabla[i] = new ListaAtomica<hashMapPair>();
    }
}

unsigned int HashMapConcurrente::hashIndex(std::string clave) {
    return (unsigned int)(clave[0] - 'a');
}

void HashMapConcurrente::incrementar(std::string clave) {
    // Completar (Ejercicio 2)
    unsigned int index = hashIndex(clave);
    ListaAtomica<hashMapPair>* lista = tabla[index];
    mutexes[index].lock();
    for(unsigned int i = 0; i < lista->longitud(); i++)
        if (lista->iesimo(i).first == clave) {
            lista->iesimo(i).second++;
            mutexes[index].unlock();
            return;
        }
    mutexes[index].unlock();
    hashMapPair *par = new hashMapPair();
    par->first = clave;
    par->second = 1;
    lista->insertar(*par);
    }


std::vector<std::string> HashMapConcurrente::claves() {
    // Completar (Ejercicio 2)
    std::vector<std::string> claves;
    for (unsigned int index = 0; index < cantLetras; index++) {
        auto it = tabla[index]->crearIt();
        while (it.haySiguiente()) {
            claves.push_back(it.siguiente().first);
            it.avanzar();
        }
    }
    return claves;
}

unsigned int HashMapConcurrente::valor(std::string clave) {
    // Completar (Ejercicio 2)
    unsigned int index = hashIndex(clave);
    ListaAtomica<hashMapPair>* lista = tabla[index];
    for (unsigned int i = 0; i < lista->longitud(); i++) {
        if (lista->iesimo(i).first == clave) {
            return lista->iesimo(i).second;
        }
    }

    return 0;
}

hashMapPair HashMapConcurrente::maximo() {
    hashMapPair *max = new hashMapPair();
    max->second = 0;

    for(unsigned int i = 0; i < cantLetras; i++) mutexes[i].lock();

    for (unsigned int index = 0; index < HashMapConcurrente::cantLetras; index++) {
        auto it = tabla[index]->crearIt();
        while (it.haySiguiente()){

            if (it.siguiente().second > max->second) {
                max->first = it.siguiente().first;
                max->second = it.siguiente().second;
            }
            it.avanzar();
        }
    }

    for(int i = 0; i < cantLetras; i++) mutexes[i].unlock();

    return *max;
}

void buscarMaximoThread(hashMapPair *max, std::mutex *procesados_mutex, vector<bool> *procesados, bool *termino, ListaAtomica<hashMapPair>* tabla[]) {
    
    while(!*termino){
        procesados_mutex->lock();

        int a_procesar=-1;
        for(int i=0; i < procesados->size(); i++){
            if(!(*procesados)[i]){
                a_procesar=i;
                break;
            }
        }

        

        if(a_procesar == -1){
            *termino = true;
            procesados_mutex->unlock();
            return;
        }

        (*procesados)[a_procesar] = true;
        procesados_mutex->unlock();
        std::cout << "HAsta aca llego" << endl;
        
        //Proceso el indice a_procesar
        auto it = tabla[a_procesar]->crearIt();
        while (it.haySiguiente()){
            if (it.siguiente().second > max->second) {
                max->first = it.siguiente().first;
                max->second = it.siguiente().second;
            }
            it.avanzar();
        }
        
    }

}

void buscarMaximoThread(int inicio, int fin, hashMapPair max, ListaAtomica<hashMapPair> tabla[], vector<bool>& visitados, std::mutex& chequeo) {
    for (unsigned int index = inicio; index < fin; index++) {
        bool puedo = false;
        chequeo.lock();
        if(!visitados[index]){
            visitados[index] = true;
        }else{
            puedo = true;
        }
        chequeo.unlock();

        if(puedo) continue;

        auto it = tabla[index]->crearIt();
        while (it.haySiguiente())
        {
            if (it.siguiente().second > max->second) {
                max->first = it.siguiente().first;
                max->second = it.siguiente().second;
            }

            it.avanzar();
        }
    }

    for(int i = 0; i < visitados.size(); i++){
        bool busco = false;
        chequeo.lock();
        if(!visitados[i]){
            visitados[i] = true;
            busco = true;
        }
        chequeo.unlock();
        if(busco){
        auto it = tabla[i]->crearIt();
        while (it.haySiguiente())
        {
            if (it.siguiente().second > max->second) {
                max->first = it.siguiente().first;
                max->second = it.siguiente().second;
            }

            it.avanzar();
        }
        }
    }
}

hashMapPair HashMapConcurrente::maximoParalelo(unsigned int cantThreads) {
    // Completar (Ejercicio 3)
    unsigned int cantListasPorThread = cantLetras / cantThreads;
    unsigned int resto = cantLetras % cantThreads;

    for(unsigned int i = 0; i < cantLetras; i++) mutexes[i].lock();

    unsigned int principio = 0;
    unsigned int fin = cantListasPorThread;

    vector<thread> threads;
    hashMapPair maximos[cantThreads];
    mutex chequeo;
    vector<bool> procesados(cantLetras, false);
    for (unsigned int i = 0; i < cantThreads; i++) {
        if (resto > 0)
        {
            fin++;
            resto--;
        }

        threads.emplace_back(thread(buscarMaximoThread, principio, fin, &maximos[i], this->tabla, std::ref(procesados), std::ref(chequeo)));

        principio = fin;    // Iteramos hasta fin (no inclusive) en buscarMaximoThread
        fin = (fin + cantListasPorThread > cantLetras) ? cantLetras : fin + cantListasPorThread;
    }

    for (auto& t : threads) {
        t.join();
    }

    hashMapPair max = maximos[0];

    for (unsigned int i = 0; i < cantThreads; i++) {
        if (maximos[i].second > max.second) {
            max.first = maximos[i].first;
            max.second = maximos[i].second;
        }
    }

    for(unsigned int i = 0; i < cantLetras; i++) mutexes[i].unlock();

    return max;
}


#endif