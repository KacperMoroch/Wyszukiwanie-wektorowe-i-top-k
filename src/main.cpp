#include <iostream>
#include <vector>
#include <algorithm>
#include <omp.h>
#include <string>
#include "loader.h"
#include "wzorzec.h"

double wyliczMediane(std::vector<double> czasy)
{
    std::sort(czasy.begin(), czasy.end());
    return czasy[czasy.size() / 2];
}

int main(int argc, char *argv[])
{
    double start_calkowity = omp_get_wtime();

    int liczba_watkow = omp_get_max_threads();
    if (argc > 1)
    {
        liczba_watkow = std::stoi(argv[1]);
    }
    omp_set_num_threads(liczba_watkow);

    int db_size = 10000;
    int queries_size = 100;
    int dim = 128;
    int k = 5;
    int liczba_pomiarow = 5;

    std::cout << "KONFIGURACJA:\n";
    std::cout << "Liczba watkow OpenMP: " << liczba_watkow << "\n";
    std::cout << "Liczba pomiarow (po rozgrzewce): " << liczba_pomiarow << "\n\n";

    std::cout << "Wczytywanie danych z plikow...\n";
    auto baza = wczytajWektory("../data/baza.bin", db_size * dim);
    auto zapytania = wczytajWektory("../data/zapytania.bin", queries_size * dim);

    if (baza.empty() || zapytania.empty())
    {
        std::cerr << "Blad wczytywania danych!\n";
        return 1;
    }

    std::cout << "\nRozpoczynamy testy sekwencyjne (Zloty Wzorzec)...\n";
    std::vector<double> czasy_seq;
    std::vector<Wynik> referencja_top_k;

    for (int proba = 0; proba < liczba_pomiarow; ++proba)
    {
        double start_rdzen = omp_get_wtime();

        for (int q = 0; q < queries_size; ++q)
        {
            std::vector<float> zap(zapytania.begin() + q * dim, zapytania.begin() + (q + 1) * dim);
            auto wyniki = znajdzTopK(baza, zap, dim, k);

            if (proba == 0 && q == 0)
                referencja_top_k = wyniki;
        }

        czasy_seq.push_back(omp_get_wtime() - start_rdzen);
    }

    double mediana_seq = wyliczMediane(czasy_seq);
    auto min_max_seq = std::minmax_element(czasy_seq.begin(), czasy_seq.end());

    std::cout << "Mediana czasu T(1): " << mediana_seq << " s\n";
    std::cout << "Rozrzut T(1)      : od " << *min_max_seq.first << " s do " << *min_max_seq.second << " s\n";

    std::cout << "\nRozgrzewka OpenMP w tle...\n";
    for (int q = 0; q < queries_size; ++q)
    {
        std::vector<float> zap(zapytania.begin() + q * dim, zapytania.begin() + (q + 1) * dim);
        znajdzTopK_OpenMP(baza, zap, dim, k);
    }

    std::cout << "Rozpoczynamy wlasciwe testy OpenMP...\n";
    std::vector<double> czasy_omp;
    std::vector<Wynik> test_top_k;

    for (int proba = 0; proba < liczba_pomiarow; ++proba)
    {
        double start_rdzen = omp_get_wtime();

        for (int q = 0; q < queries_size; ++q)
        {
            std::vector<float> zap(zapytania.begin() + q * dim, zapytania.begin() + (q + 1) * dim);
            auto wyniki = znajdzTopK_OpenMP(baza, zap, dim, k);

            if (proba == 0 && q == 0)
                test_top_k = wyniki;
        }

        czasy_omp.push_back(omp_get_wtime() - start_rdzen);
    }

    double mediana_omp = wyliczMediane(czasy_omp);
    auto min_max_omp = std::minmax_element(czasy_omp.begin(), czasy_omp.end());

    std::cout << "Mediana czasu T(p): " << mediana_omp << " s\n";
    std::cout << "Rozrzut T(p)      : od " << *min_max_omp.first << " s do " << *min_max_omp.second << " s\n";

    double przyspieszenie = mediana_seq / mediana_omp;
    double efektywnosc = przyspieszenie / liczba_watkow;

    std::cout << "\nWYNIKI KONCOWE:\n";
    std::cout << "Przyspieszenie S(p): " << przyspieszenie << "x\n";
    std::cout << "Efektywnosc E(p)   : " << efektywnosc * 100 << "%\n\n";

    std::cout << "Weryfikacja ze zlotym wzorcem (Zapytanie 0):\n";
    bool zgodne = true;
    for (int i = 0; i < k; ++i)
    {
        if (referencja_top_k[i].id != test_top_k[i].id || referencja_top_k[i].dystans != test_top_k[i].dystans)
        {
            zgodne = false;
        }
    }

    if (zgodne)
    {
        std::cout << "[SUKCES] Wyniki z OpenMP sa w 100% zgodne z obliczeniami sekwencyjnymi!\n";
    }
    else
    {
        std::cout << "[BLAD] Wykryto wyscig watkow (Race Condition), wyniki sie nie zgadzaja!\n";
    }

    double czas_calkowity = omp_get_wtime() - start_calkowity;
    std::cout << "\nPelny czas przetwarzania programu (wraz z wczytywaniem): " << czas_calkowity << " s\n";

    return 0;
}