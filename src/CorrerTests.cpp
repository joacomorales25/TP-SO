
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <pthread.h>
#include <chrono>
#include "../src/HashMapConcurrente.hpp"
#include "../src/CargarArchivos.hpp"

using namespace std;

#define CANT_ARCHIVOS 12;

void pruebaThreads()
{

    ofstream outputFile3("output_a_threads.txt");
    if (!outputFile3)
    {
        cerr << "Error: Unable to open output file." << endl;
        return;
    }

    ofstream outputFile4("output_rand_threads.txt");
    if (!outputFile4)
    {
        cerr << "Error: Unable to open output file." << endl;
        return;
    }

    HashMapConcurrente *hashMap_no_rand = new HashMapConcurrente();
    HashMapConcurrente *hashMap_rand = new HashMapConcurrente();

    vector<string> filePaths_no_rand;
    vector<string> filePaths_rand;

    for (int i = 5; i < 10000; i += 500)
    {   
        string filePath_no_rand = "../scripts/tests/no_random/" + to_string(i) + ".txt";
        string filePath_rand = "../scripts/tests/random/random" + to_string(i) + ".txt";
        filePaths_no_rand.push_back(filePath_no_rand);
        filePaths_rand.push_back(filePath_rand);
    }

    for (int i = 1; i <= CANT_ARCHIVOS; i++)
    {
        auto start = chrono::high_resolution_clock::now();
        cargarMultiplesArchivos(*hashMap_no_rand, i, filePaths_no_rand);
        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(end - start);

        outputFile3 << i << " " << duration.count() << endl;

        start = chrono::high_resolution_clock::now();
        cargarMultiplesArchivos(*hashMap_rand, i, filePaths_rand);
        end = chrono::high_resolution_clock::now();
        duration = chrono::duration_cast<chrono::microseconds>(end - start);
        outputFile4 << i << " " << duration.count() << endl;
    }
}

int main()
{
    ofstream outputFile("output_a.txt");
    if (!outputFile)
    {
        cerr << "Error: Unable to open output file." << endl;
        return 1;
    }

    ofstream outputFile2("output_rand.txt");
    if (!outputFile2)
    {
        cerr << "Error: Unable to open output file." << endl;
        return 1;
    }

    pruebaThreads();
    HashMapConcurrente *hashMap_no_rand = new HashMapConcurrente();
    HashMapConcurrente *hashMap_rand = new HashMapConcurrente();

    for (int i = 5; i < 10000; i += 500)
    {
        string filePath_no_rand = "../scripts/tests/no_random/" + to_string(i) + ".txt";
        string filePath_rand = "../scripts/tests/random/random" + to_string(i) + ".txt";

        auto start = chrono::high_resolution_clock::now();
        cargarArchivo(*hashMap_no_rand, filePath_no_rand);
        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(end - start);

        outputFile << i << " " << duration.count() << endl;

        start = chrono::high_resolution_clock::now();
        cargarArchivo(*hashMap_rand, filePath_rand);
        end = chrono::high_resolution_clock::now();
        duration = chrono::duration_cast<chrono::microseconds>(end - start);
        outputFile2 << i << " " << duration.count() << endl;
    }

    delete hashMap_no_rand;
    delete hashMap_rand;
}
