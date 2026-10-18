#include "wzorzec.h"
#include <algorithm>

float odlegloscEuklidesowa(const float *vecA, const float *vecB, int dim)
{
    float suma = 0.0f;
    for (int i = 0; i < dim; ++i)
    {
        float roznica = vecA[i] - vecB[i];
        suma += roznica * roznica;
    }
    return suma;
}

std::vector<Wynik> znajdzTopK(const std::vector<float>& baza, const std::vector<float>& zapytanie, int dim, int k) {
    int liczba_wektorow = baza.size() / dim;
    std::vector<Wynik> dystanse(liczba_wektorow);

    for (int i = 0; i < liczba_wektorow; ++i) {
        dystanse[i].id = i;
        dystanse[i].dystans = odlegloscEuklidesowa(&baza[i * dim], zapytanie.data(), dim);
    }

    auto cmp = [](const Wynik& a, const Wynik& b) {
        if (a.dystans == b.dystans) return a.id < b.id; 
        return a.dystans < b.dystans;
    };

    std::partial_sort(dystanse.begin(), dystanse.begin() + k, dystanse.end(), cmp);
    dystanse.resize(k);
    
    return dystanse;
}