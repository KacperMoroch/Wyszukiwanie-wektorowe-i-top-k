#include <iostream>
#include <vector>
#include <chrono>
#include "loader.h"
#include "wzorzec.h"

int main() {
    int db_size = 10000;
    int queries_size = 100;
    int dim = 128;
    int k = 5;

    std::cout << "Wczytywanie danych...\n";
    auto baza = wczytajWektory("../data/baza.bin", db_size * dim);
    auto zapytania = wczytajWektory("../data/zapytania.bin", queries_size * dim);

    if (baza.empty() || zapytania.empty()) {
        std::cerr << "Blad wczytywania danych! Uruchom najpierw skrypt Pythona.\n";
        return 1;
    }

    std::cout << "Rozpoczynam wyszukiwanie sekwencyjne...\n";
    auto start = std::chrono::high_resolution_clock::now();

    for (int q = 0; q < queries_size; ++q) {
        std::vector<float> pojedyncze_zapytanie(zapytania.begin() + q * dim, zapytania.begin() + (q + 1) * dim);
        auto top_k = znajdzTopK(baza, pojedyncze_zapytanie, dim, k);
        
        if (q == 0) {
            std::cout << "Wyniki Top " << k << " dla pierwszego zapytania:\n";
            for (const auto& w : top_k) {
                std::cout << "ID: " << w.id << ", Dystans kwadratowy: " << w.dystans << "\n";
            }
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> czas = end - start;
    std::cout << "Czas przetwarzania (wzorzec sekwencyjny): " << czas.count() << " sekund\n";

    return 0;
}