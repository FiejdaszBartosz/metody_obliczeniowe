//
// Created by Bartosz Fiejdasz on 05/05/2022.
//

#include <iostream>
#include <math.h>
#include <fstream>

#define iteracje 100

template<typename T>
T obliczKrok(T krok) { return krok / 1.5; }

template<typename T>
T funkcjaZadana(T x) { return sin(x); }

template<typename T>
T dokladnaPochodna(T x) { return cos(x); }

template<typename T>
T roznicaProgresywna2punktowa(T x, T h) { return (funkcjaZadana(x + h) - funkcjaZadana(x)) / h; }

template<typename T>
T roznicaCentralna2punktowa(T x, T h) { return (funkcjaZadana(x + h) - funkcjaZadana(x - h)) / (2.0 * h); }

template<typename T>
T roznicaWsteczna2punktowa(T x, T h) { return (funkcjaZadana(x) - funkcjaZadana(x - h)) / h; }

template<typename T>
T roznicaProgresywna3punktowa(T x, T h) {
  return (-3.0 / 2.0 * funkcjaZadana(x) + 2.0 * funkcjaZadana(x + h) - 1.0 / 2.0 * funkcjaZadana(x + h + h)) / h;
}
template<typename T>
T roznicaWsteczna3punktowa(T x, T h) {
  return (3.0 / 2.0 * funkcjaZadana(x) - 2.0 * funkcjaZadana(x - h) + 1.0 / 2.0 * funkcjaZadana(x - h - h)) / h;
}

template<typename T>
T **oblicz(T **tablica) {
  T **tablicaBledow = new T *[iteracje];    // Tablica przechowujaca bledy w kolejnych iteracjach
  T h = 0.2;    // Pierwszy krok

  // Przedzial [a, b] gdzie c to srodek
  T a = 0.0;
  T b = M_PI / 2.0;
  T c = (a + b) / 2.0;

  for (int i = 0; i < iteracje; ++i) {
    tablica[i] = new T[8];    // Stworzenie wiersza na wyniki
    tablicaBledow[i] = new T[8];    // Stworzenie wiersza na bledy

    tablica[i][0] = roznicaProgresywna2punktowa(a, h);
    tablicaBledow[i][0] = fabs(tablica[i][0] - dokladnaPochodna(a));

    tablica[i][1] = roznicaCentralna2punktowa(c, h);
    tablicaBledow[i][1] = fabs(tablica[i][1] - dokladnaPochodna(c));

    tablica[i][2] = roznicaWsteczna2punktowa(b, h);
    tablicaBledow[i][2] = fabs(tablica[i][2] - dokladnaPochodna(b));

    tablica[i][3] = roznicaProgresywna3punktowa(a, h);
    tablicaBledow[i][3] = fabs(tablica[i][3] - dokladnaPochodna(a));

    tablica[i][4] = roznicaProgresywna3punktowa(c, h);
    tablicaBledow[i][4] = fabs(tablica[i][4] - dokladnaPochodna(c));

    tablica[i][5] = roznicaWsteczna3punktowa(c, h);
    tablicaBledow[i][5] = fabs(tablica[i][5] - dokladnaPochodna(c));

    tablica[i][6] = roznicaWsteczna3punktowa(b, h);
    tablicaBledow[i][6] = fabs(tablica[i][6] - dokladnaPochodna(b));

    tablicaBledow[i][7] = h;

    h = obliczKrok(h);    // Wyznaczenie nowego kroku
    for (int j = 0; j < 7; j++)
      std::cout << tablicaBledow[i][j] << " ";
    std::cout << "\n";
  }

  std::cout << "\n" << "Rzedy dokladnosci: " << "\n";
  for (int i = 0; i < 7; i++) {
    std::cout << (log10(tablicaBledow[1][i]) - log10(tablicaBledow[0][i]))
        / (log10(tablicaBledow[1][7]) - log10(tablicaBledow[0][7])) << "\n";
  }
  std::cout << "\n";
  return tablicaBledow;
}

template<typename T>
void zapisDoPliku(T **tablica, std::string nazwa) {
  std::fstream f;
  f << std::scientific;
  f.precision(16); //ustalamy precyzję
  f.open(nazwa, std::ios::out);

  if (f.good()) {
    for (int i = 0; i < iteracje; i++) {
      f << log10(tablica[i][7]) << " ";
      for (int j = 0; j < 7; j++) {
        if (fabs(log10(tablica[i][j])) > 0)
          f << log10(tablica[i][j]) << " ";
        else
          f << "0 ";
      }
      f << "\n";
    }
    f.close(); //zamykamy plik
  } else
    std::cout << "Blad otwarcia pliku " << "\n";
}

int main() {
  std::cout << std::scientific;
  std::cout.precision(10); //ustalamy precyzję

  std::cout << "FLOAT\n";
  float** pochodnaFloat = new float* [iteracje]; //tworzymy tablice dwuwymiarową typu float
  pochodnaFloat = oblicz(pochodnaFloat);
  zapisDoPliku(pochodnaFloat, "pochodnaFloat.txt"); //zapisujemy wyniki do pliku

  std::cout << "DOUBLE\n";
  double** pochodnaDouble = new double* [iteracje];  //tworzymy tablice dwuwymiarową typu double
  pochodnaDouble = oblicz(pochodnaDouble);
  zapisDoPliku(pochodnaDouble, "pochodnaDouble.txt"); //zapisujemy wyniki do pliku

  std::cout << "Zakonczono.\n";
}
// os x - log10 z kroku
// os y - log10 bledow

// blad obciecia jest proporcjonalny do kroku
// blad maszynowy jest odwrotnie proporcjonalny do kroku