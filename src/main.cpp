#include <iostream>
#include <vector>
#include <omp.h>
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
        std::cerr << "Blad wczytywania danych!\n";
        return 1;
    }

    std::cout << "Wersja sekwencyjna...\n";
    std::vector<Wynik> top_k_seq;
    double start_seq = omp_get_wtime();
    
    for (int q = 0; q < queries_size; ++q) {
        std::vector<float> pojedyncze_zapytanie(zapytania.begin() + q * dim, zapytania.begin() + (q + 1) * dim);
        auto wyniki = znajdzTopK(baza, pojedyncze_zapytanie, dim, k);
        if (q == 0) top_k_seq = wyniki;
    }
    
    double czas_seq = omp_get_wtime() - start_seq;
    std::cout << "Czas sekwencyjny T(1): " << czas_seq << " sekund\n\n";

    std::cout << "Wersja OpenMP...\n";
    std::vector<Wynik> top_k_omp;
    double start_omp = omp_get_wtime();
    
    for (int q = 0; q < queries_size; ++q) {
        std::vector<float> pojedyncze_zapytanie(zapytania.begin() + q * dim, zapytania.begin() + (q + 1) * dim);
        auto wyniki = znajdzTopK_OpenMP(baza, pojedyncze_zapytanie, dim, k);
        if (q == 0) top_k_omp = wyniki;
    }
    
    double czas_omp = omp_get_wtime() - start_omp;
    std::cout << "Czas OpenMP T(p): " << czas_omp << " sekund\n";
    std::cout << "Przyspieszenie S(p) = " << czas_seq / czas_omp << "x\n\n";

    std::cout << "Weryfikacja wynikow:\n";
    bool zgodne = true;
    for (int i = 0; i < k; ++i) {
        std::cout << "Miejsce " << i+1 << " | Seq: ID " << top_k_seq[i].id << " (Dystans: " << top_k_seq[i].dystans << ") "
                  << "| OMP: ID " << top_k_omp[i].id << " (Dystans: " << top_k_omp[i].dystans << ")\n";
        
        if (top_k_seq[i].id != top_k_omp[i].id || top_k_seq[i].dystans != top_k_omp[i].dystans) {
            zgodne = false;
        }
    }
    
    if (zgodne) {
        std::cout << "\nSUKCES: Wyniki OpenMP sa w 100% zgodne z obliczeniami sekwencyjnymi!\n";
    } else {
        std::cout << "\nBLAD: Wyniki sie nie zgadzaja!\n";
    }

    return 0;
}