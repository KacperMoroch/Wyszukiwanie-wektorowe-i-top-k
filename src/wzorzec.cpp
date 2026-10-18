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