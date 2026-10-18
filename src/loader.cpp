#include "loader.h"
#include <fstream>
#include <iostream>

std::vector<float> wczytajWektory(const std::string& sciezka, int liczba_elementow) {
    std::vector<float> wektory(liczba_elementow);
    std::ifstream plik(sciezka, std::ios::binary);
    if (!plik) {
        std::cerr << "Blad otwarcia pliku: " << sciezka << "\n";
        return {};
    }
    plik.read(reinterpret_cast<char*>(wektory.data()), liczba_elementow * sizeof(float));
    return wektory;
}