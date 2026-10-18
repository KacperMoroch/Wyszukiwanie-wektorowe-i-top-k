#pragma once
#include <vector>

struct Wynik
{
    int id;
    float dystans;
};

float odlegloscEuklidesowa(const float *vecA, const float *vecB, int dim);
std::vector<Wynik> znajdzTopK(const std::vector<float> &baza, const std::vector<float> &zapytanie, int dim, int k);