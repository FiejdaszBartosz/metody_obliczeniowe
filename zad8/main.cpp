//
// Created by Bartosz Fiejdasz on 05/05/2022.
//

#include <iostream>
#include <math.h>
#include <fstream>

template<typename T>
T funkcjaZadana(T x) { return sin(x); }

template<typename T>
T dokladnaPochodna(T x) { return cos(x); }

template<typename T>
T roznicaProgresywna2punktowa(T x, T h) { return funkcjaZadana(x + h) - funkcjaZadana(x) / h; }

template<typename T>
T roznicaCentralna2punktowa(T x, T h) { return funkcjaZadana(x + h) - funkcjaZadana(x - h) / (2.0 * h); }

double roznicaCentralna2punktowa(double x, double h) {
  return funkcjaZadana(x + h) - funkcjaZadana(x - h) / (2.0 * h);
}

float roznicaCentralna2punktowa(float x, float h) {
  return funkcjaZadana(x + h) - funkcjaZadana(x - h) / (2.0f * h);
}