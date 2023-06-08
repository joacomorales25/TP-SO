
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include "../src/HashMapConcurrente.hpp"
#include "../src/CargarArchivos.hpp"

using namespace std;

int main(){
    ofstream outputFile("output_a.txt");
    if(!outputFile){
        cerr << "Error: Unable to open output file." << endl;
        return 1;
    }

    ofstream outputFile2("output_rand.txt");
    if(!outputFile2){
        cerr << "Error: Unable to open output file." << endl;
        return 1;
    }


    for(int i = 5; i < 10000; i+= 500){
        string filePath_no_rand = "tests/no_random" + to_string(i) + ".txt";
        string filePath_rand = "tests/random/random" + to_string(i) + ".txt";
        
        HashMapConcurrente* hashMap_no_rand = new HashMapConcurrente();
        HashMapConcurrente* hashMap_rand = new HashMapConcurrente();

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

}